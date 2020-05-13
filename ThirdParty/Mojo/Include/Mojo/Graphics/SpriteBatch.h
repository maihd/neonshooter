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
            Vector2 pos;
            Vector2 uv;
        };

        struct DrawCommand
        {
            Texture     texture;
            int         drawCount;
            Vector2     position;
            Vector2     scale;
            float       rotation;
            Vector4     color;

            BlendFunc   blend;
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
            const Vector2& position = Vector2(0.0f),
            float rotation = 0.0f,
            const Vector2& scale = Vector2(1.0f),
            const Vector4& color = Vector4(1.0f),
            BlendFunc blend = BlendModes::Alpha
        );
        
        void Clear(void);
        void Present(const Matrix4& projection);
    };
}