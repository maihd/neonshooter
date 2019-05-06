#pragma once

inline namespace Mojo
{
    struct Shader;
    struct Texture;

    struct GraphicsSettings
    {
        int multisamples;

        constexpr GraphicsSettings(void)
            : multisamples(1)
        {
        }
    };

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
        unsigned _handle   = 0;
        DataType _dataType = DataType::None;

        static IndexBuffer Create(void);
        static void        Destroy(IndexBuffer& buffer);

        void SetData(const void* data, int size, DataType dataType, BufferUsage usage);

        void SetBlendOp(BlendOp op);
        void SetBlendFunc(BlendFunc func);
        void SetBlendFunc(BlendFactor src, BlendFactor dst);
    };

    struct VertexBuffer
    {
        unsigned _handle = 0;

        static VertexBuffer Create(void);
        static void         Destroy(VertexBuffer& buffer);

        void SetData(const void* data, int size, BufferUsage usage);

        void SetBlendOp(BlendOp op);
        void SetBlendFunc(BlendFunc func);
        void SetBlendFunc(BlendFactor src, BlendFactor dst);
    };

    struct VertexArray
    {
        unsigned _handle = 0;

        static VertexArray Create(void);
        static void        Destroy(VertexArray& buffer);

        void SetAttribute(const VertexBuffer& buffer, int location, int size, DataType dataType, bool normalized, int stride, int offset = 0);
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
        bool Setup(const GraphicsSettings& settings = GraphicsSettings());
        void Shutdown(void);

        void ClearBuffer(void);
        void SwapBuffers(void);

        void SetBlendOp(BlendOp op);
        void SetBlendFunc(BlendFunc func);
        void SetBlendFunc(BlendFactor src, BlendFactor dst);

        void Scissor(float x, float y, float width, float height);
        void Viewport(float x, float y, float width, float height);

        void BindShader(const Shader& shader);
        void BindTexture(const Texture& texture, int index = 0);
        void BindVertexArray(const VertexArray& array);
        void BindIndexBuffer(const IndexBuffer& buffer);
        void BindVertexBuffer(const VertexBuffer& buffer);

        void BindRenderTarget(RenderTarget* renderTarget);
        void BlitRenderTarget(RenderTarget* src, RenderTarget* dst, const Shader& shader);

        void DrawArrays(DrawType type, int count, int offset = 0);
        void DrawIndices(DrawType type, DataType dataType, int count, int offset = 0);

        void DrawArrays(DrawType type, const Shader& shader, const VertexArray& array, int count, int offset = 0);
        void DrawIndices(DrawType type, const Shader& shader, const VertexArray& array, const IndexBuffer& indices, int count, int offset = 0);

        void DrawArrays(DrawType type, const Shader& shader, const VertexArray& array, const Texture& texture, int count, int offset = 0);
        void DrawIndices(DrawType type, const Shader& shader, const VertexArray& array, const Texture& texture, const IndexBuffer& indices, int count, int offset = 0);
    }
}