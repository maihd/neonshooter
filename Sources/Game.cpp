#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <Mojo/GL.h>
#include <Mojo/Math.h>
#include <Mojo/Input.h>
#include <Mojo/Assets.h>
#include <Mojo/Window.h>

#include <Mojo/Array.h>
#include <Mojo/HashTable.h>

#define PI 3.14f

struct Entity
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

    Texture texture;

    void Update(float dt)
    {
        position += velocity * movespeed * dt;
        if (velocity.x != 0.0f || velocity.y != 0.0f)
        {
            rotation = atan2f(velocity.y, velocity.x);
        }
    }

    void Update(vec2 bounds, float dt)
    {
        vec2  pos = position + velocity * movespeed * dt;
        float rad = radius;

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

        position = pos;

        if (velocity.x != 0.0f || velocity.y != 0.0f)
        {
            rotation = atan2f(velocity.y, velocity.x);
        }
    }
};

struct Particle
{
    bool       active;
    Texture    texture;
    vec2       velocity;
    vec2       position;
    float      rotation;
    vec2       scale;
    vec4       color;
    float      timer;
    float      duration;
};

static HashTable<Texture> _textures;

int HashString(const char* path)
{
    uint32_t h = (uint32_t)0;
    int length = (int)strlen(path);
    const uint8_t* key = (const uint8_t*)path;

    if (length > 3)
    {
        const uint32_t* key_x4 = (const uint32_t*)key;
        int i = length >> 2;

        do
        {
            uint32_t k = *key_x4++;
            k *= 0xcc9e2d51;
            k = (k << 15) | (k >> 17);
            k *= 0x1b873593;
            h ^= k;
            h = (h << 13) | (h >> 19);
            h = (h * 5) + 0xe6546b64;
        } while (--i);

        key = (const uint8_t*)key_x4;
    }

    if (length & 3)
    {
        int i = length & 3;
        uint32_t k = 0;
        key = &key[i - 1];

        do {
            k <<= 8;
            k |= *key--;
        } while (--i);

        k *= 0xcc9e2d51;
        k = (k << 15) | (k >> 17);
        k *= 0x1b873593;
        h ^= k;
    }

    h ^= length;
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;

    return h;
}

Texture LoadTexture(const char* path)
{
    int hash = HashString(path);

    Texture texture;
    if (_textures.TryGetValue(hash, &texture))
    {
        return texture;
    }

    int w, h, n;
    void* pixels = Assets::LoadImage(path, &w, &h, &n);
    if (pixels)
    {
        texture = Texture::Create(TextureType::Texture2D);

        texture.SetWrap(TextureWrap::Repeat);
        texture.SetFilter(TextureFilter::Linear);
        texture.SetPixels(w, h, n == 4 ? PixelFormat::RGBA : PixelFormat::RGB, pixels);

        Assets::FreeImage(pixels);
        _textures.SetValue(hash, texture);
    }

    return texture; 
}

namespace Color
{
    vec4 HSV(float h, float s, float v)
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


namespace Game
{
    static float totalTime;
}

namespace ParticleSystem
{
    void SpawnParticle(const Texture& texture, vec2 pos, vec4 tint, float duration, vec2 scale, float theta, vec2 vel);
}

namespace Renderer
{
    void DrawTexture(
        const Texture& texture, 
        vec2 position = vec2(0.0f), 
        float rotation = 0.0f, 
        vec2 scale = vec2(1.0f), 
        vec4 color = vec4(1.0f), 
        BlendFunc blend = { BlendFactor::SrcAlpha, BlendFactor::InvertSrcAlpha });
}

namespace World
{
    Entity player;

    Array<Entity> bullets;
    Array<Entity> seekers;
    Array<Entity> wanderers;
    Array<Entity> blackHoles;

    Array<int> freeBullets;
    Array<int> freeSeekers;
    Array<int> freeWanderers;
    Array<int> freeBlackHoles;

    int seekerSpawnRate = 80;
    int wandererSpawnRate = 60;
    int blackHoleSpawnRate = 20;

    float fireTimer = 0.0f;
    float fireInterval = 0.1f;

    float spawnTimer = 0.0f;
    float spawnInterval = 1.0f;

    bool  lock = false;
    float gameOverTimer = 0.0f;

    void Init(void)
    {
        player.active    = true;
        player.color     = vec4(1.0f);
        player.position  = vec2(0.0f);
        player.rotation  = 0.0f;
        player.scale     = vec2(1.0f);
        player.movespeed = 720.0f;
        player.texture   = LoadTexture("Art/Player.png");
        player.radius    = player.texture.width * 0.5f;

        bullets.Ensure(256);
        seekers.Ensure(256);
        wanderers.Ensure(256);
    }

    void spawn_bullet(vec2 pos, vec2 vel)
    {
        Entity* en = NULL;
        if (freeBullets.count > 0)
        {
            int index = freeBullets.Pop();
            en = &bullets[index];
        }
        else
        {
            en = &bullets[bullets.NewSlot()];
        }

        en->active      = true;
        en->color       = vec4(1.0f);
        en->position    = pos;
        en->rotation    = atan2f(vel.y, vel.x);
        en->scale       = vec2(1.0f);
        en->texture     = LoadTexture("Art/Bullet.png");
        en->velocity    = vel;
        en->movespeed   = 1280.0f;
        en->radius      = en->texture.height * 0.5f;
    }

    void fire_bullets(vec2 aim_dir)
    {
        float angle = atan2f(aim_dir.y, aim_dir.x) + (rand() % 101) / 100.0f * (PI * 0.025f);
        float offset = PI * 0.1f;

        aim_dir = vec2(cosf(angle), sinf(angle));

        // First bullet
        {
            vec2 vel = normalize(aim_dir);
            vec2 pos = player.position + vec2(cosf(angle + offset), sinf(angle + offset)) * player.texture.width * 1.25f;
            spawn_bullet(pos, vel);
        }

        // Second bullet
        {
            vec2 vel = normalize(aim_dir);
            vec2 pos = player.position + vec2(cosf(angle - offset), sinf(angle - offset)) * player.texture.width * 1.25f;
            spawn_bullet(pos, vel);
        }
    }

    vec2 get_spawn_position()
    {
        const float min_distance_sqr = (Window::GetHeight() * 0.3f);

        vec2 pos;

        do
        {
            float x = (2.0f * (rand() % 101) / 100.0f - 1.0f) * 0.8f * Window::GetWidth();
            float y = (2.0f * (rand() % 101) / 100.0f - 1.0f) * 0.8f * Window::GetHeight();
            pos = vec2(x, y);
        } while (distsqr(pos, player.position) < min_distance_sqr);

        return pos;
    }

    void spawn_seeker()
    {
        //audio::play_spawn();

        vec2 pos = get_spawn_position();

        Entity* en = NULL;
        if (freeSeekers.count > 0)
        {
            int index = freeSeekers.Pop();
            en = &seekers[index];
        }
        else
        {
            en = &seekers[seekers.NewSlot()];
        }

        en->active      = true;
        en->color       = vec4(1.0f, 1.0f, 1.0f, 0.0f);
        en->velocity    = normalize(player.position - pos);
        en->position    = pos;
        en->movespeed   = 360.0f;
        en->scale       = vec2(1.0f);
        en->texture     = LoadTexture("Art/Seeker.png");
        en->rotation    = atan2f(en->velocity.y, en->velocity.x);
        en->radius      = en->texture.width * 0.5f;
    }

    void spawn_wanderer()
    {
        //audio::play_spawn();

        vec2 pos = get_spawn_position();

        Entity* en = NULL;
        if (freeWanderers.count > 0)
        {
            int index = freeWanderers.Pop();
            en = &wanderers[index];
        }
        else
        {
            en = &wanderers[wanderers.NewSlot()];
        }

        en->active      = true;
        en->color       = vec4(1.0f, 1.0f, 1.0f, 0.0f);
        en->velocity    = normalize(player.position - pos);
        en->position    = pos;
        en->movespeed   = 240.0f;
        en->scale       = vec2(1.0f);
        en->texture     = LoadTexture("Art/Wanderer.png");
        en->rotation    = atan2f(en->velocity.y, en->velocity.x);
        en->radius      = en->texture.width * 0.5f;
    }

    void spawn_blackhole()
    {
        //audio::play_spawn();

        vec2 pos = get_spawn_position();

        Entity* en = NULL;
        if (freeBlackHoles.count > 0)
        {
            int index = freeBlackHoles.Pop();
            en = &blackHoles[index];
        }
        else
        {
            en = &blackHoles[blackHoles.NewSlot()];
        }

        en->active      = true;
        en->color       = vec4(1.0f, 1.0f, 1.0f, 0.0f);
        en->velocity    = vec2(0.0f);
        en->position    = pos;
        en->movespeed   = 0;
        en->scale       = vec2(1.0f);
        en->texture     = LoadTexture("Art/Black Hole.png");
        en->rotation    = 0;
        en->radius      = en->texture.width * 0.5f;
    }

    void destroy_bullet(Entity* bullet, int index, bool explosion = false)
    {
        bullet->active = false;
        freeBullets.Push(index);

        if (explosion)
        {
            const int PARTICLE_COUNT = 30;
            Texture texture = LoadTexture("Art/Laser.png");

            for (int i = 0; i < PARTICLE_COUNT; i++)
            {
                float speed = 640.0f * (0.2f + (rand() % 101 / 100.0f) * 0.8f);
                float angle = rand() % 101 / 100.0f * 2 * PI;
                vec2 vel = vec2(cosf(angle) * speed, sinf(angle) *speed);

                vec4 color = vec4(0.6f, 1.0f, 1.0f, 1.0f);
                ParticleSystem::SpawnParticle(texture, bullet->position, color, 1.0f, vec2(1.0f), 0.0f, vel);
            }
        }
    }

    void DestroySeeker(Entity* seeker, int index)
    {
        //audio::play_explosion();

        seeker->active = false;
        freeSeekers.Push(index);

        Texture texture = LoadTexture("Art/Laser.png");

        float hue1 = rand() % 101 / 100.0f * 6.0f;
        float hue2 = fmodf(hue1 + (rand() % 101 / 100.0f * 2.0f), 6.0f);
        vec4 color1 = Color::HSV(hue1, 0.5f, 1);
        vec4 color2 = Color::HSV(hue2, 0.5f, 1);

        for (int i = 0; i < 120; i++)
        {
            float speed = 640.0f * (0.2f + (rand() % 101 / 100.0f) * 0.8f);
            float angle = rand() % 101 / 100.0f * 2 * PI;
            vec2 vel = vec2(cosf(angle) * speed, sinf(angle) * speed);

            vec4 color = color1 + (color2 - color1) * ((rand() % 101) / 100.0f);
            ParticleSystem::SpawnParticle(texture, seeker->position, color, 1.0f, vec2(1.0f), 0.0f, vel);
        }
    }

    void destroy_wanderer(Entity* wanderer, int index)
    {
        //audio::play_explosion();

        wanderer->active = false;
        freeWanderers.Push(index);

        Texture texture = LoadTexture("Art/Laser.png");

        float hue1 = rand() % 101 / 100.0f * 6.0f;
        float hue2 = fmodf(hue1 + (rand() % 101 / 100.0f * 2.0f), 6.0f);
        vec4 color1 = Color::HSV(hue1, 0.5f, 1);
        vec4 color2 = Color::HSV(hue2, 0.5f, 1);

        for (int i = 0; i < 120; i++)
        {
            float speed = 640.0f * (0.2f + (rand() % 101 / 100.0f) * 0.8f);
            float angle = rand() % 101 / 100.0f * 2 * PI;
            vec2 vel = vec2(cosf(angle) * speed, sinf(angle) * speed);

            vec4 color = color1 + (color2 - color1) * ((rand() % 101) / 100.0f);
            ParticleSystem::SpawnParticle(texture, wanderer->position, color, 1.0f, vec2(1.0f), 0.0f, vel);
        }
    }

    void destroy_blackhole(Entity* blaclhole, int index)
    {
        //audio::play_explosion();

        blaclhole->active = false;
        freeBlackHoles.Push(index);

        Texture texture = LoadTexture("Art/Laser.png");

        float hue1 = rand() % 101 / 100.0f * 6.0f;
        float hue2 = fmodf(hue1 + (rand() % 101 / 100.0f * 2.0f), 6.0f);
        vec4 color1 = Color::HSV(hue1, 0.5f, 1);
        vec4 color2 = Color::HSV(hue2, 0.5f, 1);

        for (int i = 0; i < 120; i++)
        {
            float speed = 640.0f * (0.2f + (rand() % 101 / 100.0f) * 0.8f);
            float angle = rand() % 101 / 100.0f * 2 * PI;
            vec2 vel = vec2(cosf(angle) * speed, sinf(angle) * speed);

            vec4 color = color1 + (color2 - color1) * ((rand() % 101) / 100.0f);
            ParticleSystem::SpawnParticle(texture, blaclhole->position, color, 1.0f, vec2(1.0f), 0.0f, vel);
        }
    }

    void game_over()
    {
        //audio::play_explosion();

        bullets.Clear();
        seekers.Clear();
        wanderers.Clear();
        blackHoles.Clear();
        freeBullets.Clear();
        freeSeekers.Clear();
        freeWanderers.Clear();
        freeBlackHoles.Clear();

        gameOverTimer = 3.0f;
        Texture texture = LoadTexture("Art/Laser.png");

        float hue1 = rand() % 101 / 100.0f * 6.0f;
        float hue2 = fmodf(hue1 + (rand() % 101 / 100.0f * 2.0f), 6.0f);
        vec4 color1 = Color::HSV(hue1, 0.5f, 1);
        vec4 color2 = Color::HSV(hue2, 0.5f, 1);

        for (int i = 0; i < 1200; i++)
        {
            float speed = 10.0f * max(Window::GetWidth(), Window::GetHeight()) * (0.6f + (rand() % 101 / 100.0f) * 0.4f);
            float angle = rand() % 101 / 100.0f * 2 * PI;
            vec2 vel = vec2(cosf(angle) * speed, sinf(angle) * speed);

            vec4 color = color1 + (color2 - color1) * ((rand() % 101) / 100.0f);
            ParticleSystem::SpawnParticle(texture, player.position, color, gameOverTimer, vec2(1.0f), 0.0f, vel);
        }

        player.position = vec2();
        player.velocity = vec2();
        player.rotation = 0.0f;
    }

    bool UpdateBlackhole(Entity* blackhole, Entity* other)
    {
        if (distance(other->position, blackhole->position) <= other->radius + blackhole->radius)
        {
            return true;
        }
        else if (distance(other->position, blackhole->position) <= other->radius + blackhole->radius * 10.0f)
        {
            vec2 diff = blackhole->position - other->position;
            other->velocity += normalize(diff) * lerp(1, 0, length(diff) / (Window::GetWidth() * 0.2f));
            other->velocity = normalize(other->velocity);
        }

        return false;
    }

    void Update(float dt, float vertical, float horizontal, vec2 aim_dir, bool fire)
    {
        if (gameOverTimer > 0.0f)
        {
            gameOverTimer -= dt;
            return;
        }

        // Update is in progress, locking the list
        lock = true;

        player.velocity = lerp(player.velocity, normalize(vec2(horizontal, vertical)), 5.0f * dt);
        player.Update(vec2(Window::GetWidth(), Window::GetHeight()), dt);
        if (lensqr(player.velocity) > 0.1f && fmodf(Game::totalTime, 0.025f) <= 0.01f)
        {
            float speed;
            float angle = atan2f(player.velocity.y, player.velocity.x);

            Texture glow_tex = LoadTexture("Art/Laser.png");
            Texture line_tex = LoadTexture("Art/Laser.png");

            vec2 vel = -0.25f * player.movespeed * player.velocity;
            vec2 pos = player.position + 45.0f * (-player.velocity);
            vec2 nvel = vec2(vel.y, -vel.x) * 0.9f * sinf(Game::totalTime * 10.0f);
            float alpha = 0.7f;

            vec2 mid_vel = vel;
            ParticleSystem::SpawnParticle(glow_tex, pos, vec4(1.0f, 0.7f, 0.1f, 1.0f) * alpha, 0.4f, vec2(1.0f, 2.0f), angle, mid_vel);
            ParticleSystem::SpawnParticle(line_tex, pos, vec4(1.0f, 1.0f, 1.0f, 1.0f) * alpha, 0.4f, vec2(1.0f, 1.0f), angle, mid_vel);

            speed = rand() % 101 / 100.0f * 40.0f;
            angle = rand() % 101 / 100.0f * 2.0f * PI;
            vec2 side_vel1 = vel + nvel + vec2(cosf(angle), sinf(angle)) * speed;
            ParticleSystem::SpawnParticle(glow_tex, pos, vec4(0.8f, 0.2f, 0.1f, 1.0f) * alpha, 0.4f, vec2(1.0f, 2.0f), angle, side_vel1);
            ParticleSystem::SpawnParticle(line_tex, pos, vec4(1.0f, 1.0f, 1.0f, 1.0f) * alpha, 0.4f, vec2(1.0f, 1.0f), angle, side_vel1);

            speed = rand() % 101 / 100.0f * 40.0f;
            angle = rand() % 101 / 100.0f * 2.0f * PI;
            vec2 side_vel2 = vel - nvel + vec2(cosf(angle), sinf(angle)) * speed;
            ParticleSystem::SpawnParticle(glow_tex, pos, vec4(0.8f, 0.2f, 0.1f, 1.0f) * alpha, 0.4f, vec2(1.0f, 2.0f), angle, side_vel2);
            ParticleSystem::SpawnParticle(line_tex, pos, vec4(1.0f, 1.0f, 1.0f, 1.0f) * alpha, 0.4f, vec2(1.0f, 1.0f), angle, side_vel2);
        }

        for (int i = 0, n = bullets.count; i < n; i++)
        {
            Entity* b = &bullets[i];
            if (b->active)
            {
                b->Update(dt);
                if (b->position.x < -Window::GetWidth() || b->position.x > Window::GetWidth()
                    || b->position.y < -Window::GetHeight() || b->position.y > Window::GetHeight())
                {
                    destroy_bullet(b, i, true);
                }
            }
        }

        for (int i = 0, n = seekers.count; i < n; i++)
        {
            Entity* s = &seekers[i];
            if (s->active)
            {
                if (s->color.w < 1.0f)
                {
                    s->color.w += dt;
                    if (s->color.w > 1.0f)
                    {
                        s->color.w = 1.0f;
                    }
                }
                else
                {
                    s->velocity = normalize(s->velocity + normalize(player.position - s->position) * 10.0f * dt);
                    s->Update(dt);
                }
            }
        }

        for (int i = 0, n = wanderers.count; i < n; i++)
        {
            Entity* s = &wanderers[i];
            if (s->active)
            {
                if (s->color.w < 1.0f)
                {
                    s->color.w += dt;
                    if (s->color.w > 1.0f)
                    {
                        s->color.w = 1.0f;
                    }
                }
                else
                {
                    const int INTERPOLATIONS = 6;
                    const float real_speed = s->movespeed / INTERPOLATIONS;

                    float direction = atan2f(s->velocity.y, s->velocity.x);
                    for (int j = 0; j < INTERPOLATIONS; j++)
                    {
                        direction += (0.12f * (rand() % 101 / 100.0f) - 0.06f) * PI;

                        if (s->position.x < -Window::GetWidth() || s->position.x > Window::GetWidth()
                            || s->position.y < -Window::GetHeight() || s->position.y > Window::GetHeight())
                        {
                            direction = atan2f(-s->position.y, -s->position.x) + (1.0f * (rand() % 101 / 100.0f) - 0.5f) * PI;
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
            Entity* b = &bullets[i];

            if (!b->active) continue;
            for (int j = 0, m = seekers.count; j < m; j++)
            {
                Entity* s = &seekers[j];
                if (!s->active || s->color.w < 1.0f) continue;

                if (distance(b->position, s->position) <= b->radius + s->radius)
                {
                    destroy_bullet(b, i);
                    DestroySeeker(s, j);
                    break;
                }
            }

            if (!b->active) continue;
            for (int j = 0, m = wanderers.count; j < m; j++)
            {
                Entity* s = &wanderers[j];
                if (!s->active || s->color.w < 1.0f) continue;

                if (distance(b->position, s->position) <= b->radius + s->radius)
                {
                    destroy_bullet(b, i);
                    destroy_wanderer(s, j);
                    break;
                }
            }

            if (!b->active) continue;
            for (int j = 0, m = blackHoles.count; j < m; j++)
            {
                Entity* s = &blackHoles[j];
                if (!s->active || s->color.w < 1.0f) continue;

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
            Entity* s = &seekers[j];
            if (!s->active || s->color.w < 1.0f) continue;

            if (distance(player.position, s->position) <= player.radius + s->radius)
            {
                game_over();
                break;
            }
        }

        for (int j = 0, m = wanderers.count; j < m; j++)
        {
            Entity* s = &wanderers[j];
            if (!s->active || s->color.w < 1.0f) continue;

            if (distance(player.position, s->position) <= player.radius + s->radius)
            {
                game_over();
                break;
            }
        }

        for (int i = 0, n = blackHoles.count; i < n; i++)
        {
            Entity* s = &blackHoles[i];
            if (s->active)
            {
                Texture glow_tex = LoadTexture("Art/Glow.png");
                Texture line_tex = LoadTexture("Art/Laser.png");

                vec4 color1 = vec4(0.3f, 0.8f, 0.4f, 1.0f);
                vec4 color2 = vec4(0.5f, 1.0f, 0.7f, 1.0f);

                if (fmodf(Game::totalTime, 0.1f) <= 0.01f)
                {
                    float speed = 16.0f * s->radius * (0.8f + (rand() % 101 / 100.0f) * 0.2f);
                    float angle = rand() % 101 / 100.0f * Game::totalTime;
                    vec2 vel = vec2(cosf(angle) * speed, sinf(angle) * speed);
                    vec2 pos = s->position + 0.4f * vec2(vel.y, -vel.x) + (4.0f + rand() % 101 / 100.0f * 4.0f);

                    vec4 color = color1 + (color2 - color1) * ((rand() % 101) / 100.0f);
                    //ParticleSystem::SpawnParticle(glow_tex, pos, color, 4.0f, vec2(0.3f, 1.0f), 0.0f, vec2(-vel.y, vel.x));
                    ParticleSystem::SpawnParticle(line_tex, pos, color, 4.0f, vec2(1.0f, 1.0f), 0.0f, vel);
                }

                if (s->color.w < 1.0f)
                {
                    s->color.w += dt;
                    if (s->color.w > 1.0f)
                    {
                        s->color.w = 1.0f;
                    }
                }
                else
                {
                    if (UpdateBlackhole(s, &player))
                    {
                        game_over();
                        break;
                    }

                    for (int j = 0, m = seekers.count; j < m; j++)
                    {
                        Entity* other = &seekers[j];
                        if (!other->active || other->color.w < 1.0f) continue;

                        if (UpdateBlackhole(s, other))
                        {
                            DestroySeeker(other, j);
                            break;
                        }
                    }

                    for (int j = 0, m = wanderers.count; j < m; j++)
                    {
                        Entity* other = &wanderers[j];
                        if (!other->active || other->color.w < 1.0f) continue;

                        if (UpdateBlackhole(s, other))
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
            fireTimer = 0.0f;
            //audio::stop_shoot();
        }
        else
        {
            fireTimer += dt;
            if (fireTimer >= fireInterval)
            {
                fireTimer = 0;
                fire_bullets(aim_dir);
                //audio::play_shoot();
            }
        }

        // Spawn enemies
        spawnTimer += dt;
        if (spawnTimer >= spawnInterval)
        {
            spawnTimer -= spawnInterval;

            if (rand() % 101 < seekerSpawnRate) spawn_seeker();
            if (rand() % 101 < wandererSpawnRate) spawn_wanderer();
            if (rand() % 101 < blackHoleSpawnRate) spawn_blackhole();
        }
    }

    void DrawEntity(Entity* e)
    {
        if (e->active)
        {
            Renderer::DrawTexture(e->texture, e->position, e->rotation, e->scale, e->color);
        }
    }

    void Render()
    {
        if (gameOverTimer > 0)
        {
            return;
        }

        DrawEntity(&player);

        for (int i = 0, n = seekers.count; i < n; i++)
        {
            DrawEntity(&seekers[i]);
        }

        for (int i = 0, n = wanderers.count; i < n; i++)
        {
            DrawEntity(&wanderers[i]);
        }

        for (int i = 0, n = blackHoles.count; i < n; i++)
        {
            DrawEntity(&blackHoles[i]);
        }

        for (int i = 0, n = bullets.count; i < n; i++)
        {
            DrawEntity(&bullets[i]);
        }
    }
}

namespace ParticleSystem
{
    Array<Particle> particles;
    Array<int> freeParticles;

    void init(void)
    {
        particles.Ensure(20 * 1024);
    }

    void SpawnParticle(const Texture& texture, vec2 pos, vec4 tint, float duration, vec2 scale, float theta, vec2 vel)
    {
        Particle* p = NULL;
        if (freeParticles.count > 0)
        {
            int index = freeParticles.Pop();
            p = &particles[index];
        }
        else
        {
            p = &particles[particles.NewSlot()];
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

    bool UpdateParticle(Particle& p, float dt)
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

            p.rotation = atan2f(p.velocity.y, p.velocity.x);
            p.position += p.velocity * dt;
            p.velocity *= 1.0f - 3 * dt;

            p.scale.x = 1.0f - p.timer / p.duration;
            p.color.w = 1.0f - p.timer / p.duration;

            if (p.position.x < -Window::GetWidth())
            {
                p.velocity.x = fabsf(p.velocity.x);
                p.position.x = -Window::GetWidth();
            }
            else if (p.position.x > Window::GetWidth())
            {
                p.velocity.x = -fabsf(p.velocity.x);
                p.position.x = Window::GetWidth();
            }

            if (p.position.y < -Window::GetWidth())
            {
                p.velocity.y = fabsf(p.velocity.y);
                p.position.y = -Window::GetHeight();
            }
            else if (p.position.y > Window::GetWidth())
            {
                p.velocity.y = -fabsf(p.velocity.y);
                p.position.y = Window::GetHeight();
            }

            for (int i = 0, n = World::blackHoles.count; i < n; i++)
            {
                Entity* blackhole = &World::blackHoles[i];
                if (!blackhole->active) continue;

                vec2 diff = blackhole->position - p.position;
                float d = length(diff);
                vec2 normal = normalize(diff);
                p.velocity += normal * max(0.0f, Window::GetWidth() / d);

                // add tangential acceleration for nearby particles
                if (d < 10.0f * blackhole->radius)
                {
                    p.velocity += vec2(normal.y, -normal.x) * (21.0f * blackhole->radius / (120.0f + 1.2f * d));
                }
            }
        }

        return result;
    }

    void Update(float dt)
    {
        for (int i = 0, n = particles.count; i < n; i++)
        {
            Particle& p = particles[i];
            if (UpdateParticle(p, dt))
            {
                freeParticles.Push(i);
            }
        }
    }

    void Render()
    {
        BlendFunc blend = { BlendFactor::SrcAlpha, BlendFactor::One };
        for (int i = 0, n = particles.count; i < n; i++)
        {
            Particle& p = particles[i];
            if (p.active)
            {
                Renderer::DrawTexture(p.texture, p.position, p.rotation, p.scale, p.color, blend);
            }
        }
    }
}

namespace Renderer
{
    struct Vertex
    {
        vec2 pos;
        vec2 uv;
    };

    struct DrawCommand
    {
        Texture   texture;
        int       drawCount;
        vec2      position;
        vec2      scale;
        float     rotation;
        vec4      color;

        BlendFunc blend;
    };

    Array<DrawCommand>    drawcmds;
    Array<Vertex>         vertices;
    Array<unsigned short> indices;

    VertexArray  vertexArray;
    IndexBuffer  indexBuffer;
    VertexBuffer vertexBuffer;

    Shader shader;

    //GLuint  framebuf;
    //Texture frametex;
    //GLuint  framevao;
    //GLuint  framevbo;
    //GLuint  frameprog;

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

    void Init(void)
    {
        GL::Enable(GraphicsMode::Blend);
        GL::SetBlendFunc(BlendFactor::SrcAlpha, BlendFactor::InvertSrcAlpha);

        float w = Window::GetWidth();
        float h = Window::GetHeight();
        proj_matrix = mat4::ortho(-w, w, -h, h, -10.0f, 10.0f);

        vertexArray = VertexArray::Create();
        vertexBuffer = VertexBuffer::Create();
        indexBuffer = IndexBuffer::Create();

        shader = Shader::Create(vshader_src, fshader_src);

        if (!shader._handle)
        {
            fprintf(stderr, "An error occured, press any key to exit...");
            getchar();
            exit(1);
        }

        vertexArray.SetAttribute(vertexBuffer, 0, 4, DataType::Float, false, sizeof(Vertex));

        //frametex = new Texture();
        //texture::apply(frametex);
        //glBindTexture(GL_TEXTURE_2D, frametex->handle);
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
        //glBindTexture(GL_TEXTURE_2D, 0);
        //glGenFramebuffersEXT(1, &framebuf);
        //glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuf);
        //glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, frametex->handle, 0);

        //GLenum status;
        //status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
        //switch (status)
        //{
        //case GL_FRAMEBUFFER_COMPLETE_EXT:
        //    printf("renderer::init(): Success create framebuffer.\n");
        //    break;
        //
        //default:
        //    fprintf(stderr, "Failed to create framebuffer.\n");
        //    fprintf(stderr, "An error occured, press any key to exit...");
        //    getchar();
        //    exit(1);
        //    break;
        //}
        //
        //glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

        // Create framebuffer Render object
        {
            //glGenVertexArrays(1, &framevao);
            //glGenBuffers(1, &framevbo);
            //
            //vec2 vertices[] =
            //{
            //    // First triangle
            //    vec2(-1.0f, -1.0f), vec2(0.0f, 0.0f),
            //    vec2(-1.0f,  1.0f), vec2(0.0f, 1.0f),
            //    vec2(1.0f,  1.0f), vec2(1.0f, 1.0f),
            //
            //    // Second triangle
            //    vec2(1.0f,  1.0f), vec2(1.0f, 1.0f),
            //    vec2(1.0f, -1.0f), vec2(1.0f, 0.0f),
            //    vec2(-1.0f, -1.0f), vec2(0.0f, 0.0f),
            //};
            //
            //glBindVertexArray(framevao);
            //glBindBuffer(GL_ARRAY_BUFFER, framevbo);
            //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            //glEnableVertexAttribArray(0);
            //glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), NULL);
            //glBindBuffer(GL_ARRAY_BUFFER, 0);
            //glBindVertexArray(0);
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

            //GLuint vshader = create_shader(GL_VERTEX_SHADER, vshader_src);
            //GLuint fshader = create_shader(GL_FRAGMENT_SHADER, fshader_src);
            //frameprog = create_program(vshader, fshader);
            //
            //if (!vshader || !fshader || !frameprog)
            //{
            //    fprintf(stderr, "An error occured, press any key to exit...");
            //    getchar();
            //    exit(1);
            //}
            //
            //glDeleteShader(vshader);
            //glDeleteShader(fshader);
            //printf("renderer::init(): Create framebuffer's shader successfully.\n");
        }
    }

    void DrawTexture(const Texture& texture, vec2 position, float rotation, vec2 scale, vec4 color, BlendFunc blend)
    {
        DrawCommand cmd;
        cmd.texture     = texture;
        cmd.position    = position;
        cmd.rotation    = rotation;
        cmd.scale       = scale * vec2(texture.width, texture.height);
        cmd.blend       = blend;
        cmd.color       = color;
        cmd.drawCount   = 6;
        drawcmds.Push(cmd);

        unsigned short i = (unsigned short)indices.count;
        indices.Push((unsigned short)(i + 0U));
        indices.Push((unsigned short)(i + 1U));
        indices.Push((unsigned short)(i + 2U));
        indices.Push((unsigned short)(i + 0U));
        indices.Push((unsigned short)(i + 2U));
        indices.Push((unsigned short)(i + 3U));

        Vertex v;
        v.pos = vec2(-0.5f, -0.5f);
        v.uv = vec2(0.0f, 1.0f);
        vertices.Push(v);

        v.pos = vec2(-0.5f, 0.5f);
        v.uv = vec2(0.0f, 0.0f);
        vertices.Push(v);

        v.pos = vec2(0.5f, 0.5f);
        v.uv = vec2(1.0f, 0.0f);
        vertices.Push(v);

        v.pos = vec2(0.5f, -0.5f);
        v.uv = vec2(1.0f, 1.0f);
        vertices.Push(v);
    }

    void Prepair()
    {
        drawcmds.Clear();
        vertices.Clear();
        indices.Clear();
    }

    void Present()
    {
        //glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuf);
        //glClear(GL_COLOR_BUFFER_BIT);

        vertexBuffer.SetData(vertices.elements, vertices.count * sizeof(Vertex), BufferUsage::StreamDraw);
        indexBuffer.SetData(indices.elements, indices.count * sizeof(unsigned short), DataType::Ushort, BufferUsage::StreamDraw);

        int offset = 0;
        for (int i = 0, n = drawcmds.count; i < n; i++)
        {
            const DrawCommand& cmd = drawcmds[i];

            GL::SetBlendFunc(cmd.blend);

            mat4 model_matrix = mat4::translation(cmd.position) * mat4::rotation_z(cmd.rotation) * mat4::scalation(cmd.scale);
            mat4 MVP_matrix = proj_matrix * model_matrix;

            shader.SetFloat4x4("MVP", (float*)&MVP_matrix);
            shader.SetFloat4("color", cmd.color.x, cmd.color.y, cmd.color.z, cmd.color.w);

            GL::DrawIndices(DrawType::Triangles, shader, vertexArray, cmd.texture, indexBuffer, cmd.drawCount, offset);

            offset += cmd.drawCount;
        }

        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        //glBindBuffer(GL_ARRAY_BUFFER, 0);
        //glBindVertexArray(0);
        //
        //glUseProgram(0);
        //glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
        //
        //glUseProgram(frameprog);
        //glBindVertexArray(framevao);
        //glBindBuffer(GL_ARRAY_BUFFER, framevbo);
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, frametex->handle);
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        //glBindTexture(GL_TEXTURE_2D, 0);
        //glBindBuffer(GL_ARRAY_BUFFER, 0);
        //glBindVertexArray(0);
    }
}

namespace Game
{
    vec2 aim;
    bool fire;

    float axis_vertical = 0.0f;
    float axis_horizontal = 0.0f;

    void Init()
    {
        // System
        srand(time(NULL));

        Assets::AddSearchPath("..");

        // Initialize World
        World::Init();

        // Initialize Render engine
        Renderer::Init();

        // Initialize audio
        //audio::init();

        //SDL_InitSubSystem(SDL_INIT_JOYSTICK);
        //if (SDL_NumJoysticks() > 0)
        //{
        //    SDL_Joystick* joystick = SDL_JoystickOpen(0);
        //
        //    if (joystick)
        //    {
        //        printf("Opened Joystick 0\n");
        //        printf("Name: %s\n", SDL_JoystickNameForIndex(0));
        //        printf("Number of Axes: %d\n", SDL_JoystickNumAxes(joystick));
        //        printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(joystick));
        //        printf("Number of Balls: %d\n", SDL_JoystickNumBalls(joystick));
        //    }
        //    else
        //    {
        //        printf("Couldn't open Joystick 0\n");
        //    }
        //
        //}
    }

    void Input()
    {
        const float LERP_RATE = 0.5f;

        if (Input::GetKey(KeyCode::W) || Input::GetKey(KeyCode::UpArrow))
        {
            axis_vertical = lerp(axis_vertical, 1.0f, LERP_RATE);
        }
        else if (Input::GetKey(KeyCode::S) || Input::GetKey(KeyCode::DownArrow))
        {
            axis_vertical = lerp(axis_vertical, -1.0f, LERP_RATE);
        }
        else
        {
            axis_vertical = lerp(axis_vertical, 0.0f, LERP_RATE);
        }
        
        if (Input::GetKey(KeyCode::A) || Input::GetKey(KeyCode::LeftArrow))
        {
            axis_horizontal = lerp(axis_horizontal, -1.0f, LERP_RATE);
        }
        else if (Input::GetKey(KeyCode::D) || Input::GetKey(KeyCode::RightArrow))
        {
            axis_horizontal = lerp(axis_horizontal, 1.0f, LERP_RATE);
        }
        else
        {
            axis_horizontal = lerp(axis_horizontal, 0.0f, LERP_RATE);
        }

        int mx;
        int my;
        fire = Input::GetMouseButton(MouseButton::Left);
        {
            vec2 clip = vec2(2.0f * mx / (float)Window::GetWidth() - 1.0f, 1.0f - 2.0f * my / (float)Window::GetHeight());
        
            vec2 mpos = vec2(clip.x * Window::GetWidth(), clip.y * Window::GetHeight());
        
            vec2 taim = normalize(mpos - World::player.position);
        
#if 0   
            float cur_angle = vec2_angle(aim);
            float aim_angle = vec2_angle(taim);
        
            cur_angle = step(cur_angle, aim_angle, 0.8f);
            aim = vec2(cosf(cur_angle), sinf(cur_angle));
#endif  
        
            aim = lerp(aim, taim, 0.8f);
        }

        //
        //SDL_Joystick* joystick = SDL_JoystickOpen(0);
        //if (joystick)
        //{
        //    axis_vertical = step(axis_vertical, -(SDL_JoystickGetAxis(joystick, 1)) / (float)SHRT_MAX, 0.1f);
        //    axis_horizontal = step(axis_horizontal, (SDL_JoystickGetAxis(joystick, 0)) / (float)SHRT_MAX, 0.1f);
        //
        //    float x = (SDL_JoystickGetAxis(joystick, 3)) / (float)SHRT_MAX;
        //    float y = -(SDL_JoystickGetAxis(joystick, 4)) / (float)SHRT_MAX;
        //    if (length(vec2(x, y)) < 0.01f)
        //    {
        //        aim = vec2();
        //    }
        //    else
        //    {
        //        fire = true;
        //
#if 0   //
        //        float cur_angle = vec2_angle(aim);
        //        float aim_angle = atan2f(y, x);
        //
        //        cur_angle = step(cur_angle, aim_angle, 0.8f);
        //        aim = vec2(cosf(cur_angle), sinf(cur_angle));
#endif  //
        //
        //        aim.x = step(aim.x, x, 0.6f);
        //        aim.y = step(aim.y, y, 0.6f);
        //    }
        //}
        //

        vec2 axes = vec2(axis_horizontal, axis_vertical);
        if (length(axes) < 0.01f)
        {
            axes = vec2();
        }
        else
        {
            float len = clamp(length(axes), 0, 1);
            float angle = atan2(axes.y, axes.x);

            axes = vec2(cos(angle) * len, sin(angle) * len);
        }

        axis_vertical = axes.y;
        axis_horizontal = axes.x;
    }

    void Update(float dt)
    {
        Input();

        totalTime += dt;

        World::Update(dt, axis_vertical, axis_horizontal, aim, fire);
        ParticleSystem::Update(dt);
    }

    void Render(void)
    {
        Renderer::Prepair();

        ParticleSystem::Render();
        World::Render();

        Renderer::Present();
    }
}

#if 0




namespace audio
{
    struct audio_t
    {
        ALuint source;
        ALuint buffer;
    };

    ALCdevice*  device;
    ALCcontext* context;

    table_t<const char*, audio_t> audios(membuf_heap());

    void init()
    {
        device = alcOpenDevice(NULL);
        if (!device)
        {
            fprintf(stderr, "audio::init(): Cannot open audio device '%s'\n", alcGetString(NULL, alcGetError(NULL)));
            return;
        }

        fprintf(stderr, "audio::init(): %s\n", alcGetString(device, ALC_DEVICE_SPECIFIER));

        context = alcCreateContext(device, NULL);
        if (!context)
        {
            fprintf(stderr, "audio::init(): Cannot create audio context '%s'", alcGetString(device, alcGetError(device)));
            return;
        }

        alcMakeContextCurrent(context);
        printf("Audio version: %s\n", alGetString(AL_VERSION));
        printf("Audio renderer: %s\n", alGetString(AL_RENDERER));

        alListener3f(AL_POSITION, 0, 0, 0);
        alListener3f(AL_VELOCITY, 0, 0, 0);

        ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
        alListenerfv(AL_ORIENTATION, listenerOri);
    }

    static void* LoadWAV(const char* path, ALenum* format, int* size, int* freq)
    {
    #pragma push(1)
        struct WAVHeader
        {
            uint32_t id;
            uint32_t size;
            uint32_t format;

            uint32_t subchunk1;
            uint32_t subchunk1size;

            uint16_t audioformat;
            uint16_t channels;
            uint32_t samplerate;
            uint32_t byterate;
            uint32_t blockalign;
            uint32_t bitspersample;

            uint32_t subchunk2;
            uint32_t subchunk2size;
        };
    #pragma pop

        HANDLE file = CreateFileA(
            path,
            GENERIC_READ,
            0,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );
        if (file != INVALID_HANDLE_VALUE)
        {
            WAVHeader header;
            ReadFile(file, &header, sizeof(header), NULL, NULL);

            void* result = malloc(header.size);
            ReadFile(file, result, header.size, NULL, NULL);

            *size = header.size;
            *freq = header.samplerate;

            if (header.channels == 1)
            {
                if (header.bitspersample == 8)
                {
                    *format = AL_FORMAT_MONO8;
                }
                else
                {
                    *format = AL_FORMAT_MONO16;
                }
            }
            else
            {
                if (header.bitspersample == 8)
                {
                    *format = AL_FORMAT_STEREO8;
                }
                else
                {
                    *format = AL_FORMAT_STEREO16;
                }
            }

            CloseHandle(file);
            return result;
        }

        return NULL;
    }

    bool load(const char* path, audio_t* out_audio)
    {
        audio_t audio;
        if (table::tryget(audios, path, audio))
        {
            if (out_audio)
            {
                out_audio[0] = audio;
            }
            return true;
        }

        Uint32 len;
        Uint8* wav;
        ALenum format;
        ALenum error;
        SDL_AudioSpec spec;
        if (SDL_LoadWAV(path, &spec, &wav, &len))
        {
            // Lock audio context
            alcSuspendContext(context);

            alGenSources(1, &audio.source);
            error = alGetError();
            if (error != AL_NO_ERROR)
            {
                SDL_FreeWAV(wav);
                alDeleteSources(1, &audio.source);
                alDeleteBuffers(1, &audio.buffer);
                printf("audio::play(): alGenSources: %s\n", alGetString(error));
                return false;
            }

            alGenBuffers(1, &audio.buffer);
            error = alGetError();
            if (error != AL_NO_ERROR)
            {
                SDL_FreeWAV(wav);
                alDeleteSources(1, &audio.source);
                alDeleteBuffers(1, &audio.buffer);
                printf("audio::play(): alGenBuffers: %s\n", alGetString(error));
                return false;
            }

            alSourcef(audio.source, AL_GAIN, 0.3f);
            error = alGetError();
            if (error != AL_NO_ERROR)
            {
                SDL_FreeWAV(wav);
                alDeleteSources(1, &audio.source);
                alDeleteBuffers(1, &audio.buffer);
                printf("audio::play(): set source gain failed: %s\n", alGetString(error));
                return false;
            }

            alSourcef(audio.source, AL_PITCH, 1);
            error = alGetError();
            if (error != AL_NO_ERROR)
            {
                SDL_FreeWAV(wav);
                alDeleteSources(1, &audio.source);
                alDeleteBuffers(1, &audio.buffer);
                printf("audio::play(): set source pitch failed: %s\n", alGetString(error));
                return false;
            }

            alSource3f(audio.source, AL_POSITION, 0, 0, 0);
            error = alGetError();
            if (error != AL_NO_ERROR)
            {
                SDL_FreeWAV(wav);
                alDeleteSources(1, &audio.source);
                alDeleteBuffers(1, &audio.buffer);
                printf("audio::play(): set source position failed: %s\n", alGetString(error));
                return false;
            }

            alSource3f(audio.source, AL_VELOCITY, 0, 0, 0);
            error = alGetError();
            if (error != AL_NO_ERROR)
            {
                SDL_FreeWAV(wav);
                alDeleteSources(1, &audio.source);
                alDeleteBuffers(1, &audio.buffer);
                printf("audio::play(): set source velocity failed: %s\n", alGetString(error));
                return false;
            }

            alSourcei(audio.source, AL_LOOPING, AL_FALSE);
            error = alGetError();
            if (error != AL_NO_ERROR)
            {
                SDL_FreeWAV(wav);
                alDeleteSources(1, &audio.source);
                alDeleteBuffers(1, &audio.buffer);
                printf("audio::play(): set source loop failed: %s\n", alGetString(error));
                return false;
            }

            switch (spec.format)
            {
            case AUDIO_U8:
            case AUDIO_S8:
                format = spec.channels == 2 ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8;
                break;

            case AUDIO_U16:
            case AUDIO_S16:
                format = spec.channels == 2 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
                break;
            }

            alBufferData(audio.buffer, format, wav, len, spec.freq);
            error = alGetError();
            if (error != AL_NO_ERROR)
            {
                SDL_FreeWAV(wav);
                alDeleteSources(1, &audio.source);
                alDeleteBuffers(1, &audio.buffer);
                printf("audio::play(): set buffer data failed: %s\n", alGetString(error));
                return false;
            }

            alSourcei(audio.source, AL_BUFFER, audio.buffer);
            error = alGetError();
            if (error != AL_NO_ERROR)
            {
                free(wav);
                alDeleteSources(1, &audio.source);
                alDeleteBuffers(1, &audio.buffer);
                printf("audio::play(): set buffer failed: %s\n", alGetString(error));
                return false;
            }

            // Unlock audio context
            alcProcessContext(context);

            if (out_audio)
            {
                out_audio[0] = audio;
            }
            table::set(audios, path, audio);
            
            SDL_FreeWAV(wav);
            return true;
        }
        return false;
    }

    void play(const char* path)
    {
        audio_t audio;
        if (load(path, &audio))
        {
            ALint state;
            alGetSourcei(audio.source, AL_SOURCE_STATE, &state);
            alSourcePlay(audio.source);
        }
    }

    void stop(const char* path)
    {
        audio_t audio;
        if (table::tryget(audios, path, audio))
        {                        
            ALint state;
            alGetSourcei(audio.source, AL_SOURCE_STATE, &state);
            alSourceStop(audio.source);
        }
    }

    const char* shoot_audio_paths[] =
    {
        "Audios/shoot-01.wav",
        "Audios/shoot-02.wav",
        "Audios/shoot-03.wav",
        "Audios/shoot-04.wav",
    };

    void play_shoot(void)
    {
        const int count = _countof(shoot_audio_paths);
        
        int index = rand() % count;
        play(shoot_audio_paths[index]);
    }

    void stop_shoot(void)
    {
        for (int i = 0, n = _countof(shoot_audio_paths); i < 0; i++)
        {
            stop(shoot_audio_paths[i]);
        }
    }

    const char* explosion_audio_paths[] =
    {
        "Audios/explosion-01.wav",
        "Audios/explosion-02.wav",
        "Audios/explosion-03.wav",
        "Audios/explosion-04.wav",
        "Audios/explosion-05.wav",
        "Audios/explosion-06.wav",
        "Audios/explosion-07.wav",
        "Audios/explosion-08.wav",
    };

    void play_explosion(void)
    {
        const int count = _countof(explosion_audio_paths);

        int index = rand() % count;
        play(explosion_audio_paths[index]);
    }

    void stop_explosion(void)
    {
        for (int i = 0, n = _countof(explosion_audio_paths); i < 0; i++)
        {
            stop(explosion_audio_paths[i]);
        }
    }

    const char* spawn_audio_paths[] =
    {
        "Audios/spawn-01.wav",
        "Audios/spawn-02.wav",
        "Audios/spawn-03.wav",
        "Audios/spawn-04.wav",
        "Audios/spawn-05.wav",
        "Audios/spawn-06.wav",
        "Audios/spawn-07.wav",
        "Audios/spawn-08.wav",
    };

    void play_spawn(void)
    {
        const int count = _countof(spawn_audio_paths);

        int index = rand() % count;
        play(spawn_audio_paths[index]);
    }

    void stop_spawn(void)
    {
        for (int i = 0, n = _countof(spawn_audio_paths); i < 0; i++)
        {
            stop(spawn_audio_paths[i]);
        }
    }
}

namespace Game
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
        Game::window = window;
        SDL_GetWindowSize(window, &screen_width, &screen_height);

        // Initialize World
        World::init(window);

        // Initialize Render engine
		renderer::init(window);

        // Initialize audio
        audio::init();

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
            
            vec2 taim = normalize(mpos - World::player->position);

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

    void Update(float dt)
    {
        totalTime += dt;

        World::Update(dt, axis_vertical, axis_horizontal, aim, fire);
        ParticleSystem::Update(dt);
    }

    void Render(void)
    {
		renderer::Prepair();

        ParticleSystem::Render();
        World::Render();

		renderer::Present();
    }
}
#endif