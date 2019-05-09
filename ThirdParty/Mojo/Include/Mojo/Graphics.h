#pragma once

inline namespace Mojo
{
    struct ShaderHandle;
    struct TextureHandle;
    struct IndexBufferHandle;
    struct VertexArrayHandle;
    struct VertexBufferHandle;

    enum struct DrawType
    {
        Point,
        Lines,
        LineLoop,
        LineStrip,

        Triangles,
        TriangleStrip,
        TriangleFan,
    };

    enum struct BufferUsage
    {
        StaticCopy,
        StaticDraw,
        StaticRead,

        StreamCopy,
        StreamDraw,
        StreamRead,
        
        DynamicCopy,
        DynamicDraw,
        DynamicRead,
    };

    enum struct DataType
    {
        None,

        Int, 
        Uint,
        Short,
        Ushort,

        Byte,
        SignedByte,

        Float,
        Double,
    };

    enum struct BlendOp
    {
        Add,
        Subtract,
        ReverseSubstract,
        Min, Max,
    };

    enum struct BlendFactor
    {
        Zero,
        One,

        SrcAlpha,
        InvertSrcAlpha,

        DstAlpha,
        InvertDstAlpha,

        SrcColor,
        InvertSrcColor,

        DstColor,
        InvertDstColor,
    };

    struct BlendFunc
    {
        BlendFactor src;
        BlendFactor dst;
    };

    struct IndexBuffer
    {
        unsigned handle   = 0;
        DataType _dataType = DataType::None;

        static IndexBuffer Create(void);
        static void        Destroy(IndexBuffer& buffer);

        void SetData(const void* data, int size, DataType dataType, BufferUsage usage);

        void SetBlendOp(BlendOp op);
        void SetBlendFunc(BlendFunc func);
        void SetBlendFunc(BlendFactor src, BlendFactor dst);

        inline operator IndexBufferHandle*(void) const
        {
            return (IndexBufferHandle*)(long long)handle;
        }
    };

    struct VertexBuffer
    {
        unsigned handle = 0;

        static VertexBuffer Create(void);
        static void         Destroy(VertexBuffer& buffer);

        void SetData(const void* data, int size, BufferUsage usage);

        void SetBlendOp(BlendOp op);
        void SetBlendFunc(BlendFunc func);
        void SetBlendFunc(BlendFactor src, BlendFactor dst);

        inline operator VertexBufferHandle*(void) const
        {
            return (VertexBufferHandle*)(long long)handle;
        }
    };

    struct VertexArray
    {
        unsigned handle = 0;

        static VertexArray Create(void);
        static void        Destroy(VertexArray& buffer);

        void SetAttribute(const VertexBuffer& buffer, int location, int size, DataType dataType, bool normalized, int stride, int offset = 0);

        inline operator VertexArrayHandle*(void) const
        {
            return (VertexArrayHandle*)(long long)handle;
        }
    };

    struct RenderTarget
    {
        unsigned texture                = 0;
        int      width                  = 0;
        int      height                 = 0;        
        
        unsigned frameBuffer            = 0;
        unsigned renderBuffer           = 0;
        //unsigned depthStencilTexture    = 0;

        //static RenderTarget Create(void);
        static RenderTarget Create(int width, int height);
        static void         Destroy(RenderTarget& renderTarget);
    };

    namespace Graphics
    {
        bool Setup(void);
        void Shutdown(void);

        void ClearBuffer(void);
        void SwapBuffers(void);

        void SetMultisample(int samples);

        void SetBlendOp(BlendOp op);
        void SetBlendFunc(BlendFunc func);
        void SetBlendFunc(BlendFactor src, BlendFactor dst);

        void Scissor(float x, float y, float width, float height);
        void Viewport(float x, float y, float width, float height);

        void BindShader(ShaderHandle* shader);
        void BindTexture(TextureHandle* texture, int index = 0);
        void BindVertexArray(VertexArrayHandle* array);
        void BindIndexBuffer(IndexBufferHandle* buffer);
        void BindVertexBuffer(VertexBufferHandle* buffer);

        void BindRenderTarget(RenderTarget* renderTarget);
        void BlitRenderTarget(RenderTarget* src, RenderTarget* dst, ShaderHandle* shader);

        void DrawArrays(DrawType type, int count, int offset = 0);
        void DrawIndices(DrawType type, DataType dataType, int count, int offset = 0);

        void DrawArrays(DrawType type, ShaderHandle* shader, VertexArrayHandle* array, int count, int offset = 0);
        void DrawIndices(DrawType type, ShaderHandle* shader, VertexArrayHandle* array, const IndexBuffer& indices, int count, int offset = 0);

        void DrawArrays(DrawType type, ShaderHandle* shader, VertexArrayHandle* array, TextureHandle* texture, int count, int offset = 0);
        void DrawIndices(DrawType type, ShaderHandle* shader, VertexArrayHandle* array, TextureHandle* texture, const IndexBuffer& indices, int count, int offset = 0);
    }
}