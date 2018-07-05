#include <time.h>
#include <stdio.h>
#include <string.h>

#include <SDL.h>
#include <vmath.h>
#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define MEMBUF_IMPL
#include <membuf.h>

#include <array.hpp>
#include <table.hpp>

struct blendfunc_t
{
	GLenum src;
	GLenum dst;
};

struct texture_t
{
	GLuint handle;
	int width;
	int height;

	struct
	{
		GLenum min;
		GLenum mag;
	} filter;

	struct
	{
		GLenum s;
		GLenum t;
	} wrap;

	texture_t(void)
		: width(0)
		, height(0)
		, wrap({ GL_REPEAT, GL_REPEAT })
		, filter({ GL_LINEAR, GL_LINEAR })
	{
		glGenTextures(1, &handle);
	}

	~texture_t(void)
	{
		glDeleteTextures(1, &handle);
	}
};

struct entity_t
{
public:
    vec2  scale;
    vec2  position;
    float rotation;

    vec2  velocity;
    float movespeed;
    
    vec4  color;
    float radius;

    texture_t* texture;
};

namespace game
{
    int screen_width;
    int screen_height;
}

namespace renderer
{
    void draw_texture(texture_t* texture, vec2 position = vec2(0.0f), float rotation = 0.0f, vec2 scale = vec2(1.0f), vec4 color = vec4(1.0f),  blendfunc_t blend = { GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA });
}

namespace table
{
	template <>
	inline bool equal_f(const char* a, const char* b)
	{
		return _stricmp(a, b) == 0;
	}
}

namespace texture
{
	table_t<const char*, texture_t*> textures(membuf_heap());

	void apply(texture_t* texture)
	{
		glBindTexture(GL_TEXTURE_2D, texture->handle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture->wrap.s);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture->wrap.t);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture->filter.mag);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture->filter.min);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void set_pixels(texture_t* texture, int w, int h, const void* pixels)
	{
		if (w > 0 && h > 0 && pixels)
		{
			texture->width  = w;
			texture->height = h;
			glBindTexture(GL_TEXTURE_2D, texture->handle);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	texture_t* load(const char* path)
	{
		texture_t* texture;
		if (table::tryget(textures, path, texture))
		{
			return texture;
		}

		int w, h, n;
		void* pixels = stbi_load(path, &w, &h, &n, 0);
		if (pixels)
		{
			texture = new texture_t();
			texture::apply(texture);
			texture::set_pixels(texture, w, h, pixels);

			stbi_image_free(pixels);

			return texture;
		}
		else
		{
			return NULL;
		}
	}
}

namespace entity
{
    void move(entity_t* entity, float dt)
    {
        entity->position += entity->velocity * entity->movespeed * dt;
        if (entity->velocity.x != 0.0f || entity->velocity.y != 0.0f)
        {
            entity->rotation = atan2f(entity->velocity.y, entity->velocity.x);
        }
    }

    void move(entity_t* entity, vec2 bounds, float dt)
    {
        vec2  pos = entity->position + entity->velocity * entity->movespeed * dt;
        float rad = entity->radius;

        if (pos.x + rad > bounds.x)
        {
            pos.x = bounds.x - rad;
        }
        else if (pos.x - rad < -bounds.x)
        {
            pos.x = rad - bounds.x;
        }

        if (pos.y + rad > bounds.y)
        {
            pos.y = bounds.y - rad;
        }
        else if (pos.y - rad < -bounds.y)
        {
            pos.y = rad - bounds.y;
        }

        entity->position = pos;

        if (entity->velocity.x != 0.0f || entity->velocity.y != 0.0f)
        {
            entity->rotation = atan2f(entity->velocity.y, entity->velocity.x);
        }
    }
}

namespace world
{
    entity_t* player;

    array_t<entity_t> bullets(membuf_heap());
    array_t<entity_t> seekers(membuf_heap());

    array_t<int> remove_bullets(membuf_heap());
    array_t<int> remove_seekers(membuf_heap());

    float fire_timer    = 0.0f;
    float fire_interval = 0.1f;

    float spawn_timer    = 0.0f;
    float spawn_interval = 1.0f;

    bool lock = false;

    void init(SDL_Window* window)
    {
        player = new entity_t();
        player->color     = vec4(1.0f);
        player->position  = vec2(0.0f);
        player->rotation  = 0.0f;
        player->scale     = vec2(1.0f);
        player->movespeed = 720.0f;
        player->texture   = texture::load("Art/Player.png");
        player->radius    = player->texture->width * 0.5f;
    }

    void fire_bullets(vec2 aim_dir)
    {
        float angle = vec2_angle(aim_dir) + (rand() % 101) / 100.0f * (PI * 0.025f);
        float offset = PI * 0.1f;

        aim_dir = vec2(cosf(angle), sinf(angle));

        // First bullet
        {
            vec2 vel = normalize(aim_dir);
            vec2 pos = player->position + vec2(cosf(angle + offset), sinf(angle + offset)) * player->texture->width * 1.25f;
            entity_t& en = array::add(bullets);
            en.color     = vec4(1.0f);
            en.position  = pos;
            en.rotation  = atan2f(vel.y, vel.x);
            en.scale     = vec2(1.0f);
            en.texture   = texture::load("Art/Bullet.png");
            en.velocity  = vel;
            en.movespeed = 1280.0f;
            en.radius    = en.texture->height * 0.5f;
        }

        // Second bullet
        {
            vec2 vel = normalize(aim_dir);
            vec2 pos = player->position + vec2(cosf(angle - offset), sinf(angle - offset)) * player->texture->width * 1.25f;
            entity_t& en = array::add(bullets);
            en.color     = vec4(1.0f);
            en.position  = pos;
            en.rotation  = atan2f(vel.y, vel.x);
            en.scale     = vec2(1.0f);
            en.texture   = texture::load("Art/Bullet.png");
            en.velocity  = vel;
            en.movespeed = 1280.0f;
            en.radius    = en.texture->height * 0.5f;
        }
    }

    vec2 get_spawn_position()
    {
        const float min_distance_sqr = 720.0f * 720.0f;
        
        vec2 pos;

        do
        {
            float x = (2.0f * (rand() % 101) / 100.0f - 1.0f) * game::screen_width;
            float y = (2.0f * (rand() % 101) / 100.0f - 1.0f) * game::screen_height;
            pos = vec2(x, y);
        } while (distancesquared(pos, player->position) < min_distance_sqr);

        return pos;
    }

    void spawn_seeker()
    {
        vec2 pos = get_spawn_position();

        entity_t& en = array::add(seekers);
        en.color     = vec4(1.0f);
        en.velocity  = normalize(player->position - pos);
        en.position  = pos;
        en.movespeed = 360.0f;
        en.scale     = vec2(1.0f);
        en.texture   = texture::load("Art/Seeker.png");
        en.rotation  = 0.0f;
        en.radius    = en.texture->width * 0.5f;
    }

    void destroy_bullet(entity_t* bullet, int index)
    {
        if (lock)
        {
            array::push(remove_bullets, index);
        }
        else
        {
            array::erase(bullets, index);
        }
    }

    void destroy_seeker(entity_t* seeker, int index)
    {
        if (lock)
        {
            array::push(remove_seekers, index);
        }
        else
        {
            array::erase(seekers, index);
        }
    }

    void update(float dt, float vertical, float horizontal, vec2 aim_dir, bool fire)
    {
        // Update is in progress, locking the list
        lock = true;

        player->velocity = normalize(vec2(horizontal, vertical));
        entity::move(player, vec2(game::screen_width, game::screen_height), dt);

        for (int i = 0, n = bullets.count; i < n; i++)
        {
            entity_t* b = &bullets[i];
            entity::move(b, dt);
            if (b->position.x < -game::screen_width || b->position.x > game::screen_width
                || b->position.y < -game::screen_height || b->position.y > game::screen_height)
            {
                destroy_bullet(b, i);
            }
        }

        for (int i = 0, n = seekers.count; i < n; i++)
        {
            entity_t* s = &seekers[i];

            s->velocity = normalize(player->position - s->position);
            entity::move(s, dt);
        }

        for (int i = 0, n = bullets.count; i < n; i++)
        {
            entity_t* b = &bullets[i];

            for (int j = 0, m = seekers.count; j < m; j++)
            {
                entity_t* s = &seekers[j];

                if (distance(b->position, s->position) <= b->radius + s->radius)
                {
                    destroy_bullet(b, i);
                    destroy_seeker(s, j);
                }
            }
        }

        for (int j = 0, m = seekers.count; j < m; j++)
        {
            entity_t* s = &seekers[j];

            if (distance(player->position, s->position) <= player->radius + s->radius)
            {
                array::clear(bullets);
                array::clear(seekers);
                array::clear(remove_bullets);
                array::clear(remove_seekers);

                player->position = vec2();
                player->rotation = 0.0f;
                break;
            }
        }

        // Update is done, unlock the list
        lock = false;

        // Do remove
        for (int i = 0, n = remove_bullets.count; i < n; i++)
        {
            array::erase(bullets, remove_bullets[i]);
        }
        for (int i = 0, n = remove_seekers.count; i < n; i++)
        {
            array::erase(seekers, remove_seekers[i]);
        }
        array::clear(remove_bullets);
        array::clear(remove_seekers);

        // Fire bullet if requested
        if (!fire)
        {
            fire_timer = 0.0f;
        }
        else
        {
            fire_timer += dt;
            if (fire_timer >= fire_interval)
            {
                fire_timer -= fire_interval;
                fire_bullets(aim_dir);
            }
        }

        // Spawn enemies
        spawn_timer += dt;
        if (spawn_timer >= spawn_interval)
        {
            spawn_timer -= spawn_interval;

            spawn_seeker();
        }
    }

    void draw_entity(entity_t* e)
    {
        renderer::draw_texture(e->texture, e->position, e->rotation, e->scale, e->color);
    }

    void render()
    {
        draw_entity(player);

        for (int i = 0, n = bullets.count; i < n; i++)
        {
            draw_entity(&bullets[i]);
        }

        for (int i = 0, n = seekers.count; i < n; i++)
        {
            draw_entity(&seekers[i]);
        }
    }
}

namespace renderer
{
	struct vert_t
	{
		vec2 pos;
		vec2 uv;
	};

	struct drawcmd_t
	{
		texture_t* texture;
		int        draw_count;
		vec2       position;
		vec2       scale;
		float      rotation;
        vec4       color;
		
		blendfunc_t blend;
	};

	array_t<drawcmd_t>      drawcmds(membuf_heap());
	array_t<vert_t>         vertices(membuf_heap());
	array_t<unsigned short> indices(membuf_heap());

	GLuint vao;
	GLuint vbo;
	GLuint ebo;

	GLuint vshader;
	GLuint fshader;
	GLuint program;

	mat4 proj_matrix;

	const char* vshader_src =
		"#version 330 core\n"
		"layout (location = 0) in vec4 vertex;"
		"out vec2 uv;"
		"uniform mat4 MVP;"
		"void main() {"
		"uv = vertex.zw;"
		"gl_Position = MVP * vec4(vertex.xy, 0, 1.0);"
		"}";

	const char* fshader_src =
		"#version 330 core\n"
		"in vec2 uv;"
		"out vec4 fragColor;"
        "uniform vec4 color;"
		"uniform sampler2D image;"
		"void main() {"
		"fragColor = texture(image, uv) * color;"
		"}";

	static GLuint create_shader(GLenum type, const char* src)
	{
		GLuint shader = glCreateShader(type);
		glShaderSource(shader, 1, &src, NULL);
		glCompileShader(shader);

		GLint status;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (!status)
		{
			char infolog[1024];
			glGetShaderInfoLog(shader, sizeof(infolog), NULL, infolog);
			fprintf(stderr, "[Error] renderer::create_shader: %s\n", infolog);

			glDeleteShader(shader);
			return 0;
		}

		return shader;
	}

	static GLuint create_program(GLuint vshader, GLuint fshader)
	{
		GLuint program = glCreateProgram();
		glAttachShader(program, vshader);
		glAttachShader(program, fshader);
		glLinkProgram(program);

		GLint status;
		glGetProgramiv(program, GL_LINK_STATUS, &status);
		if (!status)
		{
			char infolog[1024];
			glGetProgramInfoLog(program, sizeof(infolog), NULL, infolog);
			fprintf(stderr, "[Error] renderer::create_program: %s\n", infolog);

			glDeleteProgram(program);
			return 0;
		}

		return program;
	}

	void init(SDL_Window* window)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		int w, h;
		SDL_GetWindowSize(window, &w, &h);
		glViewport(0, 0, w, h);

		proj_matrix = ortho(-w, w, -h, h, -10.0f, 10.0f);

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		vshader = create_shader(GL_VERTEX_SHADER, vshader_src);
		fshader = create_shader(GL_FRAGMENT_SHADER, fshader_src);
		program = create_program(vshader, fshader); 

		if (!vshader || !fshader || !program)
		{
			fprintf(stderr, "An error occured, press any key to exit...");
			getchar();
			exit(1);
		}

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), NULL);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void draw_texture(texture_t* texture, vec2 position, float rotation, vec2 scale, vec4 color, blendfunc_t blend)
	{
		drawcmd_t cmd;
		cmd.texture = texture;
		cmd.position = position;
		cmd.rotation = rotation;
		cmd.scale = scale * vec2(texture->width, texture->height);
		cmd.blend = blend;
        cmd.color = color;
		cmd.draw_count = 6;
		array::push(drawcmds, cmd);

		unsigned short i = (unsigned short)array::count(indices);
		array::push(indices, (unsigned short)(i + 0U));
		array::push(indices, (unsigned short)(i + 1U));
		array::push(indices, (unsigned short)(i + 2U));
		array::push(indices, (unsigned short)(i + 0U));
		array::push(indices, (unsigned short)(i + 2U));
		array::push(indices, (unsigned short)(i + 3U));

		vert_t v;
		v.pos = vec2(-0.5f, -0.5f);
		v.uv = vec2(0.0f, 1.0f);
		array::push(vertices, v);

		v.pos = vec2(-0.5f, 0.5f);
		v.uv = vec2(0.0f, 0.0f);
		array::push(vertices, v);

		v.pos = vec2(0.5f, 0.5f);
		v.uv = vec2(1.0f, 0.0f);
		array::push(vertices, v);

		v.pos = vec2(0.5f, -0.5f);
		v.uv = vec2(1.0f, 1.0f);
		array::push(vertices, v);
	}

	void prepair()
	{
		array::clear(drawcmds);
		array::clear(vertices);
		array::clear(indices);
	}

	void present()
	{
		glUseProgram(program);

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vert_t) * array::count(vertices), array::elements(vertices), GL_STREAM_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * array::count(indices), array::elements(indices), GL_STREAM_DRAW);

		int offset = 0;
		for (int i = 0, n = array::count(drawcmds); i < n; i++)
		{
			const drawcmd_t& cmd = drawcmds[i];

			if (cmd.texture)
			{
				glBlendFunc(cmd.blend.src, cmd.blend.dst);

				mat4 model_matrix = translate(cmd.position) * mat4_rotatez(cmd.rotation) * scale(cmd.scale);
				mat4 MVP_matrix = proj_matrix * model_matrix;
				
				GLint location = glGetUniformLocation(program, "MVP");
                GLint colorloc = glGetUniformLocation(program, "color");

				glUniformMatrix4fv(location, 1, GL_FALSE, MVP_matrix);
                glUniform4f(colorloc, cmd.color.r, cmd.color.g, cmd.color.b, cmd.color.a);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, cmd.texture->handle);
				glDrawElements(GL_TRIANGLES, cmd.draw_count, GL_UNSIGNED_SHORT, (void*)(intptr_t)offset);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glUseProgram(0);
	}
}

namespace game
{
    SDL_Window* window;

    vec2 aim;
    bool fire;

	float axis_vertical   = 0.0f;
	float axis_horizontal = 0.0f;

	texture_t* texture;
	void init(SDL_Window* window)
	{
        // System
        srand(time(NULL));

        // Initialize screen
        game::window = window;
        SDL_GetWindowSize(window, &screen_width, &screen_height);

        // Initialize world
        world::init(window);

        // Initialize render engine
		renderer::init(window);

        // Sample texture
		texture = texture::load("Art/Player.png");
	}

	void input(const SDL_Event* e)
	{
		switch (e->type)
		{
		case SDL_KEYUP:
			switch (e->key.keysym.sym)
			{
            case SDLK_s:
                if (axis_vertical < 0.0f) axis_vertical = 0.0f;
                break;

			case SDLK_w:
				if (axis_vertical > 0.0f) axis_vertical = 0.0f;
                break;

            case SDLK_a:
                if (axis_horizontal < 0.0f) axis_horizontal = 0.0f;
                break;

            case SDLK_d:
                if (axis_horizontal > 0.0f) axis_horizontal = 0.0f;
                break;
			}
			break;

		case SDL_KEYDOWN:
            switch (e->key.keysym.sym)
            {
            case SDLK_s:
                axis_vertical = -1.0f;
                break;

            case SDLK_w:
                axis_vertical = 1.0f;
                break;

            case SDLK_a:
                axis_horizontal = -1.0f;
                break;

            case SDLK_d:
                axis_horizontal = 1.0f;
                break;
            }
			break;

        case SDL_MOUSEBUTTONDOWN:
            if (e->button.button == SDL_BUTTON_LEFT)
            {
                fire = true;
            }
            break;

        case SDL_MOUSEBUTTONUP:
            if (e->button.button == SDL_BUTTON_LEFT)
            {
                fire = false;
            }
            break;

        case SDL_MOUSEMOTION:
            vec2 clip = vec2(2.0f * e->motion.x / (float)screen_width - 1.0f, 1.0f - 2.0f * e->motion.y / (float)screen_height);

            vec2 mpos = vec2(clip.x * screen_width, clip.y * screen_height);
            aim = normalize(mpos - world::player->position);
            break;
		}

        vec2 axes = normalize(vec2(axis_horizontal, axis_vertical));
        axis_vertical = axes.y;
        axis_horizontal = axes.x;
	}

    void update(float dt)
    {
        world::update(dt, axis_vertical, axis_horizontal, aim, fire);
    }

    void render(void)
    {
		renderer::prepair();

        world::render();

		renderer::present();
    }
}