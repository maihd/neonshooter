#include <stdint.h>
#include <assert.h>
#include <GL/glew.h>

#include <Mojo/Math.h>
#include <Mojo/Shader.h>
#include <Mojo/Texture.h>
#include <Mojo/Graphics.h>

inline namespace Mojo
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

        static int ConvertDataSize(DataType type)
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
        VertexArray  _renderTargetVertexArray;
        VertexBuffer _renderTargetVertexBuffer;
    }

    IndexBuffer IndexBuffer::Create(void)
    {
        IndexBuffer buffer;
        glGenBuffers(1, &buffer._handle);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer._handle);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        return buffer;
    }

    void IndexBuffer::Destroy(IndexBuffer& buffer)
    {
        glDeleteBuffers(1, &buffer._handle);
        buffer._handle = 0;
    }

    void IndexBuffer::SetData(const void* data, int size, DataType type, BufferUsage usage)
    {
        _dataType = type;

        GLint boundHandle;
        glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &boundHandle);
        if (boundHandle == _handle)
        {
            ::glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, ConvertBufferUsage(usage));
        }
        else
        {
            ::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _handle);
            ::glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, ConvertBufferUsage(usage));
            ::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (GLuint)boundHandle);
        }

        //glNamedBufferData(_handle, size, data, ConvertBufferUsage(usage));
    }

    void IndexBuffer::SetBlendOp(BlendOp op)
    {
        glBlendEquationi(_handle, ConvertBlendOp(op));
    }

    void IndexBuffer::SetBlendFunc(BlendFunc func)
    {
        glBlendFunci(_handle, ConvertBlendFactor(func.src), ConvertBlendFactor(func.dst));
    }

    void IndexBuffer::SetBlendFunc(BlendFactor src, BlendFactor dst)
    {
        glBlendFunci(_handle, ConvertBlendFactor(src), ConvertBlendFactor(dst));
    }

    VertexBuffer VertexBuffer::Create(void)
    {
        VertexBuffer buffer = {};
        glGenBuffers(1, &buffer._handle);
        glBindBuffer(GL_ARRAY_BUFFER, buffer._handle);
        glBufferData(GL_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        return buffer;
    }

    void VertexBuffer::Destroy(VertexBuffer& buffer)
    {
        ::glDeleteBuffers(1, &buffer._handle);
        buffer._handle = 0;
    }

    void VertexBuffer::SetData(const void* data, int size, BufferUsage usage)
    {
        GLint boundHandle;
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &boundHandle);
        
        if (boundHandle == _handle)
        {
            ::glBufferData(GL_ARRAY_BUFFER, size, data, ConvertBufferUsage(usage));
        }
        else
        {
            ::glBindBuffer(GL_ARRAY_BUFFER, _handle);
            ::glBufferData(GL_ARRAY_BUFFER, size, data, ConvertBufferUsage(usage));
            ::glBindBuffer(GL_ARRAY_BUFFER, (GLuint)boundHandle);
        }

        //glNamedBufferData(_handle, size, data, ConvertBufferUsage(usage));
    }

    void VertexBuffer::SetBlendOp(BlendOp op) 
    {
        glBlendEquationi(_handle, ConvertBlendOp(op));
    }

    void VertexBuffer::SetBlendFunc(BlendFunc func)
    {
        glBlendFunci(_handle, ConvertBlendFactor(func.src), ConvertBlendFactor(func.dst));
    }

    void VertexBuffer::SetBlendFunc(BlendFactor src, BlendFactor dst)
    {
        glBlendFunci(_handle, ConvertBlendFactor(src), ConvertBlendFactor(dst));
    }
    
    VertexArray VertexArray::Create(void)
    {
        VertexArray array;
        glGenVertexArrays(1, &array._handle);
        return array;
    }

    void VertexArray::Destroy(VertexArray& array)
    {
        glDeleteVertexArrays(1, &array._handle);
        array._handle = 0;
    }

    void VertexArray::SetAttribute(const VertexBuffer& buffer, int location, int size, DataType dataType, bool normalized, int stride, int offset)
    {
        GLenum glDataType = ConvertDataType(dataType);

        glBindVertexArray(_handle);
        glBindBuffer(GL_ARRAY_BUFFER, buffer._handle);

        glEnableVertexAttribArray(location);
        glVertexAttribPointer(location, size, glDataType, normalized, stride, (const void*)(intptr_t)offset);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    RenderTarget RenderTarget::Create(int width, int height)
    {
        RenderTarget renderTarget;

        // Generate color texture
        glGenTextures(1, &renderTarget.texture);
        if (!renderTarget.texture)
        {
            return renderTarget;
        }

        glBindTexture(GL_TEXTURE_2D, renderTarget.texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glBindTexture(GL_TEXTURE_2D, 0);

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
        glGenRenderbuffers(1, &renderTarget.renderBuffer);
        glNamedRenderbufferStorage(renderTarget.renderBuffer, GL_DEPTH24_STENCIL8, width, height);

        // Generate frame buffer
        GLint bindingFramebuffer;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &bindingFramebuffer);

        glGenFramebuffers(1, &renderTarget.frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, renderTarget.frameBuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTarget.texture, 0); 
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
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_STENCIL_TEST);

            // Default blend
            Graphics::SetBlendOp(BlendOp::Add);
            Graphics::SetBlendFunc(BlendFactor::SrcAlpha, BlendFactor::InvertSrcAlpha);

            // First clear and swap buffer
            Graphics::ClearBuffer();
            Graphics::SwapBuffers();
            Graphics::ClearBuffer();
            Graphics::SwapBuffers();
        }

        void CreateDefaultObjects(void)
        {
            float2 vertices[] =
            {
                // First triangle
                float2(-1.0f, -1.0f), float2(0.0f, 0.0f),
                float2(-1.0f,  1.0f), float2(0.0f, 1.0f),
                float2( 1.0f,  1.0f), float2(1.0f, 1.0f),

                // Second triangle
                float2( 1.0f,  1.0f), float2(1.0f, 1.0f),
                float2( 1.0f, -1.0f), float2(1.0f, 0.0f),
                float2(-1.0f, -1.0f), float2(0.0f, 0.0f),
            };

            _renderTargetVertexArray = VertexArray::Create();
            _renderTargetVertexBuffer = VertexBuffer::Create();

            _renderTargetVertexBuffer.SetData(vertices, sizeof(vertices), BufferUsage::StaticDraw);
            _renderTargetVertexArray.SetAttribute(_renderTargetVertexBuffer, 0, 4, DataType::Float, false, 4 * sizeof(float));
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
            glUseProgram(shader._handle);
        }

        void BindTexture(const Texture& texture, int index)
        {
            glActiveTexture(GL_TEXTURE0 + index);
            glBindTexture(GL_TEXTURE_2D, texture._handle);
        }

        void BindVertexArray(const VertexArray& array)
        {
            glBindVertexArray(array._handle);
        }

        void BindIndexBuffer(const IndexBuffer& buffer)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer._handle);
        }

        void BindVertexBuffer(const VertexBuffer& buffer)
        {
            glBindBuffer(GL_ARRAY_BUFFER, buffer._handle);
        }

        void BindRenderTarget(RenderTarget* renderTarget)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, renderTarget ? renderTarget->frameBuffer : 0);
        }

        void BlitRenderTarget(RenderTarget* src, RenderTarget* dst, const Shader& shader)
        {
            if (src)
            {
                GLint bindingFramebuffer;
                glGetIntegerv(GL_FRAMEBUFFER_BINDING, &bindingFramebuffer);

                glBindFramebuffer(GL_FRAMEBUFFER, dst ? dst->frameBuffer : 0);

                // Clear dst buffer
                Graphics::ClearBuffer();

                glUseProgram(shader._handle);
                glBindVertexArray(_renderTargetVertexArray._handle);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, src->texture);

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
            glDrawElements(glDrawType, count, ConvertDataType(dataType), (const void*)(intptr_t)(offset * ConvertDataSize(dataType)));
        }

        void DrawArrays(DrawType type, const Shader& shader, const VertexArray& array, int count, int offset)
        {
            GLenum glDrawType = (GLenum)type;

            glUseProgram(shader._handle);
            glBindVertexArray(array._handle);
            glDrawArrays(glDrawType, offset, count);
            glBindVertexArray(0);
            glUseProgram(0);
        }

        void DrawIndices(DrawType type, const Shader& shader, const VertexArray& array, const IndexBuffer& indices, int count, int offset)
        {
            GLenum glDrawType = (GLenum)type;

            glUseProgram(shader._handle);

            glBindVertexArray(array._handle);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices._handle);
            
            glDrawElements(glDrawType, count, ConvertDataType(indices._dataType), (const void*)(offset * ConvertDataSize(indices._dataType)));
            
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glBindVertexArray(0);

            glUseProgram(0);
        }

        void DrawArrays(DrawType type, const Shader& shader, const VertexArray& array, const Texture& texture, int count, int offset)
        {
            GLenum glDrawType = (GLenum)type;

            glUseProgram(shader._handle);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture._handle);
            
            glBindVertexArray(array._handle);
            
            glDrawArrays(glDrawType, offset, count);
            
            glBindVertexArray(0);

            glBindTexture(GL_TEXTURE_2D, 0);
            glUseProgram(0);
        }

        void DrawIndices(DrawType type, const Shader& shader, const VertexArray& array, const Texture& texture, const IndexBuffer& indices, int count, int offset)
        {
            GLenum glDrawType = (GLenum)type;

            glUseProgram(shader._handle);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture._handle);
            
            glBindVertexArray(array._handle);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices._handle);
            
            const void* memoryOffset = (const void*)(intptr_t)(offset * ConvertDataSize(indices._dataType));
            glDrawElements(glDrawType, count, ConvertDataType(indices._dataType), memoryOffset);
            
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glBindVertexArray(0);

            glBindTexture(GL_TEXTURE_2D, 0);
            glUseProgram(0);
        }
    }
}