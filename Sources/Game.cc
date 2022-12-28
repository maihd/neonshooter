#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <Mojo/Math.h>
#include <Mojo/Audio.h>
#include <Mojo/Assets.h>

#include <Mojo/Native/Input.h>
#include <Mojo/Native/Window.h>

#include <Mojo/JobSystem.h>
#include <Mojo/JobCounter.h>
#include <Mojo/FileSystem.h>

#include <Mojo/Core/Array.h>
#include <Mojo/Core/HashTable.h>

#include <Mojo/Graphics.h>
#include <Mojo/Graphics/Shader.h>
#include <Mojo/Graphics/Texture.h>
#include <Mojo/Graphics/SpriteBatch.h>

#define PI 3.14f

struct Entity
{
public:
    bool        active;

    Vector2     scale;
    Vector2     position;
    float       rotation;

    Vector2     velocity;
    float       movespeed;

    Vector4     color;
    float       radius;

    Texture     texture;

    void Update(float dt)
    {
        position += velocity * movespeed * dt;
        if (velocity.x != 0.0f || velocity.y != 0.0f)
        {
            rotation = Math::Atan2(velocity.y, velocity.x);
        }
    }

    void Update(Vector2 bounds, float dt)
    {
        Vector2  pos = position + velocity * movespeed * dt;
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
            rotation = Math::Atan2(velocity.y, velocity.x);
        }
    }
};

struct Particle
{
    bool        active;
    Texture     texture;
    Vector2     velocity;
    Vector2     position;
    float       rotation;
    Vector2     scale;
    Vector4     color;
    float       timer;
    float       duration;
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
        texture = Texture::Create();

        texture.SetWrap(TextureWrap::Repeat);
        texture.SetFilter(TextureFilter::Linear);
        texture.SetPixels(w, h, n == 4 ? PixelFormat::RGBA : PixelFormat::RGB, pixels);

        Assets::FreeImage(pixels);
        _textures.SetValue(hash, texture);
    }

    return texture; 
}

namespace ColorUtils
{
    Vector4 HSV(float h, float s, float v)
    {
        if (h == 0 && s == 0)
        {
            return Vector4(v, v, v, 1.0f);
        }

        float c = s * v;
        float x = c * (1 - Math::Abs(Math::Mod(h, 2) - 1));
        float m = v - c;

        if (h < 1)      return Vector4(c + m, x + m, m, 1.0f);
        else if (h < 2) return Vector4(x + m, c + m, m, 1.0f);
        else if (h < 3) return Vector4(m, c + m, x + m, 1.0f);
        else if (h < 4) return Vector4(m, x + m, c + m, 1.0f);
        else if (h < 5) return Vector4(x + m, m, c + m, 1.0f);
        else            return Vector4(c + m, m, x + m, 1.0f);
    }
}

namespace Game
{
    static int   _currentFrame = 0;
    static float totalTime;
}

namespace GameAudio
{
    void Init();

    void PlayShoot(void);
    void StopShoot(void);

    void PlayExplosion(void);
    void StopExplosion(void);

    void PlaySpawn(void);
    void StopSpawn(void);
}

namespace ParticleSystem
{
    void SpawnParticle(const Texture& texture, Vector2 pos, Vector4 tint, float duration, Vector2 scale, float theta, Vector2 vel);
}

namespace Renderer
{
    void DrawTexture(
        const Texture& texture, 
        Vector2 position = Vector2(0.0f), 
        float rotation = 0.0f, 
        Vector2 scale = Vector2(1.0f), 
        Vector4 color = Vector4(1.0f), 
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

    bool oldFire = false;
    float fireTimer = 0.0f;
    float fireInterval = 0.1f;

    float spawnTimer = 0.0f;
    float spawnInterval = 1.0f;

    bool  lock = false;
    float gameOverTimer = 0.0f;

    void Init(void)
    {
        player.active    = true;
        player.color     = Vector4(1.0f);
        player.position  = Vector2(0.0f);
        player.rotation  = 0.0f;
        player.scale     = Vector2(1.0f);
        player.movespeed = 720.0f;
        player.texture   = LoadTexture("Art/Player.png");
        player.radius    = player.texture.width * 0.5f;

        bullets.Ensure(256);
        seekers.Ensure(256);
        wanderers.Ensure(256);
    }

    void SpawnBullet(Vector2 pos, Vector2 vel)
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
        en->color       = Vector4(1.0f);
        en->position    = pos;
        en->rotation    = Math::Atan2(vel.y, vel.x);
        en->scale       = Vector2(1.0f);
        en->texture     = LoadTexture("Art/Bullet.png");
        en->velocity    = vel;
        en->movespeed   = 1280.0f;
        en->radius      = en->texture.height * 0.5f;
    }

    void FireBullets(Vector2 aim_dir)
    {
        float angle = Math::Atan2(aim_dir.y, aim_dir.x) + (rand() % 101) / 100.0f * (PI * 0.025f);
        float offset = PI * 0.1f;

        aim_dir = Vector2(Math::Cos(angle), Math::Sin(angle));

        // First bullet
        {
            Vector2 vel = Math::Normalize(aim_dir);
            Vector2 pos = player.position + Vector2(Math::Cos(angle + offset), Math::Sin(angle + offset)) * player.texture.width * 1.25f;
            SpawnBullet(pos, vel);
        }

        // Second bullet
        {
            Vector2 vel = Math::Normalize(aim_dir);
            Vector2 pos = player.position + Vector2(Math::Cos(angle - offset), Math::Sin(angle - offset)) * player.texture.width * 1.25f;
            SpawnBullet(pos, vel);
        }
    }

    Vector2 GetSpawnPosition()
    {
        const float minDistanceSq = (Window::GetHeight() * 0.3f) * (Window::GetHeight() * 0.3f);

        Vector2 pos;

        do
        {
            float x = (2.0f * (rand() % 101) / 100.0f - 1.0f) * 0.8f * Window::GetWidth();
            float y = (2.0f * (rand() % 101) / 100.0f - 1.0f) * 0.8f * Window::GetHeight();
            pos = Vector2(x, y);
        } while (Math::DistanceSq(pos, player.position) < minDistanceSq);

        return pos;
    }

    void SpawnSeeker()
    {
        GameAudio::PlaySpawn();

        Vector2 pos = GetSpawnPosition();

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
        en->color       = Vector4(1.0f, 1.0f, 1.0f, 0.0f);
        en->velocity    = Math::Normalize(player.position - pos);
        en->position    = pos;
        en->movespeed   = 360.0f;
        en->scale       = Vector2(1.0f);
        en->texture     = LoadTexture("Art/Seeker.png");
        en->rotation    = Math::Atan2(en->velocity.y, en->velocity.x);
        en->radius      = en->texture.width * 0.5f;
    }

    void SpawnWanderer()
    {
        GameAudio::PlaySpawn();

        Vector2 pos = GetSpawnPosition();

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
        en->color       = Vector4(1.0f, 1.0f, 1.0f, 0.0f);
        en->velocity    = Math::Normalize(player.position - pos);
        en->position    = pos;
        en->movespeed   = 240.0f;
        en->scale       = Vector2(1.0f);
        en->texture     = LoadTexture("Art/Wanderer.png");
        en->rotation    = Math::Atan2(en->velocity.y, en->velocity.x);
        en->radius      = en->texture.width * 0.5f;
    }

    void SpawnBlackhole()
    {
        GameAudio::PlaySpawn();

        Vector2 pos = GetSpawnPosition();

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
        en->color       = Vector4(1.0f, 1.0f, 1.0f, 0.0f);
        en->velocity    = Vector2(0.0f);
        en->position    = pos;
        en->movespeed   = 0;
        en->scale       = Vector2(1.0f);
        en->texture     = LoadTexture("Art/Black Hole.png");
        en->rotation    = 0;
        en->radius      = en->texture.width * 0.5f;
    }

    void DestroyBullet(Entity* bullet, int index, bool explosion = false)
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
                Vector2 vel = Vector2(Math::Cos(angle) * speed, Math::Sin(angle) *speed);

                Vector4 color = Vector4(0.6f, 1.0f, 1.0f, 1.0f);
                ParticleSystem::SpawnParticle(texture, bullet->position, color, 1.0f, Vector2(1.0f), 0.0f, vel);
            }
        }
    }

    void DestroySeeker(Entity* seeker, int index)
    {
        GameAudio::PlayExplosion();

        seeker->active = false;
        freeSeekers.Push(index);

        Texture texture = LoadTexture("Art/Laser.png");

        float hue1 = rand() % 101 / 100.0f * 6.0f;
        float hue2 = Math::Mod(hue1 + (rand() % 101 / 100.0f * 2.0f), 6.0f);
        Vector4 color1 = ColorUtils::HSV(hue1, 0.5f, 1);
        Vector4 color2 = ColorUtils::HSV(hue2, 0.5f, 1);

        for (int i = 0; i < 120; i++)
        {
            float speed = 640.0f * (0.2f + (rand() % 101 / 100.0f) * 0.8f);
            float angle = rand() % 101 / 100.0f * 2 * PI;
            Vector2 vel = Vector2(Math::Cos(angle) * speed, Math::Sin(angle) * speed);

            Vector4 color = color1 + (color2 - color1) * ((rand() % 101) / 100.0f);
            ParticleSystem::SpawnParticle(texture, seeker->position, color, 1.0f, Vector2(1.0f), 0.0f, vel);
        }
    }

    void DestroyWanderer(Entity* wanderer, int index)
    {
        GameAudio::PlayExplosion();

        wanderer->active = false;
        freeWanderers.Push(index);

        Texture texture = LoadTexture("Art/Laser.png");

        float hue1 = rand() % 101 / 100.0f * 6.0f;
        float hue2 = Math::Mod(hue1 + (rand() % 101 / 100.0f * 2.0f), 6.0f);
        Vector4 color1 = ColorUtils::HSV(hue1, 0.5f, 1);
        Vector4 color2 = ColorUtils::HSV(hue2, 0.5f, 1);

        for (int i = 0; i < 120; i++)
        {
            float speed = 640.0f * (0.2f + (rand() % 101 / 100.0f) * 0.8f);
            float angle = rand() % 101 / 100.0f * 2 * PI;
            Vector2 vel = Vector2(Math::Cos(angle) * speed, Math::Sin(angle) * speed);

            Vector4 color = color1 + (color2 - color1) * ((rand() % 101) / 100.0f);
            ParticleSystem::SpawnParticle(texture, wanderer->position, color, 1.0f, Vector2(1.0f), 0.0f, vel);
        }
    }

    void DestroyBlackhole(Entity* blaclhole, int index)
    {
        GameAudio::PlayExplosion();

        blaclhole->active = false;
        freeBlackHoles.Push(index);

        Texture texture = LoadTexture("Art/Laser.png");

        float hue1 = rand() % 101 / 100.0f * 6.0f;
        float hue2 = Math::Mod(hue1 + (rand() % 101 / 100.0f * 2.0f), 6.0f);
        Vector4 color1 = ColorUtils::HSV(hue1, 0.5f, 1);
        Vector4 color2 = ColorUtils::HSV(hue2, 0.5f, 1);

        for (int i = 0; i < 120; i++)
        {
            float speed = 640.0f * (0.2f + (rand() % 101 / 100.0f) * 0.8f);
            float angle = rand() % 101 / 100.0f * 2 * PI;
            Vector2 vel = Vector2(Math::Cos(angle) * speed, Math::Sin(angle) * speed);

            Vector4 color = color1 + (color2 - color1) * ((rand() % 101) / 100.0f);
            ParticleSystem::SpawnParticle(texture, blaclhole->position, color, 1.0f, Vector2(1.0f), 0.0f, vel);
        }
    }

    void OnGameOver()
    {
        GameAudio::PlayExplosion();

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
        float hue2 = Math::Mod(hue1 + (rand() % 101 / 100.0f * 2.0f), 6.0f);
        Vector4 color1 = ColorUtils::HSV(hue1, 0.5f, 1);
        Vector4 color2 = ColorUtils::HSV(hue2, 0.5f, 1);

        for (int i = 0; i < 1200; i++)
        {
            float speed = 10.0f * Math::Max(Window::GetWidth(), Window::GetHeight()) * (0.6f + (rand() % 101 / 100.0f) * 0.4f);
            float angle = rand() % 101 / 100.0f * 2 * PI;
            Vector2 vel = Vector2(Math::Cos(angle) * speed, Math::Sin(angle) * speed);

            Vector4 color = color1 + (color2 - color1) * ((rand() % 101) / 100.0f);
            ParticleSystem::SpawnParticle(texture, player.position, color, gameOverTimer, Vector2(1.0f), 0.0f, vel);
        }

        player.position = Vector2();
        player.velocity = Vector2();
        player.rotation = 0.0f;
    }

    bool UpdateBlackhole(Entity* blackhole, Entity* other)
    {
        if (Math::Distance(other->position, blackhole->position) <= other->radius + blackhole->radius)
        {
            return true;
        }
        else if (Math::Distance(other->position, blackhole->position) <= other->radius + blackhole->radius * 10.0f)
        {
            Vector2 diff = blackhole->position - other->position;
            other->velocity += Math::Normalize(diff) * Math::Lerp(1, 0, Math::Length(diff) / (Window::GetWidth() * 0.2f));
            other->velocity = Math::Normalize(other->velocity);
        }

        return false;
    }

    void Update(float dt, float vertical, float horizontal, Vector2 aim_dir, bool fire)
    {
        if (gameOverTimer > 0.0f)
        {
            gameOverTimer -= dt;
            return;
        }

        // Update is in progress, locking the list
        lock = true;

        player.velocity = Math::Lerp(player.velocity, Math::Normalize(Vector2(horizontal, vertical)), 5.0f * dt);
        player.Update(Vector2(Window::GetWidth(), Window::GetHeight()), dt);
        if (Math::LengthSq(player.velocity) > 0.1f && Math::Mod(Game::totalTime, 0.025f) <= 0.01f)
        {
            float speed;
            float angle = Math::Atan2(player.velocity.y, player.velocity.x);

            Texture glow_tex = LoadTexture("Art/Laser.png");
            Texture line_tex = LoadTexture("Art/Laser.png");

            Vector2 vel = -0.25f * player.movespeed * player.velocity;
            Vector2 pos = player.position + 45.0f * (-player.velocity);
            Vector2 nvel = Vector2(vel.y, -vel.x) * 0.9f * Math::Sin(Game::totalTime * 10.0f);
            float alpha = 0.7f;

            Vector2 mid_vel = vel;
            ParticleSystem::SpawnParticle(glow_tex, pos, Vector4(1.0f, 0.7f, 0.1f, 1.0f) * alpha, 0.4f, Vector2(3.0f, 2.0f), angle, mid_vel);
            ParticleSystem::SpawnParticle(line_tex, pos, Vector4(1.0f, 1.0f, 1.0f, 1.0f) * alpha, 0.4f, Vector2(3.0f, 1.0f), angle, mid_vel);

            speed = rand() % 101 / 100.0f * 40.0f;
            angle = rand() % 101 / 100.0f * 2.0f * PI;
            Vector2 side_vel1 = vel + nvel + Vector2(Math::Cos(angle), Math::Sin(angle)) * speed;
            ParticleSystem::SpawnParticle(glow_tex, pos, Vector4(0.8f, 0.2f, 0.1f, 1.0f) * alpha, 0.4f, Vector2(3.0f, 2.0f), angle, side_vel1);
            ParticleSystem::SpawnParticle(line_tex, pos, Vector4(1.0f, 1.0f, 1.0f, 1.0f) * alpha, 0.4f, Vector2(3.0f, 1.0f), angle, side_vel1);

            speed = rand() % 101 / 100.0f * 40.0f;
            angle = rand() % 101 / 100.0f * 2.0f * PI;
            Vector2 side_vel2 = vel - nvel + Vector2(Math::Cos(angle), Math::Sin(angle)) * speed;
            ParticleSystem::SpawnParticle(glow_tex, pos, Vector4(0.8f, 0.2f, 0.1f, 1.0f) * alpha, 0.4f, Vector2(3.0f, 2.0f), angle, side_vel2);
            ParticleSystem::SpawnParticle(line_tex, pos, Vector4(1.0f, 1.0f, 1.0f, 1.0f) * alpha, 0.4f, Vector2(3.0f, 1.0f), angle, side_vel2);
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
                    DestroyBullet(b, i, true);
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
                    s->velocity = Math::Normalize(s->velocity + Math::Normalize(player.position - s->position) * 10.0f * dt);
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

                    float direction = Math::Atan2(s->velocity.y, s->velocity.x);
                    for (int j = 0; j < INTERPOLATIONS; j++)
                    {
                        direction += (0.12f * (rand() % 101 / 100.0f) - 0.06f) * PI;

                        if (s->position.x < -Window::GetWidth() || s->position.x > Window::GetWidth()
                            || s->position.y < -Window::GetHeight() || s->position.y > Window::GetHeight())
                        {
                            direction = Math::Atan2(-s->position.y, -s->position.x) + (1.0f * (rand() % 101 / 100.0f) - 0.5f) * PI;
                        }

                        s->rotation = direction;
                        s->velocity = Vector2(Math::Cos(direction), Math::Sin(direction));
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

                if (Math::Distance(b->position, s->position) <= b->radius + s->radius)
                {
                    DestroyBullet(b, i);
                    DestroySeeker(s, j);
                    break;
                }
            }

            if (!b->active) continue;
            for (int j = 0, m = wanderers.count; j < m; j++)
            {
                Entity* s = &wanderers[j];
                if (!s->active || s->color.w < 1.0f) continue;

                if (Math::Distance(b->position, s->position) <= b->radius + s->radius)
                {
                    DestroyBullet(b, i);
                    DestroyWanderer(s, j);
                    break;
                }
            }

            if (!b->active) continue;
            for (int j = 0, m = blackHoles.count; j < m; j++)
            {
                Entity* s = &blackHoles[j];
                if (!s->active || s->color.w < 1.0f) continue;

                float d = Math::Distance(b->position, s->position);
                if (d <= b->radius + s->radius)
                {
                    DestroyBullet(b, i);
                    DestroyBlackhole(s, j);
                    break;
                }
                else if (d <= b->radius + s->radius * 5.0f)
                {
                    float r = b->radius + s->radius * 5.0f;
                    float t = (d - r) / r;
                    b->velocity = Math::Normalize(b->velocity + Math::Normalize(b->position - s->position) * 0.3f);
                }
            }
        }

        for (int j = 0, m = seekers.count; j < m; j++)
        {
            Entity* s = &seekers[j];
            if (!s->active || s->color.w < 1.0f) continue;

            if (Math::Distance(player.position, s->position) <= player.radius + s->radius)
            {
                OnGameOver();
                break;
            }
        }

        for (int j = 0, m = wanderers.count; j < m; j++)
        {
            Entity* s = &wanderers[j];
            if (!s->active || s->color.w < 1.0f) continue;

            if (Math::Distance(player.position, s->position) <= player.radius + s->radius)
            {
                OnGameOver();
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

                Vector4 color1 = Vector4(0.3f, 0.8f, 0.4f, 1.0f);
                Vector4 color2 = Vector4(0.5f, 1.0f, 0.7f, 1.0f);

                if (Game::_currentFrame % 3 == 0)
                {
                    float speed = 16.0f * s->radius * (0.8f + (rand() % 101 / 100.0f) * 0.2f);
                    float angle = rand() % 101 / 100.0f * Game::totalTime;
                    Vector2 vel = Vector2(Math::Cos(angle) * speed, Math::Sin(angle) * speed);
                    Vector2 pos = s->position + 0.4f * Vector2(vel.y, -vel.x) + (4.0f + rand() % 101 / 100.0f * 4.0f);

                    Vector4 color = color1 + (color2 - color1) * ((rand() % 101) / 100.0f);
                    ParticleSystem::SpawnParticle(glow_tex, pos, color, 4.0f, Vector2(0.3f, 0.2f), 0.0f, vel);
                    ParticleSystem::SpawnParticle(line_tex, pos, color, 4.0f, Vector2(1.0f, 1.0f), 0.0f, vel);
                }

                if (Game::_currentFrame % 60 == 0)
                {
                    Texture texture = LoadTexture("Art/Laser.png");

                    float hue1 = rand() % 101 / 100.0f * 6.0f;
                    float hue2 = Math::Mod(hue1 + (rand() % 101 / 100.0f * 2.0f), 6.0f);
                    Vector4 color1 = ColorUtils::HSV(hue1, 0.5f, 1);
                    Vector4 color2 = ColorUtils::HSV(hue2, 0.5f, 1);

                    for (int i = 0; i < 120.0f; i++)
                    {
                        float speed = 180.0f;
                        float angle = rand() % 101 / 100.0f * 2 * PI;
                        Vector2 vel = Vector2(Math::Cos(angle) * speed, Math::Sin(angle) * speed);
                        Vector2 pos = s->position + vel;

                        Vector4 color = color1 + (color2 - color1) * ((rand() % 101) / 100.0f);
                        ParticleSystem::SpawnParticle(texture, pos, color, 2.0f, Vector2(1.0f), 0.0f, Vector2(0.0f));
                    }
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
                        OnGameOver();
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
                            DestroyWanderer(other, j);
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
            oldFire = false;
            fireTimer = 0.0f;
            //GameAudio::StopShoot();
        }
        else
        {
            if (oldFire != fire)
            {
                oldFire = fire;
                fireTimer = fireInterval;
            }

            fireTimer += dt;
            if (fireTimer >= fireInterval)
            {
                fireTimer = 0;
                FireBullets(aim_dir);
                GameAudio::PlayShoot();
            }
        }

        // Spawn enemies
        spawnTimer += dt;
        if (spawnTimer >= spawnInterval)
        {
            spawnTimer -= spawnInterval;

            if (rand() % 101 < seekerSpawnRate) SpawnSeeker();
            if (rand() % 101 < wandererSpawnRate) SpawnWanderer();
            if (rand() % 101 < blackHoleSpawnRate) SpawnBlackhole();
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

    void SpawnParticle(const Texture& texture, Vector2 pos, Vector4 tint, float duration, Vector2 scale, float theta, Vector2 vel)
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

            p.rotation = Math::Angle(p.velocity);
            p.position += p.velocity * dt;
            p.velocity *= 1.0f - 3 * dt;

            p.scale.x = 1.0f - p.timer / p.duration;
            p.color.w = 1.0f - p.timer / p.duration;

            if (p.position.x <= -Window::GetWidth())
            {
                p.velocity.x = Math::Abs(p.velocity.x);
                p.position.x = -Window::GetWidth();
            }
            else if (p.position.x >= Window::GetWidth())
            {
                p.velocity.x = -Math::Abs(p.velocity.x);
                p.position.x = Window::GetWidth();
            }

            if (p.position.y <= -Window::GetHeight())
            {
                p.velocity.y = Math::Abs(p.velocity.y);
                p.position.y = -Window::GetHeight();
            }
            else if (p.position.y >= Window::GetHeight())
            {
                p.velocity.y = -Math::Abs(p.velocity.y);
                p.position.y = Window::GetHeight();
            }

            for (int i = 0, n = World::blackHoles.count; i < n; i++)
            {
                Entity* blackhole = &World::blackHoles[i];
                if (!blackhole->active) continue;

                Vector2 diff = blackhole->position - p.position;
                float d = Math::Length(diff);
                Vector2 normal = Math::Normalize(diff);
                p.velocity += normal * Math::Max(0.0f, Window::GetWidth() / d);

                // add tangential acceleration for nearby particles
                if (d < 10.0f * blackhole->radius)
                {
                    p.velocity += Vector2(normal.y, -normal.x) * (21.0f * blackhole->radius / (120.0f + 1.2f * d));
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
        BlendFunc blend = BlendModes::Additive;
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
    SpriteBatch* _spriteBatch;

    Shader _glowShader;
    Shader _fxaaShader;

    RenderTarget _glowRenderTarget;
    RenderTarget _fxaaRenderTarget;

    Matrix4 proj_matrix;

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
        Graphics::SetBlendFunc(BlendFactor::SrcAlpha, BlendFactor::InvertSrcAlpha);

        float w = Window::GetWidth();
        float h = Window::GetHeight();
        proj_matrix = Math::Ortho(-w, w, -h, h, -10.0f, 10.0f);
        Graphics::Viewport(0, 0, w, h);

        _glowRenderTarget = RenderTarget::Create(w, h);
        _fxaaRenderTarget = RenderTarget::Create(w, h);

        _spriteBatch = new SpriteBatch();

        // Create post processing _spriteShader
        printf("Renderer::Init(): Starting create framebuffer's shader...\n");
        {
            const char* fshader_src =
                "#version 330 core\n"
                "in vec2 VertexTexCoord;"
                "out vec4 fragColor;"
                "uniform sampler2D image;"
                "void main() {"
                "float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);"
                "vec2 texSize = textureSize(image, 0);"
                "float size = max(texSize.x, texSize.y);"
                "vec2 texOffset = 1.0 / vec2(size, size);"
                "vec3 result = texture(image, VertexTexCoord).rgb * weight[0];"
                "for(int i = 1; i < 5; ++i) {"
                "result += texture(image, VertexTexCoord + vec2(texOffset.x * i, 0.0)).rgb * weight[i];"
                "result += texture(image, VertexTexCoord - vec2(texOffset.x * i, 0.0)).rgb * weight[i];"
                "result += texture(image, VertexTexCoord + vec2(0.0, texOffset.y * i)).rgb * weight[i];"
                "result += texture(image, VertexTexCoord - vec2(0.0, texOffset.y * i)).rgb * weight[i];"
                "}"
                "result = texture(image, VertexTexCoord).rgb + result;"
                "result = vec3(1.0) - exp(-result * 0.5f);"
                "result = pow(result, vec3(1.0 / 2.2));"
                "fragColor = vec4(result, 1.0);"
                "}";
            _glowShader = Shader::CreateEffect(fshader_src);
        }

        // FXAA shader
        {
            const char* shaderSource =
                "#version 330 core\n"
                "in vec2 VertexTexCoord;"
                "out vec4 fragColor;"
                "uniform vec2 frameSize;"
                "uniform sampler2D image;"

                "void main() {"

                "float FXAA_SPAN_MAX = 8.0;"
                "float FXAA_REDUCE_MUL = 1.0 / 8.0;"
                "float FXAA_REDUCE_MIN = 1.0 / 128.0;"

                "vec3 rgbNW = texture2D(image, VertexTexCoord + (vec2(-1.0, -1.0) / frameSize)).xyz;"
                "vec3 rgbNE = texture2D(image, VertexTexCoord + (vec2(1.0, -1.0) / frameSize)).xyz;"
                "vec3 rgbSW = texture2D(image, VertexTexCoord + (vec2(-1.0, 1.0) / frameSize)).xyz;"
                "vec3 rgbSE = texture2D(image, VertexTexCoord + (vec2(1.0, 1.0) / frameSize)).xyz;"
                "vec3 rgbM = texture2D(image, VertexTexCoord).xyz;"

                "vec3 luma = vec3(0.299, 0.587, 0.114);"
                "float lumaNW = dot(rgbNW, luma);"
                "float lumaNE = dot(rgbNE, luma);"
                "float lumaSW = dot(rgbSW, luma);"
                "float lumaSE = dot(rgbSE, luma);"
                "float lumaM = dot(rgbM, luma);"

                "float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));"
                "float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));"
                
                "vec2 dir;"
                "dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));"
                "dir.y = ((lumaNW + lumaSW) - (lumaNE + lumaSE));"
                
                "float dirReduce = max("
                "    (lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAA_REDUCE_MUL),"
                "    FXAA_REDUCE_MIN);"
                
                "float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);"
                
                "dir = min(vec2(FXAA_SPAN_MAX, FXAA_SPAN_MAX),"
                "    max(vec2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX),"
                "        dir * rcpDirMin)) / frameSize;"
                
                "vec3 rgbA = (1.0 / 2.0) * ("
                "    texture2D(image, VertexTexCoord.xy + dir * (1.0 / 3.0 - 0.5)).xyz +"
                "    texture2D(image, VertexTexCoord.xy + dir * (2.0 / 3.0 - 0.5)).xyz);"
                "vec3 rgbB = rgbA * (1.0 / 2.0) + (1.0 / 4.0) * ("
                "    texture2D(image, VertexTexCoord.xy + dir * (0.0 / 3.0 - 0.5)).xyz +"
                "    texture2D(image, VertexTexCoord.xy + dir * (3.0 / 3.0 - 0.5)).xyz);"
                "float lumaB = dot(rgbB, luma);"
                
                "if ((lumaB < lumaMin) || (lumaB > lumaMax)) {"
                "    fragColor = vec4(rgbA, 1.0);"
                "}"
                "else {"
                "    fragColor = vec4(rgbB, 1.0);"
                "}"

                // End main()
                "}";

            _fxaaShader = Shader::CreateEffect(shaderSource);
            _fxaaShader.SetFloat2("frameSize", w, h);
        }
    }

    void DrawTexture(const Texture& texture, Vector2 position, float rotation, Vector2 scale, Vector4 color, BlendFunc blend)
    {
        _spriteBatch->DrawTexture(texture, position, rotation, scale, color, blend);
    }

    void Prepair()
    {
        _spriteBatch->Clear();
    }

    void Present()
    {
        Graphics::BindRenderTarget(_fxaaRenderTarget);
        Graphics::Clear();

        _spriteBatch->Present(proj_matrix);

        //Graphics::BlitRenderTarget(&_fxaaRenderTarget, &_glowRenderTarget, _fxaaShader);
        //Graphics::BlitRenderTarget(&_glowRenderTarget, &_fxaaRenderTarget, _fxaaShader);
        //Graphics::BlitRenderTarget(&_glowRenderTarget, NULL, _glowShader);
        Graphics::BlitRenderTarget(_fxaaRenderTarget, _glowShader);
    }
}

namespace Game
{
    Vector2 aimDirection;
    bool fire;

    float axisVertical = 0.0f;
    float axisHorizontal = 0.0f;

    void Init()
    {
        Assets::AddSearchPath("..");

        // Initialize World
        World::Init();

        // Initialize Render engine
        Renderer::Init();

        // Initialize audio
        GameAudio::Init();
    }

    void Input()
    {
        const float LERP_RATE = 0.5f;

        if (Input::GetKey(KeyCode::W) || Input::GetKey(KeyCode::UpArrow))
        {
            axisVertical = Math::Lerp(axisVertical, 1.0f, LERP_RATE);
        }
        else if (Input::GetKey(KeyCode::S) || Input::GetKey(KeyCode::DownArrow))
        {
            axisVertical = Math::Lerp(axisVertical, -1.0f, LERP_RATE);
        }
        else
        {
            axisVertical = Math::Lerp(axisVertical, 0.0f, LERP_RATE);
        }
        
        if (Input::GetKey(KeyCode::A) || Input::GetKey(KeyCode::LeftArrow))
        {
            axisHorizontal = Math::Lerp(axisHorizontal, -1.0f, LERP_RATE);
        }
        else if (Input::GetKey(KeyCode::D) || Input::GetKey(KeyCode::RightArrow))
        {
            axisHorizontal = Math::Lerp(axisHorizontal, 1.0f, LERP_RATE);
        }
        else
        {
            axisHorizontal = Math::Lerp(axisHorizontal, 0.0f, LERP_RATE);
        }

        float mx;
        float my;
        Input::GetMouseState(&mx, &my);
        fire = Input::GetMouseButton(MouseButton::Left);
        {
            Vector2 clip = Vector2(2.0f * mx / Window::GetWidth() - 1.0f, 1.0f - 2.0f * my / Window::GetHeight());
        
            Vector2 mpos = Vector2(clip.x * Window::GetWidth(), clip.y * Window::GetHeight());
        
            Vector2 taim = Math::Normalize(mpos - World::player.position);
        
            aimDirection = Math::Lerp(aimDirection, taim, 0.8f);
        }

        if (Input::IsGamepadAttached(0))
        {
            axisVertical = Math::Lerp(axisVertical, Input::GetAxis(0, GamepadAxis::LeftVertical), LERP_RATE);
            axisHorizontal = Math::Lerp(axisHorizontal, Input::GetAxis(0, GamepadAxis::LeftHorizontal), LERP_RATE);

            float x = Input::GetAxis(0, GamepadAxis::RightHorizontal);
            float y = Input::GetAxis(0, GamepadAxis::RightVertical);
            if (Math::Length(Vector2(x, y)) < 0.01f)
            {
                aimDirection = Vector2();
            }
            else
            {
                fire = true;


                float curAngle = Math::Atan2(aimDirection.y, aimDirection.x);
                float aimAngle = Math::Atan2(y, x);

                curAngle = Math::Lerp(curAngle, aimAngle, 0.8f);
                aimDirection = Vector2(Math::Cos(curAngle), Math::Sin(curAngle));


                aimDirection.x = Math::Lerp(aimDirection.x, x, 0.6f);
                aimDirection.y = Math::Lerp(aimDirection.y, y, 0.6f);
            }
        }

        Vector2 axes = Vector2(axisHorizontal, axisVertical);
        if (Math::Length(axes) < 0.01f)
        {
            axes = Vector2();
        }
        else
        {
            float len = Math::Clamp(Math::Length(axes), 0.0f, 1.0f);
            float angle = Math::Atan2(axes.y, axes.x);

            axes = Vector2(Math::Cos(angle) * len, Math::Sin(angle) * len);
        }

        axisVertical = axes.y;
        axisHorizontal = axes.x;
    }

    void Update(float dt)
    {
        _currentFrame++;

        Input();

        totalTime += dt;

        World::Update(dt, axisVertical, axisHorizontal, aimDirection, fire);
        ParticleSystem::Update(dt);
    }

    void Render(void)
    {
        Renderer::Prepair();

        World::Render();
        ParticleSystem::Render();

        Renderer::Present();
    }
}

namespace GameAudio
{
    struct AudioDefine
    {
        AudioSource source;
        AudioBuffer buffer;
    };

    HashTable<AudioDefine> _audios;

    void Init()
    {
        if (!Audio::Setup())
        {
            exit(1);
        }
    }

    bool Load(const char* path, AudioDefine* outAudio)
    {
        AudioDefine audio;
        if (_audios.TryGetValue(HashString(path), &audio))
        {
            if (outAudio)
            {
                outAudio[0] = audio;
            }
            return true;
        }

        void* wav;
        int   len;
        int   freq;
        int   format;
        if (wav = Assets::LoadWave(path, &len, &freq, &format))
        {
            AudioFormat audioFormat = AudioFormat::Mono8;
            switch (format)
            {
            case 0:
                audioFormat = AudioFormat::Mono8;
                break;

            case 1:
                audioFormat = AudioFormat::Mono16;
                break;

            case 2:
                audioFormat = AudioFormat::Stereo8;
                break;

            case 3:
                audioFormat = AudioFormat::Stereo16;
                break;
            }

            audio.source = AudioSource::Create();
            audio.source.SetGain(0.3f);
            audio.source.SetPitch(1.0f);
            audio.source.SetLooping(false);
            audio.source.SetPosition(0, 0, 0);
            audio.source.SetVelocity(0, 0, 0);

            audio.buffer = AudioBuffer::Create();
            audio.buffer.SetData(wav, len, freq, audioFormat);
            
            audio.source.SetBuffer(audio.buffer);

            if (outAudio)
            {
                outAudio[0] = audio;
            }

            _audios.SetValue(HashString(path), audio);
            Assets::FreeWave(wav);
            return true;
        }
        return false;
    }

    void Play(const char* path)
    {
        AudioDefine audio;
        if (Load(path, &audio))
        {
            audio.source.Play();
        }
    }

    void Stop(const char* path)
    {
        AudioDefine audio;
        if (_audios.TryGetValue(HashString(path), &audio))
        {                        
            audio.source.Stop();
        }
    }

    const char* shoot_audio_paths[] =
    {
        "Audios/shoot-01.wav",
        "Audios/shoot-02.wav",
        "Audios/shoot-03.wav",
        "Audios/shoot-04.wav",
    };

    void PlayShoot(void)
    {
        const int count = _countof(shoot_audio_paths);
        
        int index = rand() % count;
        Play(shoot_audio_paths[index]);
    }

    void StopShoot(void)
    {
        for (int i = 0, n = _countof(shoot_audio_paths); i < 0; i++)
        {
            Stop(shoot_audio_paths[i]);
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

    void PlayExplosion(void)
    {
        const int count = _countof(explosion_audio_paths);

        int index = rand() % count;
        Play(explosion_audio_paths[index]);
    }

    void StopExplosion(void)
    {
        for (int i = 0, n = _countof(explosion_audio_paths); i < 0; i++)
        {
            Stop(explosion_audio_paths[i]);
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

    void PlaySpawn(void)
    {
        const int count = _countof(spawn_audio_paths);

        int index = rand() % count;
        Play(spawn_audio_paths[index]);
    }

    void StopSpawn(void)
    {
        for (int i = 0, n = _countof(spawn_audio_paths); i < 0; i++)
        {
            Stop(spawn_audio_paths[i]);
        }
    }
}