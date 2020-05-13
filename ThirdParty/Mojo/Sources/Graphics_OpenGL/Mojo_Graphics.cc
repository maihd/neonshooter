#include <stdint.h>
#include <assert.h>
#include <GL/glew.h>

#include <Mojo/Math.h>
#include <Mojo/Graphics/Shader.h>
#include <Mojo/Graphics/Texture.h>
#include <Mojo/Graphics.h>

namespace Mojo
{
    namespace
    {
        static void HandleError()
        {
            glGetError();
        }

        static GLenum ConvertBufferUsage(BufferUsage usage)
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
            case DataType::U8:
                return GL_UNSIGNED_BYTE;

            case DataType::I8:
                return GL_BYTE;

            case DataType::I32:
                return GL_INT;

            case DataType::U32:
                return GL_UNSIGNED_INT;

            case DataType::I16:
                return GL_SHORT;

            case DataType::U16:
                return GL_UNSIGNED_SHORT;

            case DataType::Float:
                return GL_FLOAT;

            case DataType::Double:
                return GL_DOUBLE;
            }

            return 0;
        }

        static int ConvertDataSize(DataType type)
        {
            switch (type)
            {
            case DataType::U8:
                return 1;

            case DataType::I8:
                return 1;

            case DataType::I32:
                return 4;

            case DataType::U32:
                return 4;

            case DataType::I16:
                return 2;

            case DataType::U16:
                return 2;

            case DataType::Float:
                return 4;

            case DataType::Double:
                return 8;
            }

            return 0;
        }

        static GLenum ConvertBlendOp(BlendOp op)
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

        static GLenum ConvertBlendFactor(BlendFactor mode)
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

    namespace
    {
        Vector4       _globalColor;

        VertexArray  _renderTargetVertexArray;
        VertexBuffer _renderTargetVertexBuffer;

        const char*  _shapeVertexProcessor =
            "#version 330 core\n"
            "layout (location = 0) in vec2 vertex;"
            "out vec4 VertexPosition;"
            "out vec2 VertexTexCoord;"
            "out vec4 VertexColor;"
            "void main() {"
            "gl_Position = vec4(vertex.xy, 0, 1.0);"
            "}";

        const char*  _shapePixelsProcessor =
            "#version 330\n"
            "out vec4 fragColor;"
            "uniform vec4 shapeColor;"
            "void main() {"
            "fragColor = shapeColor;"
            "}";

        Shader       _shapeShader;
    }

    IndexBuffer IndexBuffer::Create(void)
    {
        IndexBuffer buffer;
        glGenBuffers(1, (GLuint*)&buffer.handle);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.handle);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        return buffer;
    }

    void IndexBuffer::Destroy(IndexBuffer& buffer)
    {
        glDeleteBuffers(1, (GLuint*)&buffer.handle);
        buffer.handle = 0;
    }

    void IndexBuffer::SetData(const void* data, int size, BufferUsage usage)
    {
        GLint boundHandle;
        glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &boundHandle);
        if (boundHandle == handle)
        {
            ::glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, ConvertBufferUsage(usage));
        }
        else
        {
            ::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
            ::glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, ConvertBufferUsage(usage));
            ::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (GLuint)boundHandle);
        }
    }

    void IndexBuffer::SetBlendOp(BlendOp op)
    {
        glBlendEquationi(handle, ConvertBlendOp(op));
    }

    void IndexBuffer::SetBlendFunc(BlendFunc func)
    {
        glBlendFunci(handle, ConvertBlendFactor(func.src), ConvertBlendFactor(func.dst));
    }

    void IndexBuffer::SetBlendFunc(BlendFactor src, BlendFactor dst)
    {
        glBlendFunci(handle, ConvertBlendFactor(src), ConvertBlendFactor(dst));
    }

    VertexBuffer VertexBuffer::Create(void)
    {
        VertexBuffer buffer = {};
        glGenBuffers(1, (GLuint*)&buffer.handle);
        glBindBuffer(GL_ARRAY_BUFFER, buffer.handle);
        glBufferData(GL_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        return buffer;
    }

    void VertexBuffer::Destroy(VertexBuffer& buffer)
    {
        ::glDeleteBuffers(1, (GLuint*)&buffer.handle);
        buffer.handle = 0;
    }

    void VertexBuffer::SetData(const void* data, int size, BufferUsage usage)
    {
        GLint boundHandle;
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &boundHandle);
        
        if (boundHandle == handle)
        {
            ::glBufferData(GL_ARRAY_BUFFER, size, data, ConvertBufferUsage(usage));
        }
        else
        {
            ::glBindBuffer(GL_ARRAY_BUFFER, handle);
            ::glBufferData(GL_ARRAY_BUFFER, size, data, ConvertBufferUsage(usage));
            ::glBindBuffer(GL_ARRAY_BUFFER, (GLuint)boundHandle);
        }
    }

    void VertexBuffer::SetBlendOp(BlendOp op) 
    {
        glBlendEquationi(handle, ConvertBlendOp(op));
    }

    void VertexBuffer::SetBlendFunc(BlendFunc func)
    {
        glBlendFunci(handle, ConvertBlendFactor(func.src), ConvertBlendFactor(func.dst));
    }

    void VertexBuffer::SetBlendFunc(BlendFactor src, BlendFactor dst)
    {
        glBlendFunci(handle, ConvertBlendFactor(src), ConvertBlendFactor(dst));
    }
    
    VertexArray VertexArray::Create(void)
    {
        VertexArray array;
        glGenVertexArrays(1, (GLuint*)&array.handle);
        return array;
    }

    void VertexArray::Destroy(VertexArray& array)
    {
        glDeleteVertexArrays(1, (GLuint*)&array.handle);
        array.handle = 0;
    }

    void VertexArray::SetAttribute(const VertexBuffer& buffer, int location, int size, DataType dataType, bool normalized, int stride, int offset)
    {
        GLenum glDataType = ConvertDataType(dataType);

        glBindVertexArray(handle);
        glBindBuffer(GL_ARRAY_BUFFER, buffer.handle);

        glEnableVertexAttribArray(location);
        glVertexAttribPointer(location, size, glDataType, normalized, stride, (const void*)(GLintptr)offset);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    RenderTarget RenderTarget::Create(float width, float height)
    {
        RenderTarget renderTarget;

        int pixelsWidth  = (int)width;
        int pixelsHeight = (int)height;

        renderTarget.texture = Texture::Create();
        renderTarget.texture.SetPixels(pixelsWidth, pixelsHeight, PixelFormat::RGB, nullptr, PixelFormat::RGB);

        // Failed to generate texture
        if (!renderTarget.texture.handle)
        {
            return renderTarget;
        }

        // Generate depth stencil buffer
        //glGenTextures(1, &renderTarget.depthStencilTexture);
        //if (!renderTarget.depthStencilTexture)
        //{
        //    return renderTarget;
        //}
        //glBindTexture(GL_TEXTURE_2D, renderTarget.colorTexture);
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
        //glBindTexture(GL_TEXTURE_2D, 0);

        // Generate render buffer
        glGenRenderbuffers(1, (GLuint*)&renderTarget.renderBuffer);
        glNamedRenderbufferStorage(renderTarget.renderBuffer, GL_DEPTH24_STENCIL8, pixelsWidth, pixelsHeight);

        // Generate frame buffer
        GLint bindingFramebuffer;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &bindingFramebuffer);

        glGenFramebuffers(1, (GLuint*)&renderTarget.frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, renderTarget.frameBuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTarget.texture.handle, 0); 
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderTarget.renderBuffer);
        //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, renderTarget.depthStencilTexture, 0);

        GLenum status;
        status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        switch (status)
        {
        case GL_FRAMEBUFFER_COMPLETE:
            break;
        
        default:
            return renderTarget;
        }
        
        glBindFramebuffer(GL_FRAMEBUFFER, bindingFramebuffer);

        return renderTarget;
    }

    void RenderTarget::Destroy(RenderTarget& renderTarget)
    {

    }

    namespace Graphics
    {
        void ApplyDefaultSettings(void)
        {
            // Default graphics mode
            glEnable(GL_BLEND);
            //glEnable(GL_DEPTH_TEST);
            //glEnable(GL_STENCIL_TEST);

            glEnable(GL_MULTISAMPLE);
            glEnable(GL_LINE_SMOOTH);
            glEnable(GL_POLYGON_SMOOTH);

            glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
            glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

            // Default blend
            Graphics::SetBlendOp(BlendOp::Add);
            Graphics::SetBlendFunc(BlendFactor::SrcAlpha, BlendFactor::InvertSrcAlpha);
        }

        void CreateDefaultObjects(void)
        {
            Vector2 vertices[] =
            {
                // First triangle
                Vector2(-1.0f, -1.0f), Vector2(0.0f, 0.0f),
                Vector2(-1.0f,  1.0f), Vector2(0.0f, 1.0f),
                Vector2( 1.0f,  1.0f), Vector2(1.0f, 1.0f),

                // Second triangle
                Vector2( 1.0f,  1.0f), Vector2(1.0f, 1.0f),
                Vector2( 1.0f, -1.0f), Vector2(1.0f, 0.0f),
                Vector2(-1.0f, -1.0f), Vector2(0.0f, 0.0f),
            };

            _renderTargetVertexArray = VertexArray::Create();
            _renderTargetVertexBuffer = VertexBuffer::Create();

            _renderTargetVertexBuffer.SetData(vertices, sizeof(vertices), BufferUsage::StaticDraw);
            _renderTargetVertexArray.SetAttribute(_renderTargetVertexBuffer, 0, 4, DataType::Float, false, 4 * sizeof(float));

            _shapeShader = Shader::CreateEffect(_shapePixelsProcessor);
        }

        void SetBlendOp(BlendOp op)
        {
            glBlendEquation(ConvertBlendOp(op));
        }

        void SetBlendFunc(BlendFunc func)
        {
            glBlendFunc(ConvertBlendFactor(func.src), ConvertBlendFactor(func.dst));
        }

        void SetBlendFunc(BlendFactor src, BlendFactor dst)
        {
            glBlendFunc(ConvertBlendFactor(src), ConvertBlendFactor(dst));
        }

        void Scissor(float x, float y, float width, float height)
        {
            glScissor((GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height);
        }

        void Viewport(float x, float y, float width, float height)
        {
            glViewport((GLint)x, (GLint)y, (GLsizei)width, (GLsizei)height);
        }

        void BindShader(const Shader& shader)
        {
            glUseProgram(shader.handle);
        }

        void BindTexture(const Texture& texture, int index)
        {
            glActiveTexture(GL_TEXTURE0 + index);
            glBindTexture(GL_TEXTURE_2D, texture.handle);
        }

        void BindVertexArray(const VertexArray& array)
        {
            glBindVertexArray(array.handle);
        }

        void BindIndexBuffer(const IndexBuffer& buffer)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.handle);
        }

        void BindVertexBuffer(const VertexBuffer& buffer)
        {
            glBindBuffer(GL_ARRAY_BUFFER, buffer.handle);
        }

        void BindRenderTarget(const RenderTarget& renderTarget)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, renderTarget.frameBuffer);
        }

        void BlitRenderTarget(const RenderTarget& src, RenderTarget& dst, const Shader& shader)
        {
            if (src.texture.handle)
            {
                GLint bindingFramebuffer;
                glGetIntegerv(GL_FRAMEBUFFER_BINDING, &bindingFramebuffer);

                glBindFramebuffer(GL_FRAMEBUFFER, dst.frameBuffer);

                // Clear dst buffer
                Graphics::Clear();

                glUseProgram(shader.handle);
                glBindVertexArray(_renderTargetVertexArray.handle);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, src.texture.handle);

                glDrawArrays(GL_TRIANGLES, 0, 6);

                glBindTexture(GL_TEXTURE_2D, 0);

                glBindVertexArray(0);
                glUseProgram(0);

                glBindFramebuffer(GL_FRAMEBUFFER, bindingFramebuffer);
            }
        }

        void BlitRenderTarget(const RenderTarget& src, const Shader& shader)
        {
            if (src.texture.handle)
            {
                GLint bindingFramebuffer;
                glGetIntegerv(GL_FRAMEBUFFER_BINDING, &bindingFramebuffer);

                glBindFramebuffer(GL_FRAMEBUFFER, 0);

                // Clear dst buffer
                Graphics::Clear();

                glUseProgram(shader.handle);
                glBindVertexArray(_renderTargetVertexArray.handle);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, src.texture.handle);

                glDrawArrays(GL_TRIANGLES, 0, 6);

                glBindTexture(GL_TEXTURE_2D, 0);

                glBindVertexArray(0);
                glUseProgram(0);

                glBindFramebuffer(GL_FRAMEBUFFER, bindingFramebuffer);
            }
        }

        void DrawArrays(DrawType type, int count, int offset)
        {
            GLenum glDrawType = (GLenum)type;
            glDrawArrays(glDrawType, offset, count);
        }

        void DrawIndices(DrawType type, DataType dataType, int count, int offset)
        {
            GLenum glDrawType = (GLenum)type;
            glDrawElements(glDrawType, count, ConvertDataType(dataType), (const void*)(GLintptr)(offset * ConvertDataSize(dataType)));
        }

        void DrawArrays(DrawType type, const Shader& shader, const VertexArray& array, int count, int offset)
        {
            GLenum glDrawType = (GLenum)type;

            glUseProgram(shader.handle);
            glBindVertexArray(array.handle);
            glDrawArrays(glDrawType, offset, count);
            glBindVertexArray(0);
            glUseProgram(0);
        }

        void DrawIndices(DrawType type, const Shader& shader, const VertexArray& array, const IndexBuffer& indices, int count, int offset)
        {
            GLenum glDrawType = (GLenum)type;

            glUseProgram(shader.handle);

            glBindVertexArray(array.handle);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices.handle);
            
            //glDrawElements(glDrawType, count, ConvertDataType(indices._dataType), (const void*)(offset * ConvertDataSize(indices._dataType)));
            
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glBindVertexArray(0);

            glUseProgram(0);
        }

        void DrawArrays(DrawType type, const Shader& shader, const VertexArray& array, const Texture& texture, int count, int offset)
        {
            GLenum glDrawType = (GLenum)type;

            glUseProgram(shader.handle);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture.handle);
            
            glBindVertexArray(array.handle);
            
            glDrawArrays(glDrawType, offset, count);
            
            glBindVertexArray(0);

            glBindTexture(GL_TEXTURE_2D, 0);
            glUseProgram(0);
        }

        void DrawIndices(DrawType type, const Shader& shader, const VertexArray& array, const Texture& texture, const IndexBuffer& indices, int count, int offset)
        {
            GLenum glDrawType = (GLenum)type;

            glUseProgram(shader.handle);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture.handle);
            
            glBindVertexArray(array.handle);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices.handle);
            
            //const void* memoryOffset = (const void*)(GLintptr)(offset * ConvertDataSize(indices._dataType));
            //glDrawElements(glDrawType, count, ConvertDataType(indices._dataType), memoryOffset);
            
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glBindVertexArray(0);

            glBindTexture(GL_TEXTURE_2D, 0);
            glUseProgram(0);
        }

        void SetColor(unsigned color24, float a)
        {
            const float factor = 1.0f / 255.0f;

            float r = ((color24 >> 16) & 0xff) * factor;
            float g = ((color24 >>  8) & 0xff) * factor;
            float b = ((color24 >>  0) & 0xff) * factor;

            _globalColor = Vector4(r, g, b, a);
        }

        void SetColor(float r, float g, float b, float a)
        {
            _globalColor = Vector4(r, g, b, a);
        }

        void Rectangle(DrawMode mode, float x, float y, float w, float h)
        {
            Matrix4 transform = Math::Transform(Vector2(x, y), 0.0, Vector2(w, h));
        }
    }
}