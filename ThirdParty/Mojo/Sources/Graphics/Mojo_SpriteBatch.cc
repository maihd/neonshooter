#include <Mojo/Graphics/SpriteBatch.h>

#include <assert.h>

namespace Mojo
{
    constexpr const char* vshader_src =
        "#version 330 core\n"
        "layout (location = 0) in vec4 vertex;"
        "out vec2 uv;"
        "uniform mat4 MVP;"
        "void main() {"
        "uv = vertex.zw;"
        "gl_Position = MVP * vec4(vertex.xy, 0, 1.0);"
        "}";

    constexpr const char* fshader_src =
        "#version 330 core\n"
        "in vec2 uv;"
        "out vec4 fragColor;"
        "uniform vec4 color;"
        "uniform sampler2D image;"
        "void main() {"
        "fragColor = texture(image, uv) * color;"
        "}";

    SpriteBatch::SpriteBatch(void)
    {
        _spriteVertexArray = VertexArray::Create();
        _spriteVertexBuffer = VertexBuffer::Create();
        _spriteIndexBuffer = IndexBuffer::Create();

        _spriteShader = Shader::Create(vshader_src, fshader_src);
        _spriteVertexArray.SetAttribute(_spriteVertexBuffer, 0, 4, DataType::Float, false, sizeof(Vertex));
    }

    void SpriteBatch::DrawTexture(const Texture& texture, float2 position, float rotation, float2 scale, float4 color, BlendFunc blend)
    {
        DrawCommand cmd;
        cmd.texture = texture;
        cmd.position = position;
        cmd.rotation = rotation;
        cmd.scale = scale * float2(texture.width, texture.height);
        cmd.blend = blend;
        cmd.color = color;
        cmd.drawCount = 6;
        if (!_drawCmds.Push(cmd))
        {
            assert(false && "Renderer: Out of memory when push draw call to queue.");
        }

        unsigned short i = (unsigned short)_indices.count;
        _indices.Push((unsigned short)(i + 0U));
        _indices.Push((unsigned short)(i + 1U));
        _indices.Push((unsigned short)(i + 2U));
        _indices.Push((unsigned short)(i + 0U));
        _indices.Push((unsigned short)(i + 2U));
        _indices.Push((unsigned short)(i + 3U));

        Vertex v;
        v.pos = float2(-0.5f, -0.5f);
        v.uv = float2(0.0f, 1.0f);
        _vertices.Push(v);

        v.pos = float2(-0.5f, 0.5f);
        v.uv = float2(0.0f, 0.0f);
        _vertices.Push(v);

        v.pos = float2(0.5f, 0.5f);
        v.uv = float2(1.0f, 0.0f);
        _vertices.Push(v);

        v.pos = float2(0.5f, -0.5f);
        v.uv = float2(1.0f, 1.0f);
        _vertices.Push(v);
    }

    void SpriteBatch::Clear(void)
    {
        _drawCmds.Clear();
        _vertices.Clear();
        _indices.Clear();
    }

    void SpriteBatch::Present(const float4x4& projection)
    {
        Graphics::BindShader(_spriteShader);
        Graphics::BindVertexArray(_spriteVertexArray);
        Graphics::BindIndexBuffer(_spriteIndexBuffer);
        Graphics::BindVertexBuffer(_spriteVertexBuffer);

        _spriteVertexBuffer.SetData(_vertices.elements, _vertices.count * sizeof(Vertex), BufferUsage::StreamDraw);
        _spriteIndexBuffer.SetData(_indices.elements, _indices.count * sizeof(unsigned short), BufferUsage::StreamDraw);

        for (int i = 0, n = _drawCmds.count; i < n; i++)
        {
            const DrawCommand& cmd = _drawCmds[i];

            Graphics::SetBlendFunc(cmd.blend);
            //_spriteVertexBuffer.SetBlendFunc(cmd.blend);

            float4x4 model_matrix = float4x4::translation(cmd.position) * float4x4::rotation_z(cmd.rotation) * float4x4::scalation(cmd.scale);
            float4x4 MVP_matrix = projection * model_matrix;

            _spriteShader.SetFloat4x4("MVP", (float*)&MVP_matrix);
            _spriteShader.SetFloat4("color", cmd.color.x, cmd.color.y, cmd.color.z, cmd.color.w);

            Graphics::BindTexture(cmd.texture);
            Graphics::DrawIndices(DrawType::Triangles, DataType::Uint16, cmd.drawCount, 0);
        }
    }
}