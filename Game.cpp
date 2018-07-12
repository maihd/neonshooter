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
    bool  active;

    vec2  scale;
    vec2  position;
    float rotation;

    vec2  velocity;
    float movespeed;
    
    vec4  color;
    float radius;

    texture_t* texture;
};

struct particle_t
{
    bool       active;
    texture_t* texture;
    vec2       velocity;
    vec2       position;
    float      rotation;
    vec2       scale;
    vec4       color;
    float      timer;
    float      duration;
};

namespace game
{
    float total_time;
    int screen_width;
    int screen_height;
}

namespace color
{
    vec4 hsv(float h, float s, float v)
    {
        if (h == 0 && s == 0)
            return vec4(v, v, v, 1.0f);

        float c = s * v;
        float x = c * (1 - fabsf(fmodf(h, 2) - 1));
        float m = v - c;

        if (h < 1)      return vec4(c + m, x + m, m, 1.0f);
        else if (h < 2) return vec4(x + m, c + m, m, 1.0f);
        else if (h < 3) return vec4(m, c + m, x + m, 1.0f);
        else if (h < 4) return vec4(m, x + m, c + m, 1.0f);
        else if (h < 5) return vec4(x + m, m, c + m, 1.0f);
        else            return vec4(c + m, m, x + m, 1.0f);
    }
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

	void set_pixels(texture_t* texture, int w, int h, const void* pixels, GLenum infmt = GL_RGBA, GLenum fmt = GL_RGBA)
	{
		if (w > 0 && h > 0 && pixels)
		{
			texture->width  = w;
			texture->height = h;
			glBindTexture(GL_TEXTURE_2D, texture->handle);
			glTexImage2D(GL_TEXTURE_2D, 0, infmt, w, h, 0, fmt, GL_UNSIGNED_BYTE, pixels);
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
            char ext[32];
            _splitpath_s(path, NULL, 0, NULL, 0, NULL, 0, ext, sizeof(ext));
            GLenum fmt, infmt;
            if (_stricmp(ext, ".jpg") == 0 || _stricmp(ext, ".jpeg") == 0)
            {
                fmt   = GL_RGB;
                infmt = GL_RGBA;
            }
            else
            {
                fmt   = GL_RGBA;
                infmt = GL_RGBA;
            }

			texture = new texture_t();
			texture::apply(texture);
			texture::set_pixels(texture, w, h, pixels, infmt, fmt);

			stbi_image_free(pixels);
            table::set(textures, path, texture);

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

namespace particle_system
{
    void spawn_particle(texture_t* texture, vec2 pos, vec4 tint, float duration, vec2 scale, float theta, vec2 vel);
}

namespace world
{
    entity_t* player;

    array_t<entity_t> bullets(membuf_heap());
    array_t<entity_t> seekers(membuf_heap());
    array_t<entity_t> wanderers(membuf_heap());
    array_t<entity_t> blackholes(membuf_heap());

    array_t<int> free_bullets(membuf_heap());
    array_t<int> free_seekers(membuf_heap());
    array_t<int> free_wanderers(membuf_heap());
    array_t<int> free_blackholes(membuf_heap());

    int seeker_spawn_rate    = 80;
    int wanderer_spawn_rate  = 60;
    int blackhole_spawn_rate = 20;

    float fire_timer = 0.0f;
    float fire_interval = 0.1f;

    float spawn_timer = 0.0f;
    float spawn_interval = 1.0f;

    bool lock = false;
    float game_over_timer = 0.0f;

    void init(SDL_Window* window)
    {
        player = new entity_t();
        player->active = true;
        player->color = vec4(1.0f);
        player->position = vec2(0.0f);
        player->rotation = 0.0f;
        player->scale = vec2(1.0f);
        player->movespeed = 720.0f;
        player->texture = texture::load("Art/Player.png");
        player->radius = player->texture->width * 0.5f;

        array::ensure(bullets  , 256);
        array::ensure(seekers  , 256);
        array::ensure(wanderers, 256);
    }

    void spawn_bullet(vec2 pos, vec2 vel)
    {
        entity_t* en = NULL;
        if (array::count(free_bullets) > 0)
        {
            int index = array::pop(free_bullets);
            en = &bullets[index];
        }
        else
        {
            en = &array::add(bullets);
        }

        en->active = true;
        en->color = vec4(1.0f);
        en->position = pos;
        en->rotation = atan2f(vel.y, vel.x);
        en->scale = vec2(1.0f);
        en->texture = texture::load("Art/Bullet.png");
        en->velocity = vel;
        en->movespeed = 1280.0f;
        en->radius = en->texture->height * 0.5f;
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
            spawn_bullet(pos, vel);
        }

        // Second bullet
        {
            vec2 vel = normalize(aim_dir);
            vec2 pos = player->position + vec2(cosf(angle - offset), sinf(angle - offset)) * player->texture->width * 1.25f;
            spawn_bullet(pos, vel);
        }
    }

    vec2 get_spawn_position()
    {
        const float min_distance_sqr = (game::screen_height * 0.3f) * (game::screen_height * 0.3f);

        vec2 pos;

        do
        {
            float x = (2.0f * (rand() % 101) / 100.0f - 1.0f) * 0.8f * game::screen_width;
            float y = (2.0f * (rand() % 101) / 100.0f - 1.0f) * 0.8f * game::screen_height;
            pos = vec2(x, y);
        } while (distancesquared(pos, player->position) < min_distance_sqr);

        return pos;
    }

    void spawn_seeker()
    {
        vec2 pos = get_spawn_position();

        entity_t* en = NULL;
        if (array::count(free_seekers) > 0)
        {
            int index = array::pop(free_seekers);
            en = &seekers[index];
        }
        else
        {
            en = &array::add(seekers);
        }

        en->active    = true;
        en->color     = vec4(1.0f, 1.0f, 1.0f, 0.0f);
        en->velocity  = normalize(player->position - pos);
        en->position  = pos;
        en->movespeed = 360.0f;
        en->scale     = vec2(1.0f);
        en->texture   = texture::load("Art/Seeker.png");
        en->rotation  = atan2f(en->velocity.y, en->velocity.x);
        en->radius    = en->texture->width * 0.5f;
    }

    void spawn_wanderer()
    {
        vec2 pos = get_spawn_position();

        entity_t* en = NULL;
        if (array::count(free_wanderers) > 0)
        {
            int index = array::pop(free_wanderers);
            en = &wanderers[index];
        }
        else
        {
            en = &array::add(wanderers);
        }

        en->active    = true;
        en->color     = vec4(1.0f, 1.0f, 1.0f, 0.0f);
        en->velocity  = normalize(player->position - pos);
        en->position  = pos;
        en->movespeed = 240.0f;
        en->scale     = vec2(1.0f);
        en->texture   = texture::load("Art/wanderer.png");
        en->rotation  = atan2f(en->velocity.y, en->velocity.x);
        en->radius    = en->texture->width * 0.5f;
    }

    void spawn_blackhole()
    {
        vec2 pos = get_spawn_position();

        entity_t* en = NULL;
        if (array::count(free_blackholes) > 0)
        {
            int index = array::pop(free_blackholes);
            en = &blackholes[index];
        }
        else
        {
            en = &array::add(blackholes);
        }

        en->active    = true;
        en->color     = vec4(1.0f, 1.0f, 1.0f, 0.0f);
        en->velocity  = vec2(0.0f);
        en->position  = pos;
        en->movespeed = 0;
        en->scale     = vec2(1.0f);
        en->texture   = texture::load("Art/Black Hole.png");
        en->rotation  = 0;
        en->radius    = en->texture->width * 0.5f;
    }

    void destroy_bullet(entity_t* bullet, int index, bool explosion = false)
    {
        bullet->active = false;
        array::push(free_bullets, index);

        if (explosion)
        {
            const int PARTICLE_COUNT = 30;
            texture_t* texture = texture::load("Art/Laser.png");

            for (int i = 0; i < PARTICLE_COUNT; i++)
            {
                float speed = 640.0f * (0.2f + (rand() % 101 / 100.0f) * 0.8f);
                float angle = rand() % 101 / 100.0f * 2 * PI;
                vec2 vel = vec2(cosf(angle) * speed, sinf(angle) *speed);

                vec4 color = vec4(0.6f, 1.0f, 1.0f, 1.0f);
                particle_system::spawn_particle(texture, bullet->position, color, 1.0f, vec2(1.0f), 0.0f, vel);
            }
        }
    }

    void destroy_seeker(entity_t* seeker, int index)
    {
        seeker->active = false;
        array::push(free_seekers, index);

        texture_t* texture = texture::load("Art/Laser.png");

        float hue1 = rand() % 101 / 100.0f * 6.0f;
        float hue2 = fmodf(hue1 + (rand() % 101 / 100.0f * 2.0f), 6.0f);
        vec4 color1 = color::hsv(hue1, 0.5f, 1);
        vec4 color2 = color::hsv(hue2, 0.5f, 1);

        for (int i = 0; i < 120; i++)
        {
            float speed = 640.0f * (0.2f + (rand() % 101 / 100.0f) * 0.8f);
            float angle = rand() % 101 / 100.0f * 2 * PI;
            vec2 vel = vec2(cosf(angle) * speed, sinf(angle) *speed);

            vec4 color = mix(color1, color2, rand() % 101 / 100.0f);
            particle_system::spawn_particle(texture, seeker->position, color, 1.0f, vec2(1.0f), 0.0f, vel);
        }
    }

    void destroy_wanderer(entity_t* wanderer, int index)
    {
        wanderer->active = false;
        array::push(free_wanderers, index);

        texture_t* texture = texture::load("Art/Laser.png");

        float hue1 = rand() % 101 / 100.0f * 6.0f;
        float hue2 = fmodf(hue1 + (rand() % 101 / 100.0f * 2.0f), 6.0f);
        vec4 color1 = color::hsv(hue1, 0.5f, 1);
        vec4 color2 = color::hsv(hue2, 0.5f, 1);

        for (int i = 0; i < 120; i++)
        {
            float speed = 640.0f * (0.2f + (rand() % 101 / 100.0f) * 0.8f);
            float angle = rand() % 101 / 100.0f * 2 * PI;
            vec2 vel = vec2(cosf(angle) * speed, sinf(angle) *speed);

            vec4 color = mix(color1, color2, rand() % 101 / 100.0f);
            particle_system::spawn_particle(texture, wanderer->position, color, 1.0f, vec2(1.0f), 0.0f, vel);
        }
    }

    void destroy_blackhole(entity_t* blaclhole, int index)
    {
        blaclhole->active = false;
        array::push(free_blackholes, index);

        texture_t* texture = texture::load("Art/Laser.png");

        float hue1 = rand() % 101 / 100.0f * 6.0f;
        float hue2 = fmodf(hue1 + (rand() % 101 / 100.0f * 2.0f), 6.0f);
        vec4 color1 = color::hsv(hue1, 0.5f, 1);
        vec4 color2 = color::hsv(hue2, 0.5f, 1);

        for (int i = 0; i < 120; i++)
        {
            float speed = 640.0f * (0.2f + (rand() % 101 / 100.0f) * 0.8f);
            float angle = rand() % 101 / 100.0f * 2 * PI;
            vec2 vel = vec2(cosf(angle) * speed, sinf(angle) *speed);

            vec4 color = mix(color1, color2, rand() % 101 / 100.0f);
            particle_system::spawn_particle(texture, blaclhole->position, color, 1.0f, vec2(1.0f), 0.0f, vel);
        }
    }

    void game_over()
    {
        array::clear(bullets);
        array::clear(seekers);
        array::clear(wanderers);
        array::clear(blackholes);
        array::clear(free_bullets);
        array::clear(free_seekers);
        array::clear(free_wanderers);
        array::clear(free_blackholes);

        game_over_timer = 3.0f;
        texture_t* texture = texture::load("Art/Laser.png");

        float hue1 = rand() % 101 / 100.0f * 6.0f;
        float hue2 = fmodf(hue1 + (rand() % 101 / 100.0f * 2.0f), 6.0f);
        vec4 color1 = color::hsv(hue1, 0.5f, 1);
        vec4 color2 = color::hsv(hue2, 0.5f, 1);

        for (int i = 0; i < 1200; i++)
        {
            float speed = 10.0f * max(game::screen_width, game::screen_height) * (0.6f + (rand() % 101 / 100.0f) * 0.4f);
            float angle = rand() % 101 / 100.0f * 2 * PI;
            vec2 vel = vec2(cosf(angle) * speed, sinf(angle) * speed);

            vec4 color = mix(color1, color2, rand() % 101 / 100.0f);
            particle_system::spawn_particle(texture, player->position, color, game_over_timer, vec2(1.0f), 0.0f, vel);
        }

        player->position = vec2();
        player->velocity = vec2();
        player->rotation = 0.0f;
    }

    bool update_blackhole(entity_t* blackhole, entity_t* other)
    {
        if (distance(other->position, blackhole ->position) <= other->radius + blackhole->radius)
        {
            return true;
        }
        else if (distance(other->position, blackhole->position) <= other->radius + blackhole->radius * 10.0f)
        {
            vec2 diff = blackhole->position - other->position;
            other->velocity += normalize(diff) * step(1, 0, length(diff) / (game::screen_width * 0.2f));
            other->velocity = normalize(other->velocity);
        }

        return false;
    }

    void update(float dt, float vertical, float horizontal, vec2 aim_dir, bool fire)
    {
        if (game_over_timer > 0.0f)
        {
            game_over_timer -= dt;
            return;
        }

        // Update is in progress, locking the list
        lock = true;

        player->velocity = step(player->velocity, normalize(vec2(horizontal, vertical)), 5.0f * dt);
        entity::move(player, vec2(game::screen_width, game::screen_height), dt);
        if (lengthsquared(player->velocity) > 0.1f && fmodf(game::total_time, 0.025f) <= 0.01f)
        {
            float speed;
            float angle = vec2_angle(player->velocity);
            
            texture_t* glow_tex = texture::load("Art/Laser.png");
            texture_t* line_tex = texture::load("Art/Laser.png");

            vec2 vel = -0.25f * player->movespeed * player->velocity;
            vec2 pos = player->position + 45.0f * (-player->velocity);
            vec2 nvel = vec2(vel.y, -vel.x) * 0.9f * sinf(game::total_time * 10.0f);
            float alpha = 0.7f;

            vec2 mid_vel = vel;
            particle_system::spawn_particle(glow_tex, pos, vec4(1.0f, 0.7f, 0.1f, 1.0f) * alpha, 0.4f, vec2(1.0f, 2.0f), angle, mid_vel);
            particle_system::spawn_particle(line_tex, pos, vec4(1.0f, 1.0f, 1.0f, 1.0f) * alpha, 0.4f, vec2(1.0f, 1.0f), angle, mid_vel);

            speed = rand() % 101 / 100.0f * 40.0f;
            angle = rand() % 101 / 100.0f * 2.0f * PI;
            vec2 side_vel1 = vel + nvel + vec2(cosf(angle), sinf(angle)) * speed;
            particle_system::spawn_particle(glow_tex, pos, vec4(0.8f, 0.2f, 0.1f, 1.0f) * alpha, 0.4f, vec2(1.0f, 2.0f), angle, side_vel1);
            particle_system::spawn_particle(line_tex, pos, vec4(1.0f, 1.0f, 1.0f, 1.0f) * alpha, 0.4f, vec2(1.0f, 1.0f), angle, side_vel1);

            speed = rand() % 101 / 100.0f * 40.0f;
            angle = rand() % 101 / 100.0f * 2.0f * PI;
            vec2 side_vel2 = vel - nvel + vec2(cosf(angle), sinf(angle)) * speed;
            particle_system::spawn_particle(glow_tex, pos, vec4(0.8f, 0.2f, 0.1f, 1.0f) * alpha, 0.4f, vec2(1.0f, 2.0f), angle, side_vel2);
            particle_system::spawn_particle(line_tex, pos, vec4(1.0f, 1.0f, 1.0f, 1.0f) * alpha, 0.4f, vec2(1.0f, 1.0f), angle, side_vel2);
        }                                   

        for (int i = 0, n = bullets.count; i < n; i++)
        {
            entity_t* b = &bullets[i];
            if (b->active)
            {
                entity::move(b, dt);
                if (b->position.x < -game::screen_width || b->position.x > game::screen_width
                    || b->position.y < -game::screen_height || b->position.y > game::screen_height)
                {
                    destroy_bullet(b, i, true);
                }
            }
        }

        for (int i = 0, n = seekers.count; i < n; i++)
        {
            entity_t* s = &seekers[i];
            if (s->active)
            {
                if (s->color.a < 1.0f)
                {
                    s->color.a += dt;
                    if (s->color.a > 1.0f)
                    {
                        s->color.a = 1.0f;
                    }
                }
                else
                {
                    s->velocity = normalize(s->velocity + normalize(player->position - s->position) * 10.0f * dt);
                    entity::move(s, dt);
                }
            }
        }

        for (int i = 0, n = wanderers.count; i < n; i++)
        {
            entity_t* s = &wanderers[i];
            if (s->active)
            {
                if (s->color.a < 1.0f)
                {
                    s->color.a += dt;
                    if (s->color.a > 1.0f)
                    {
                        s->color.a = 1.0f;
                    }
                }
                else
                {
                    const int INTERPOLATIONS = 6;
                    const float real_speed = s->movespeed / INTERPOLATIONS;

                    float direction = vec2_angle(s->velocity);
                    for (int j = 0; j < INTERPOLATIONS; j++)
                    {
                        direction += (0.12f * (rand() % 101 / 100.0f) - 0.06f) * PI;

                        if (s->position.x < -game::screen_width || s->position.x > game::screen_width
                            || s->position.y < -game::screen_height || s->position.y > game::screen_height)
                        {
                            direction = vec2_angle(-s->position) + (1.0f * (rand() % 101 / 100.0f) - 0.5f) * PI;
                        }

                        s->rotation = direction;
                        s->velocity = vec2(cosf(direction), sinf(direction));
                        s->position = s->position + s->velocity * real_speed * dt;
                    }
                }
            }
        }

        for (int i = 0, n = bullets.count; i < n; i++)
        {
            entity_t* b = &bullets[i];

            if (!b->active) continue;
            for (int j = 0, m = seekers.count; j < m; j++)
            {
                entity_t* s = &seekers[j];
                if (!s->active || s->color.a < 1.0f) continue;

                if (distance(b->position, s->position) <= b->radius + s->radius)
                {
                    destroy_bullet(b, i);
                    destroy_seeker(s, j);
                    break;
                }
            }

            if (!b->active) continue;
            for (int j = 0, m = wanderers.count; j < m; j++)
            {
                entity_t* s = &wanderers[j];
                if (!s->active || s->color.a < 1.0f) continue;

                if (distance(b->position, s->position) <= b->radius + s->radius)
                {
                    destroy_bullet(b, i);
                    destroy_wanderer(s, j);
                    break;
                }
            }

            if (!b->active) continue;
            for (int j = 0, m = blackholes.count; j < m; j++)
            {
                entity_t* s = &blackholes[j];
                if (!s->active || s->color.a < 1.0f) continue;

                float d = distance(b->position, s->position);
                if (d <= b->radius + s->radius)
                {
                    destroy_bullet(b, i);
                    destroy_blackhole(s, j);
                    break;
                }
                else if (d <= b->radius + s->radius * 5.0f)
                {
                    float r = b->radius + s->radius * 5.0f;
                    float t = (d - r) / r;
                    b->velocity = normalize(b->velocity + normalize(b->position - s->position) * 0.3f);
                }
            }
        }

        for (int j = 0, m = seekers.count; j < m; j++)
        {
            entity_t* s = &seekers[j];
            if (!s->active || s->color.a < 1.0f) continue;

            if (distance(player->position, s->position) <= player->radius + s->radius)
            {
                game_over();
                break;
            }
        }

        for (int j = 0, m = wanderers.count; j < m; j++)
        {
            entity_t* s = &wanderers[j];
            if (!s->active || s->color.a < 1.0f) continue;

            if (distance(player->position, s->position) <= player->radius + s->radius)
            {
                game_over();
                break;
            }
        }

        for (int i = 0, n = blackholes.count; i < n; i++)
        {
            entity_t* s = &blackholes[i];
            if (s->active)
            {
                texture_t* glow_tex = texture::load("Art/Glow.png");
                texture_t* line_tex = texture::load("Art/Laser.png");

                vec4 color1 = vec4(0.3f, 0.8f, 0.4f, 1.0f);
                vec4 color2 = vec4(0.5f, 1.0f, 0.7f, 1.0f);

                if (fmodf(game::total_time, 0.1f) <= 0.01f)
                {
                    float speed = 16.0f * s->radius * (0.8f + (rand() % 101 / 100.0f) * 0.2f);
                    float angle = rand() % 101 / 100.0f * game::total_time;
                    vec2 vel = vec2(cosf(angle) * speed, sinf(angle) * speed);
                    vec2 pos = s->position + 0.4f * vec2(vel.y, -vel.x) + (4.0f + rand() % 101 / 100.0f * 4.0f);

                    vec4 color = mix(color1, color2, rand() % 101 / 100.0f);
                    //particle_system::spawn_particle(glow_tex, pos, color, 4.0f, vec2(0.3f, 1.0f), 0.0f, vec2(-vel.y, vel.x));
                    particle_system::spawn_particle(line_tex, pos, color, 4.0f, vec2(1.0f, 1.0f), 0.0f, vel);
                }

                if (s->color.a < 1.0f)
                {
                    s->color.a += dt;
                    if (s->color.a > 1.0f)
                    {
                        s->color.a = 1.0f;
                    }
                }
                else
                {
                    if (update_blackhole(s, player))
                    {
                        game_over();
                        break;
                    }

                    for (int j = 0, m = seekers.count; j < m; j++)
                    {
                        entity_t* other = &seekers[j];
                        if (!other->active || other->color.a < 1.0f) continue;

                        if (update_blackhole(s, other))
                        {
                            destroy_seeker(other, j);
                            break;
                        }
                    }

                    for (int j = 0, m = wanderers.count; j < m; j++)
                    {
                        entity_t* other = &wanderers[j];
                        if (!other->active || other->color.a < 1.0f) continue;

                        if (update_blackhole(s, other))
                        {
                            destroy_wanderer(other, j);
                            break;
                        }
                    }
                }
            }
        }

        // Update is done, unlock the list
        lock = false;

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

            if (rand() % 101 < seeker_spawn_rate) spawn_seeker();
            if (rand() % 101 < wanderer_spawn_rate) spawn_wanderer();
            if (rand() % 101 < blackhole_spawn_rate) spawn_blackhole();
        }
    }

    void draw_entity(entity_t* e)
    {
        if (e->active)
        {
            renderer::draw_texture(e->texture, e->position, e->rotation, e->scale, e->color);
        }
    }

    void render()
    {
        if (game_over_timer > 0)
        {
            return;
        }

        draw_entity(player);

        for (int i = 0, n = seekers.count; i < n; i++)
        {
            draw_entity(&seekers[i]);
        }

        for (int i = 0, n = wanderers.count; i < n; i++)
        {
            draw_entity(&wanderers[i]);
        }

        for (int i = 0, n = blackholes.count; i < n; i++)
        {
            draw_entity(&blackholes[i]);
        }

        for (int i = 0, n = bullets.count; i < n; i++)
        {
            draw_entity(&bullets[i]);
        }
    }
}

namespace particle_system
{
    array_t<particle_t> particles(membuf_heap());
    array_t<int> free_particles(membuf_heap());

    void init(void)
    {
        array::ensure(particles, 20 * 1024);
    }

    void spawn_particle(texture_t* texture, vec2 pos, vec4 tint, float duration, vec2 scale, float theta, vec2 vel)
    {
        particle_t* p = NULL;
        if (array::count(free_particles) > 0)
        {
            int index = array::pop(free_particles);
            p = &particles[index];
        } 
        else
        {
            p = &array::add(particles);
        }

        p->active   = true;
        p->scale    = scale;
        p->texture  = texture;
        p->velocity = vel;
        p->position = pos;
        p->rotation = theta;
        p->color    = tint;
        p->timer    = 0.0f;
        p->duration = duration;
    }

    bool update_particle(particle_t& p, float dt)
    {
        bool result = false;

        if (p.active)
        {
            p.timer += dt;
            if (p.timer >= p.duration)
            {
                result = true;
                p.active = false;
            }

            p.rotation  = vec2_angle(p.velocity);
            p.position += p.velocity * dt;
            p.velocity *= 1.0f - 3 * dt;

            p.scale.x = 1.0f - p.timer / p.duration;
            p.color.a = 1.0f - p.timer / p.duration;

            if (p.position.x < -game::screen_width)
            {
                p.velocity.x = fabsf(p.velocity.x);
                p.position.x = -game::screen_width;
            }
            else if (p.position.x > game::screen_width)
            {
                p.velocity.x = -fabsf(p.velocity.x);
                p.position.x = game::screen_width;
            }

            if (p.position.y < -game::screen_width)
            {
                p.velocity.y = fabsf(p.velocity.y);
                p.position.y = -game::screen_height;
            }
            else if (p.position.y > game::screen_width)
            {
                p.velocity.y = -fabsf(p.velocity.y);
                p.position.y = game::screen_height;
            }

            for (int i = 0, n = world::blackholes.count; i < n; i++)
            {
                entity_t* blackhole = &world::blackholes[i];
                if (!blackhole->active) continue;

                vec2 diff = blackhole->position - p.position;
                float d = length(diff);
                vec2 normal = normalize(diff);
                p.velocity += normal * max(0.0f, game::screen_width / d);

                // add tangential acceleration for nearby particles
                if (d < 10.0f * blackhole->radius)
                {
                    p.velocity += vec2(normal.y, -normal.x) * (21.0f * blackhole->radius / (120.0f + 1.2f * d));
                }
            }
        }

        return result;
    }

    void update(float dt)
    {
        for (int i = 0, n = particles.count; i < n; i++)
        {
            particle_t& p = particles[i];
            if (update_particle(p, dt))
            {
                array::push(free_particles, i);
            }
        }
    }

    void render()
    {
        blendfunc_t blend = { GL_SRC_ALPHA, GL_ONE };
        for (int i = 0, n = particles.count; i < n; i++)
        {
            particle_t& p = particles[i];
            if (p.active)
            {
                renderer::draw_texture(p.texture, p.position, p.rotation, p.scale, p.color, blend);
            }
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

    GLuint     framebuf;
    texture_t* frametex; 
    GLuint     framevao;
    GLuint     framevbo;
    GLuint     frameprog;

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

        frametex = new texture_t();
        texture::apply(frametex);
        glBindTexture(GL_TEXTURE_2D, frametex->handle);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
        glBindTexture(GL_TEXTURE_2D, 0);
        glGenFramebuffersEXT(1, &framebuf);
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuf);
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, frametex->handle, 0);

        GLenum status;
        status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
        switch (status)
        {
        case GL_FRAMEBUFFER_COMPLETE_EXT:
            printf("renderer::init(): Success create framebuffer.\n");
            break;

        default:
            fprintf(stderr, "Failed to create framebuffer.\n");
            fprintf(stderr, "An error occured, press any key to exit...");
            getchar();
            exit(1);
            break;
        }

        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

        // Create framebuffer render object
        {
            glGenVertexArrays(1, &framevao);
            glGenBuffers(1, &framevbo);

            vec2 vertices[] = 
            {
                // First triangle
                vec2(-1.0f, -1.0f), vec2(0.0f, 0.0f),
                vec2(-1.0f,  1.0f), vec2(0.0f, 1.0f),
                vec2( 1.0f,  1.0f), vec2(1.0f, 1.0f),

                // Second triangle
                vec2( 1.0f,  1.0f), vec2(1.0f, 1.0f),
                vec2( 1.0f, -1.0f), vec2(1.0f, 0.0f),
                vec2(-1.0f, -1.0f), vec2(0.0f, 0.0f),
            };

            glBindVertexArray(framevao);
            glBindBuffer(GL_ARRAY_BUFFER, framevbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), NULL);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        // Create post processing shader
        printf("renderer::init(): Starting create framebuffer's shader...\n");
        {
            const char* vshader_src =
                "#version 330 core\n"
                "layout (location = 0) in vec4 vertex;"
                "out vec2 uv;"
                "void main() {"
                "uv = vertex.zw;"
                "gl_Position = vec4(vertex.xy, 0, 1.0);"
                "}";

            const char* fshader_src =
                "#version 330 core\n"
                "in vec2 uv;"
                "out vec4 fragColor;"
                "uniform sampler2D image;"
                "void main() {"
                "float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);"
                "vec2 texOffset = 1.0 / textureSize(image, 0);"
                "vec3 result = texture(image, uv).rgb * weight[0];"
                "for(int i = 1; i < 5; ++i) {"
                "result += texture(image, uv + vec2(texOffset.x * i, 0.0)).rgb * weight[i];"
                "result += texture(image, uv - vec2(texOffset.x * i, 0.0)).rgb * weight[i];"
                "}"
                "result = texture(image, uv).rgb + result;"
                "result = vec3(1.0) - exp(-result * 0.5f);"
                "result = pow(result, vec3(1.0 / 2.2));"
                "fragColor = vec4(result, 1.0);"
                "}";

            GLuint vshader = create_shader(GL_VERTEX_SHADER, vshader_src);
            GLuint fshader = create_shader(GL_FRAGMENT_SHADER, fshader_src);
            frameprog = create_program(vshader, fshader);

            if (!vshader || !fshader || !frameprog)
            {
                fprintf(stderr, "An error occured, press any key to exit...");
                getchar();
                exit(1);
            }

            glDeleteShader(vshader);
            glDeleteShader(fshader);
            printf("renderer::init(): Create framebuffer's shader successfully.\n");
        }
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
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuf);
        glClear(GL_COLOR_BUFFER_BIT);
        
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
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

        glUseProgram(frameprog);
        glBindVertexArray(framevao);
        glBindBuffer(GL_ARRAY_BUFFER, framevbo);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, frametex->handle);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
	}
}

namespace game
{
    SDL_Window* window;

    vec2 aim;
    bool fire;

	float axis_vertical   = 0.0f;
	float axis_horizontal = 0.0f;

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

        SDL_InitSubSystem(SDL_INIT_JOYSTICK);
        if (SDL_NumJoysticks() > 0)
        {
            SDL_Joystick* joystick = SDL_JoystickOpen(0);
            
            if (joystick)
            {
                printf("Opened Joystick 0\n");
                printf("Name: %s\n", SDL_JoystickNameForIndex(0));
                printf("Number of Axes: %d\n", SDL_JoystickNumAxes(joystick));
                printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(joystick));
                printf("Number of Balls: %d\n", SDL_JoystickNumBalls(joystick));
            }
            else
            {
                printf("Couldn't open Joystick 0\n");
            }

        }
	}

	void input()
	{
        const Uint8* keys = SDL_GetKeyboardState(NULL);

        if (keys[SDL_SCANCODE_W])
        {
            axis_vertical = step(axis_vertical, 1.0f, 0.1f);
        }
        else
        {
            axis_vertical = step(axis_vertical, 0.0f, 0.1f);
        }

        if (keys[SDL_SCANCODE_S])
        {
            axis_vertical = step(axis_vertical, -1.0f, 0.1f);
        }
        else
        {
            axis_vertical = step(axis_vertical, 0.0f, 0.1f);
        }

        if (keys[SDL_SCANCODE_A])
        {
            axis_horizontal = step(axis_horizontal, -1.0f, 0.1f);
        }
        else
        {
            axis_horizontal = step(axis_horizontal, 0.0f, 0.1f);
        }

        if (keys[SDL_SCANCODE_D])
        {
            axis_horizontal = step(axis_horizontal, 1.0f, 0.1f);
        }
        else
        {
            axis_horizontal = step(axis_horizontal, 0.0f, 0.1f);
        }

        int mx;
        int my;
        Uint32 mouse = SDL_GetMouseState(&mx, &my);
        fire = mouse & SDL_BUTTON(SDL_BUTTON_LEFT);
        {
            vec2 clip = vec2(2.0f * mx / (float)screen_width - 1.0f, 1.0f - 2.0f * my / (float)screen_height);

            vec2 mpos = vec2(clip.x * screen_width, clip.y * screen_height);
            
            vec2 taim = normalize(mpos - world::player->position);

        #if 0
            float cur_angle = vec2_angle(aim);
            float aim_angle = vec2_angle(taim);

            cur_angle = step(cur_angle, aim_angle, 0.8f);
            aim = vec2(cosf(cur_angle), sinf(cur_angle));
        #endif

            aim = step(aim, taim, 0.8f);
        }

        SDL_Joystick* joystick = SDL_JoystickOpen(0);
        if (joystick)
        {
            axis_vertical = step(axis_vertical, -(SDL_JoystickGetAxis(joystick, 1)) / (float)SHRT_MAX, 0.1f);
            axis_horizontal = step(axis_horizontal, (SDL_JoystickGetAxis(joystick, 0)) / (float)SHRT_MAX, 0.1f);

            float x = (SDL_JoystickGetAxis(joystick, 3)) / (float)SHRT_MAX;
            float y = -(SDL_JoystickGetAxis(joystick, 4)) / (float)SHRT_MAX;
            if (length(vec2(x, y)) < 0.01f)
            {
                aim = vec2();
            }
            else
            {
                fire = true;
            
            #if 0
                float cur_angle = vec2_angle(aim);
                float aim_angle = atan2f(y, x);

                cur_angle = step(cur_angle, aim_angle, 0.8f);
                aim = vec2(cosf(cur_angle), sinf(cur_angle));
            #endif

                aim.x = step(aim.x, x, 0.6f);
                aim.y = step(aim.y, y, 0.6f);
            }
        }

        vec2 axes = vec2(axis_horizontal, axis_vertical);
        if (length(axes) < 0.01f)
        {
            axes = vec2();
        }
        else
        {
            axes = clamplength(axes, 0, 1.0f);;
        }
        axis_vertical   = axes.y;
        axis_horizontal = axes.x;
	}

    void update(float dt)
    {
        total_time += dt;

        world::update(dt, axis_vertical, axis_horizontal, aim, fire);
        particle_system::update(dt);
    }

    void render(void)
    {
		renderer::prepair();

        particle_system::render();
        world::render();

		renderer::present();
    }
}