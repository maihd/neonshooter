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

    void SpriteBatch::DrawTexture(const Texture& texture, const Vector2& position, float rotation, const Vector2& scale, const Vector4& color, BlendFunc blend)
    {
        DrawCommand cmd;
        cmd.texture = texture;
        cmd.position = position;
        cmd.rotation = rotation;
        cmd.scale = scale * Vector2((float)texture.width, (float)texture.height);
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
        v.pos = Vector2(-0.5f, -0.5f);
        v.uv = Vector2(0.0f, 1.0f);
        _vertices.Push(v);

        v.pos = Vector2(-0.5f, 0.5f);
        v.uv = Vector2(0.0f, 0.0f);
        _vertices.Push(v);

        v.pos = Vector2(0.5f, 0.5f);
        v.uv = Vector2(1.0f, 0.0f);
        _vertices.Push(v);

        v.pos = Vector2(0.5f, -0.5f);
        v.uv = Vector2(1.0f, 1.0f);
        _vertices.Push(v);
    }

    void SpriteBatch::Clear(void)
    {
        _drawCmds.Clear();
        _vertices.Clear();
        _indices.Clear();
    }

    void SpriteBatch::Present(const Matrix4& projection)
    {
        Graphics::BindShader(_spriteShader);
        Graphics::BindVertexArray(_spriteVertexArray);
        Graphics::BindIndexBuffer(_spriteIndexBuffer);
        Graphics::BindVertexBuffer(_spriteVertexBuffer);

        _spriteVertexBuffer.SetData(_vertices.elements, _vertices.count * sizeof(Vertex), BufferUsage::StreamDraw);
        _spriteIndexBuffer.SetData(_indices.elements, _indices.count * sizeof(unsigned short), BufferUsage::StreamDraw);

        int offset = 0;
        for (int i = 0, n = _drawCmds.count; i < n; i++)
        {
            const DrawCommand& cmd = _drawCmds[i];

            Graphics::SetBlendFunc(cmd.blend);
            //_spriteVertexBuffer.SetBlendFunc(cmd.blend);

            Matrix4 model_matrix = Math::Transform(cmd.position, cmd.rotation, cmd.scale);
            Matrix4 MVP_matrix = Math::Mul(projection, model_matrix);

            _spriteShader.SetMatrix4("MVP", (float*)&MVP_matrix);
            _spriteShader.SetVector4("color", cmd.color.x, cmd.color.y, cmd.color.z, cmd.color.w);

            Graphics::BindTexture(cmd.texture);
            Graphics::DrawIndices(DrawType::Triangles, DataType::U16, cmd.drawCount, offset);
            
            offset += 0;// cmd.drawCount;
        }
    }
}