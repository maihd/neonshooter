#pragma once

#include <Mojo/Math.h>
#include <Mojo/Graphics.h>
#include <Mojo/Core/Array.h>
#include <Mojo/Graphics/Shader.h>
#include <Mojo/Graphics/Texture.h>

inline namespace Mojo
{
    struct Sprite
    {
        float u0, v0;
        float u1, v1;

        float width;
        float height;

        inline Sprite(const Texture& texture)
            : u0(0), v0(1)
            , u1(0), v1(1)
            , width((float)texture.width)
            , height((float)texture.height)
        {
        }
    };

    struct SpriteBatch
    {
        struct Vertex
        {
            float2 pos;
            float2 uv;
        };

        struct DrawCommand
        {
            TextureHandle*      texture;
            int                 drawCount;
            float2              position;
            float2              scale;
            float               rotation;
            float4              color;

            BlendFunc           blend;
        };

        Array<DrawCommand>      _drawCmds;
        Array<Vertex>           _vertices;
        Array<unsigned short>   _indices;

        Shader                  _spriteShader;
        VertexArray             _spriteVertexArray;
        IndexBuffer             _spriteIndexBuffer;
        VertexBuffer            _spriteVertexBuffer;

        SpriteBatch(void);

        void DrawTexture(
            const Texture& texture,
            float2 position = float2(0.0f),
            float rotation = 0.0f,
            float2 scale = float2(1.0f),
            float4 color = float4(1.0f),
            BlendFunc blend = { BlendFactor::SrcAlpha, BlendFactor::InvertSrcAlpha });
        
        void Clear(void);
        void Present(const float4x4& projection);
    };
}