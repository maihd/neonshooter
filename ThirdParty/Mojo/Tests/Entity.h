#pragma once

#include <Mojo/GL.h>
#include <Mojo/Math.h>

struct Entity
{
    bool    active;

    vec2    scale;
    vec2    position;
    float   rotation;

    vec2    velocity;
    float   movespeed;
    
    vec4    color;
    float   radius;

    Texture texture;
};

struct Particle
{
    bool     active;
    Texture  texture;
    vec2     velocity;
    vec2     position;
    float    rotation;
    vec2     scale;
    vec4     color;
    float    timer;
    float    duration;
};

namespace Audio
{
    void PlayShoot(void);
    void StopShoot(void);
    void PlaySpawn(void);
    void StopSpawn(void);
    void PlayExplosion(void);
    void StopExplosion(void);
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