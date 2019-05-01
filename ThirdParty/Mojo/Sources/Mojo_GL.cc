#include <stdint.h>
#include <GL/glew.h>
#include <Mojo/GL.h>
#include <Mojo/Math.h>

inline namespace Mojo
{
    inline namespace Converters
    {
        GLenum ConvertBufferUsage(BufferUsage usage)
        {
            switch (usage)
            {
            case BufferUsage::StaticCopy:
                return GL_STATIC_COPY;

            case BufferUsage::StaticDraw:
                return GL_STATIC_DRAW;

            case BufferUsage::StaticRead:
                return GL_STATIC_READ;

            case BufferUsage::StreamCopy:
                return GL_STREAM_COPY;

            case BufferUsage::StreamDraw:
                return GL_STREAM_DRAW;

            case BufferUsage::StreamRead:
                return GL_STREAM_READ;

            case BufferUsage::DynamicCopy:
                return GL_DYNAMIC_COPY;

            case BufferUsage::DynamicDraw:
                return GL_DYNAMIC_DRAW;

            case BufferUsage::DynamicRead:
                return GL_DYNAMIC_READ;

            default:
                return 0;
            }
        }

        GLenum ConvertDataType(DataType type)
        {
            switch (type)
            {
            case DataType::Byte:
                return GL_UNSIGNED_BYTE;

            case DataType::SignedByte:
                return GL_BYTE;

            case DataType::Int:
                return GL_INT;

            case DataType::Uint:
                return GL_UNSIGNED_INT;

            case DataType::Short:
                return GL_SHORT;

            case DataType::Ushort:
                return GL_UNSIGNED_SHORT;

            case DataType::Float:
                return GL_FLOAT;

            case DataType::Double:
                return GL_DOUBLE;
            }

            return 0;
        }

        int ConvertDataSize(DataType type)
        {
            switch (type)
            {
            case DataType::Byte:
                return 1;

            case DataType::SignedByte:
                return 1;

            case DataType::Int:
                return 4;

            case DataType::Uint:
                return 4;

            case DataType::Short:
                return 2;

            case DataType::Ushort:
                return 2;

            case DataType::Float:
                return 4;

            case DataType::Double:
                return 8;
            }

            return 0;
        }

        GLenum ConvertTextureType(TextureType type)
        {
            switch (type)
            {
            case TextureType::Texture1D:
                return GL_TEXTURE_1D;

            case TextureType::Texture2D:
                return GL_TEXTURE_2D;

            case TextureType::TextureCube:
                return GL_TEXTURE_3D;

            default:
                break;
            }

            return 0;
        }

        GLenum ConvertTextureWrap(TextureWrap wrap)
        {
            switch (wrap)
            {
            case TextureWrap::Clamp:
                return GL_CLAMP;

            case TextureWrap::Repeat:
                return GL_REPEAT;
            }

            return 0;
        }

        GLenum ConvertTextureFilter(TextureFilter filter)
        {
            switch (filter)
            {
            case TextureFilter::Linear:
                return GL_LINEAR;

            case TextureFilter::Nearest:
                return GL_NEAREST;
            }

            return 0;
        }

        GLenum ConvertPixelFormat(PixelFormat format)
        {
            switch (format)
            {
            case PixelFormat::RGB:
                return GL_RGB;

            case PixelFormat::RGBA:
                return GL_RGBA;
            }

            return 0;
        }

        GLenum ConvertBlendOp(BlendOp op)
        {
            switch (op)
            {
            case BlendOp::Add:
                return GL_ADD;

            case BlendOp::Subtract:
                return GL_SUBTRACT;

            case BlendOp::ReverseSubstract:
                return GL_FUNC_REVERSE_SUBTRACT;

            case BlendOp::Min:
                return GL_MIN;

            case BlendOp::Max:
                return GL_MAX;
                
            default:
                break;
            }

            return 0;
        }

        GLenum ConvertBlendFactor(BlendFactor mode)
        {
            switch (mode)
            {
            case BlendFactor::One:
                return GL_ONE;

            case BlendFactor::Zero:
                return GL_ZERO;

            case BlendFactor::SrcAlpha:
                return GL_SRC_ALPHA;

            case BlendFactor::InvertSrcAlpha:
                return GL_ONE_MINUS_SRC_ALPHA;

            case BlendFactor::DstAlpha:
                return GL_DST_ALPHA;

            case BlendFactor::InvertDstAlpha:
                return GL_ONE_MINUS_DST_ALPHA;

            case BlendFactor::SrcColor:
                return GL_SRC_COLOR;

            case BlendFactor::InvertSrcColor:
                return GL_ONE_MINUS_SRC_COLOR;

            case BlendFactor::DstColor:
                return GL_DST_COLOR;

            case BlendFactor::InvertDstColor:
                return GL_ONE_MINUS_DST_COLOR;

            default:
                break;
            }

            return 0;
        }
    }

    inline namespace OpenGL
    {
        static GLuint CreateGLShader(GLenum type, const char* src)
        {
            GLuint shader = glCreateShader(type);
            if (!shader)
            {
                const char* error = (const char*)glewGetErrorString(glGetError());
                return 0;
            }

            glShaderSource(shader, 1, &src, 0);
            glCompileShader(shader);

            GLint status;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
            if (!status)
            {
                char errorLog[1024];
                glGetShaderInfoLog(shader, sizeof(errorLog), 0, errorLog);

                glDeleteShader(shader);
                return 0;
            }
            else
            {
                return shader;
            }
        }

        static GLuint CreateGLProgram(GLuint vshader, GLuint fshader)
        {
            if (!vshader || !fshader)
            {
                return 0;
            }

            GLuint program = glCreateProgram();
            if (!program)
            {
                return 0;
            }

            glAttachShader(program, vshader);
            glAttachShader(program, fshader);
            glLinkProgram(program);

            GLint status;
            glGetProgramiv(program, GL_LINK_STATUS, &status);
            if (!status)
            {
                char errorLog[1024];
                glGetProgramInfoLog(program, sizeof(errorLog), 0, errorLog);
                glDeleteProgram(program);
                return 0;
            }
            else
            {
                return program;
            }
        }
    }

    Shader Shader::Create(const char* vertexSource, const char* pixelsSource)
    {
        Shader shader = {};

        GLuint vshader = CreateGLShader(GL_VERTEX_SHADER, vertexSource);
        if (!vshader)
        {
            return shader;
        }

        GLuint fshader = CreateGLShader(GL_FRAGMENT_SHADER, pixelsSource);
        if (!fshader)
        {
            return shader;
        }

        GLuint program = CreateGLProgram(vshader, fshader);
        if (!program)
        {
            return shader;
        }
        
        shader._handle = program;
        return shader;
    }

    void Shader::Destroy(Shader& shader)
    {
        ::glDeleteProgram(shader._handle);
        shader._handle = 0;
    }

    int Shader::FindAttribute(const char* name)
    {
        return ::glGetAttribLocation(_handle, name);
    }

    int Shader::FindUniform(const char* name)
    {
        return ::glGetUniformLocation(_handle, name);
    }

    bool Shader::SetFloat(int location, float x)
    {
        if (location < 0)
        {
            return false;
        }

        ::glProgramUniform1f(_handle, location, x);
        return true;
    }
    
    bool Shader::SetFloat(const char* name, float x)
    {
        return this->SetFloat(this->FindUniform(name), x);
    }
    
    bool Shader::SetFloat2(int location, float x, float y)
    {
        if (location < 0)
        {
            return false;
        }

        ::glProgramUniform2f(_handle, location, x, y);
        return true;
    }

    bool Shader::SetFloat2(const char* name, float x, float y)
    {
        return this->SetFloat2(this->FindUniform(name), x, y);
    }
    
    bool Shader::SetFloat3(int location, float x, float y, float z)
    {
        if (location < 0)
        {
            return false;
        }

        ::glProgramUniform3f(_handle, location, x, y, z);
        return true;
    }

    bool Shader::SetFloat3(const char* name, float x, float y, float z)
    {
        return this->SetFloat3(this->FindUniform(name), x, y, z);
    }
    
    bool Shader::SetFloat4(int location, float x, float y, float z, float w)
    {
        if (location < 0)
        {
            return false;
        }

        ::glProgramUniform4f(_handle, location, x, y, z, w);
        return true;
    }

    bool Shader::SetFloat4(const char* name, float x, float y, float z, float w)
    {
        return this->SetFloat4(this->FindUniform(name), x, y, z, w);
    }

    bool Shader::SetFloat4x4(int location, const float* value, bool transpose)
    {
        if (location < 0)
        {
            return false;
        }

        ::glProgramUniformMatrix4fv(_handle, location, 1, transpose, value);
        return true;
    }

    bool Shader::SetFloat4x4(const char* name, const float* value, bool transpose)
    {
        return this->SetFloat4x4(this->FindUniform(name), value, transpose);
    }

    IndexBuffer IndexBuffer::Create(void)
    {
        IndexBuffer buffer;
        ::glGenBuffers(1, &buffer._handle);
        return buffer;
    }

    void IndexBuffer::Destroy(IndexBuffer& buffer)
    {
        ::glDeleteBuffers(1, &buffer._handle);
        buffer._handle = 0;
    }

    void IndexBuffer::SetData(const void* data, int size, DataType type, BufferUsage usage)
    {
        _dataType = type;

        ::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _handle);
        ::glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, ConvertBufferUsage(usage));
        ::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    VertexBuffer VertexBuffer::Create(void)
    {
        VertexBuffer buffer = {};
        ::glGenBuffers(1, &buffer._handle);
        return buffer;
    }

    void VertexBuffer::Destroy(VertexBuffer& buffer)
    {
        ::glDeleteBuffers(1, &buffer._handle);
        buffer._handle = 0;
    }

    void VertexBuffer::SetData(const void* data, int size, BufferUsage usage)
    {
        ::glBindBuffer(GL_ARRAY_BUFFER, _handle);
        ::glBufferData(GL_ARRAY_BUFFER, size, data, ConvertBufferUsage(usage));
        ::glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
    VertexArray VertexArray::Create(void)
    {
        VertexArray array;
        ::glGenVertexArrays(1, &array._handle);
        return array;
    }

    void VertexArray::Destroy(VertexArray& array)
    {
        ::glDeleteVertexArrays(1, &array._handle);
        array._handle = 0;
    }

    void VertexArray::SetAttribute(const VertexBuffer& buffer, int location, int size, DataType dataType, bool normalized, int stride, int offset)
    {
        GLenum glDataType = ConvertDataType(dataType);

        ::glBindVertexArray(_handle);
        ::glBindBuffer(GL_ARRAY_BUFFER, buffer._handle);

        ::glEnableVertexAttribArray(location);
        ::glVertexAttribPointer(location, size, glDataType, normalized, stride, (const void*)(intptr_t)offset);

        ::glBindBuffer(GL_ARRAY_BUFFER, 0);
        ::glBindVertexArray(0);
    }

    Texture Texture::Create(TextureType type)
    {
        Texture texture;

        ::glGenTextures(1, (GLuint*)&texture._handle);
        if (texture._handle)
        {
            *(TextureType*)&texture._type = type;
        }
        else
        {
            *(TextureType*)&texture._type = TextureType::None;
        }

        return texture;
    }

    void Texture::Destroy(Texture& texture)
    {
        ::glDeleteTextures(1, &texture._handle);

        *(TextureType*)&texture._type   = TextureType::None;
        *(unsigned*   )&texture._handle = 0;
    }
    
    void Texture::SetWrap(TextureWrap wrap)
    {
        this->SetWrap(wrap, wrap);
    }

    void Texture::SetWrap(TextureWrap wrapU, TextureWrap wrapV)
    {
        GLenum glType = ConvertTextureType(_type);

        ::glBindTexture(glType, _handle);
        ::glTexParameteri(glType, GL_TEXTURE_WRAP_S, ConvertTextureWrap(wrapU));
        ::glTexParameteri(glType, GL_TEXTURE_WRAP_T, ConvertTextureWrap(wrapV));
        ::glBindTexture(glType, 0);
    }
    
    void Texture::SetFilter(TextureFilter filter)
    {
        this->SetFilter(filter, filter);
    }

    void Texture::SetFilter(TextureFilter minFilter, TextureFilter magFilter)
    {
        GLenum glType = ConvertTextureType(_type);

        ::glBindTexture(glType, _handle);
        ::glTexParameteri(glType, GL_TEXTURE_MIN_FILTER, ConvertTextureFilter(minFilter));
        ::glTexParameteri(glType, GL_TEXTURE_MAG_FILTER, ConvertTextureFilter(magFilter));
        ::glBindTexture(glType, 0);
    }
    
    void Texture::SetPixels(int w, int h, PixelFormat format, const void* pixels, PixelFormat targetFormat)
    {
        if (w <= 0 || h <= 0)
        {
            return;
        }

        this->width  = w;
        this->height = h;

        GLenum glType = ConvertTextureType(_type);

        ::glBindTexture(glType, _handle);
        switch (_type)
        {
        case TextureType::Texture2D:
            ::glTexImage2D(glType, 0, ConvertPixelFormat(targetFormat), w, h, 0, ConvertPixelFormat(format), GL_UNSIGNED_BYTE, pixels);
            break;
        }
        ::glBindTexture(glType, 0);
    }

    namespace GL
    {
        void Enable(GraphicsMode mode)
        {
            switch (mode)
            {
            case GraphicsMode::Blend:
                ::glEnable(GL_BLEND);
                break;

            case GraphicsMode::Depth:
                ::glEnable(GL_DEPTH);
                ::glEnable(GL_DEPTH_TEST);
                break;

            case GraphicsMode::Stencil:
                ::glEnable(GL_STENCIL);
                ::glEnable(GL_STENCIL_TEST);
                break;
            }
        }

        void Disable(GraphicsMode mode)
        {
            switch (mode)
            {
            case GraphicsMode::Blend:
                ::glDisable(GL_BLEND);
                break;

            case GraphicsMode::Depth:
                ::glDisable(GL_DEPTH);
                ::glDisable(GL_DEPTH_TEST);
                break;

            case GraphicsMode::Stencil:
                ::glDisable(GL_STENCIL);
                ::glDisable(GL_STENCIL_TEST);
                break;
            }
        }

        bool IsEnabled(GraphicsMode mode)
        {
            switch (mode)
            {
            case GraphicsMode::Blend:
                return ::glIsEnabled(GL_BLEND);

            case GraphicsMode::Depth:
                return ::glIsEnabled(GL_DEPTH_TEST);

            case GraphicsMode::Stencil:
                return ::glIsEnabled(GL_STENCIL_TEST);
            }

            return false;
        }

        void SetBlendOp(BlendOp op)
        {
            ::glBlendEquation(ConvertBlendOp(op));
        }

        void SetBlendFunc(BlendFunc func)
        {
            ::glBlendFunc(ConvertBlendFactor(func.src), ConvertBlendFactor(func.dst));
        }

        void SetBlendFunc(BlendFactor src, BlendFactor dst)
        {
            ::glBlendFunc(ConvertBlendFactor(src), ConvertBlendFactor(dst));
        }

        void Scissor(float x, float y, float width, float height)
        {
            ::glScissor((GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height);
        }

        void Viewport(float x, float y, float width, float height)
        {
            ::glViewport((GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height);
        }

        void DrawArrays(DrawType type, int count, int offset)
        {
            GLenum glDrawType = (GLenum)type;
            ::glDrawArrays(glDrawType, offset, count);
        }

        void DrawIndices(DrawType type, DataType dataType, int count, int offset)
        {
            GLenum glDrawType = (GLenum)type;

            ::glDrawElements(glDrawType, count, ConvertDataType(dataType), (const void*)(offset * ConvertDataSize(dataType)));
        }

        void DrawArrays(DrawType type, const Shader& shader, const VertexArray& array, int count, int offset)
        {
            GLenum glDrawType = (GLenum)type;

            ::glUseProgram(shader._handle);
            ::glBindVertexArray(array._handle);
            ::glDrawArrays(glDrawType, offset, count);
            ::glBindVertexArray(0);
            ::glUseProgram(0);
        }

        void DrawIndices(DrawType type, const Shader& shader, const VertexArray& array, const IndexBuffer& indices, int count, int offset)
        {
            GLenum glDrawType = (GLenum)type;

            ::glUseProgram(shader._handle);

            ::glBindVertexArray(array._handle);
            ::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices._handle);
            
            ::glDrawElements(glDrawType, count, ConvertDataType(indices._dataType), (const void*)(offset * ConvertDataSize(indices._dataType)));
            
            ::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            ::glBindVertexArray(0);

            ::glUseProgram(0);
        }

        void DrawArrays(DrawType type, const Shader& shader, const VertexArray& array, const Texture& texture, int count, int offset)
        {
            GLenum glDrawType = (GLenum)type;
            GLenum glTextureType = ConvertTextureType(texture._type);

            ::glUseProgram(shader._handle);
            ::glActiveTexture(GL_TEXTURE0);
            ::glBindTexture(glTextureType, texture._handle);
            
            ::glBindVertexArray(array._handle);
            
            ::glDrawArrays(glDrawType, offset, count);
            
            ::glBindVertexArray(0);

            ::glBindTexture(glTextureType, 0);
            ::glUseProgram(0);
        }

        void DrawIndices(DrawType type, const Shader& shader, const VertexArray& array, const Texture& texture, const IndexBuffer& indices, int count, int offset)
        {
            GLenum glDrawType = (GLenum)type;
            GLenum glTextureType = ConvertTextureType(texture._type);

            ::glUseProgram(shader._handle);
            ::glActiveTexture(GL_TEXTURE0);
            ::glBindTexture(glTextureType, texture._handle);
            
            ::glBindVertexArray(array._handle);
            ::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices._handle);
            
            ::glDrawElements(glDrawType, count, ConvertDataType(indices._dataType), (const void*)(offset * ConvertDataSize(indices._dataType)));
            
            ::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            ::glBindVertexArray(0);

            ::glBindTexture(glTextureType, 0);
            ::glUseProgram(0);
        }
    }
}