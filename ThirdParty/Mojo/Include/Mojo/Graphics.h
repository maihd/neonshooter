#pragma once

#include <Mojo/Core/Types.h>
#include <Mojo/Graphics/Texture.h>

inline namespace Mojo
{
    struct Shader;
    struct Texture;
    struct IndexBuffer;
    struct VertexArray;
    struct VertexBuffer;
    struct RenderTarget;

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

    enum struct DrawMode
    {
        Fill,
        Line,
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

        I8,
        I16,
        I32,
        I64,

        U8,
        U16,
        U32,
        U64,

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

    namespace BlendModes
    {
        constexpr BlendFunc Alpha = { BlendFactor::SrcAlpha, BlendFactor::InvertSrcAlpha };
        constexpr BlendFunc Additive = { BlendFactor::SrcAlpha, BlendFactor::One };
    };

    struct IndexBuffer
    {
        Handle handle = 0;

        static IndexBuffer Create(void);
        static void        Destroy(IndexBuffer& buffer);

        void SetData(const void* data, int size, BufferUsage usage);

        void SetBlendOp(BlendOp op);
        void SetBlendFunc(BlendFunc func);
        void SetBlendFunc(BlendFactor src, BlendFactor dst);

        inline void SetData(const int indices[], int count, BufferUsage usage)
        {
            this->SetData(indices, count * sizeof(indices[0]), usage);
        }
    };

    struct VertexBuffer
    {
        Handle handle = 0;

        static VertexBuffer Create(void);
        static void         Destroy(VertexBuffer& buffer);

        void SetData(const void* data, int size, BufferUsage usage);

        void SetBlendOp(BlendOp op);
        void SetBlendFunc(BlendFunc func);
        void SetBlendFunc(BlendFactor src, BlendFactor dst);
    };

    struct VertexArray
    {
        Handle handle = 0;

        static VertexArray Create(void);
        static void        Destroy(VertexArray& buffer);

        void SetAttribute(const VertexBuffer& buffer, int location, int size, DataType dataType, bool normalized, int stride, int offset = 0);
    };

    struct RenderTarget
    {
        Texture texture         = {};
        
        Handle  frameBuffer     = 0;
        Handle  renderBuffer    = 0;

        //static RenderTarget Create(void);
        static RenderTarget Create(float width, float height);
        static void         Destroy(RenderTarget& renderTarget);
    };

    struct GraphicsSettings
    {
        int multisamples = 1;
    };

    namespace Graphics
    {
        bool Setup(const GraphicsSettings& settings = GraphicsSettings());
        void Shutdown(void);

        void Clear(void);
        void Present(void);

        void SetClearColor(float r, float g, float b, float a = 1.0f);
        void SetMultisample(int samples);

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

        void BindRenderTarget(const RenderTarget& renderTarget);
        void BlitRenderTarget(const RenderTarget& src, RenderTarget& dst, const Shader& shader);
        void BlitRenderTarget(const RenderTarget& src, const Shader& shader);

        void DrawArrays(DrawType type, int count, int offset = 0);
        void DrawIndices(DrawType type, DataType dataType, int count, int offset = 0);

        void SetColor(unsigned color24, float a = 1.0f);
        void SetColor(float r, float g, float b, float a = 1.0f);

        Vector4 GetColor(void);
        void    GetColor(float* r, float* g, float* b, float* a = 0);

        void Arc(DrawMode mode, float x, float y, float radius, float angle, int segments = 30);
        void Circle(DrawMode mode, float x, float y, float radius, int segments = 30);
        void Polygon(DrawMode mode, const float* points);
        void Rectangle(DrawMode mode, float x, float y, float w, float h);
    }
}