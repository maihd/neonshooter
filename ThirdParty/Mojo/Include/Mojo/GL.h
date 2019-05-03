#pragma once 

inline namespace Mojo
{
    struct GraphicsSettings
    {
        int redBits;
        int greenBits;
        int blueBits;
        int alphaBits;
        int depthBits;
        int stencilBits;

        int multisamples;

        constexpr GraphicsSettings(void)
            : redBits(8)
            , greenBits(8)
            , blueBits(8)
            , alphaBits(8)
            , depthBits(24)
            , stencilBits(8)
            , multisamples(1)
        {
        }
    };

    namespace __ClearFlag
    {
        enum Type
        {
            Color   = 1 << 0,
            Depth   = 1 << 1,
            Stencil = 1 << 2,
        };
    };
    using ClearFlag = __ClearFlag::Type;

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

    enum struct TextureFilter
    {
        Linear,
        Nearest,
    };

    enum struct TextureWrap
    {
        Clamp,
        Repeat,
        MirrorClamp,
        MirrorRepeat,
    };

    enum struct PixelFormat
    {
        RGB,
        RGBA,
        Depth24Stencil8,
    };

    enum struct GraphicsMode
    {
        Blend,
        Depth,
        Stencil,
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

    struct Shader
    {
        unsigned _handle = 0;

        static Shader Create(const char* vertexSource, const char* pixelsSource);
        static void   Destroy(Shader& shader);

        int FindAttribute(const char* name);
        int FindUniform(const char* name);

        bool SetFloat(int location, float x);
        bool SetFloat(const char* name, float x);
        
        bool SetFloat2(int location, float x, float y);
        bool SetFloat2(const char* name, float x, float y);
        
        bool SetFloat3(int location, float x, float y, float z);
        bool SetFloat3(const char* name, float x, float y, float z);
        
        bool SetFloat4(int location, float x, float y, float z, float w);
        bool SetFloat4(const char* name, float x, float y, float z, float w);

        bool SetFloat4x4(int location, const float* value, bool transpose = false);
        bool SetFloat4x4(const char* name, const float* value, bool transpose = false);
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

    struct Texture
    {
        unsigned _handle = 0;

        int      width  = 0;
        int      height = 0;
        
        static Texture   Create(void);
        static void      Destroy(Texture& texture);
    
        void      SetWrap(TextureWrap wrap);
        void      SetWrap(TextureWrap wrapU, TextureWrap wrapV);

        void      SetWrapU(TextureWrap wrap);
        void      SetWrapV(TextureWrap wrap);

        void      SetFilter(TextureFilter filter);
        void      SetFilter(TextureFilter minFilter, TextureFilter magFilter);

        void      SetMinFilter(TextureFilter minFilter);
        void      SetMagFilter(TextureFilter magFilter);

        void      SetPixels(int width, int height, PixelFormat format, const void* pixels, PixelFormat targetFormat = PixelFormat::RGBA);
    };

    struct RenderTarget
    {
        Texture      _texture;
        VertexArray  _vertexArray;
        VertexBuffer _vertexBuffer;

        unsigned _frameBuffer  = 0;
        unsigned _renderBuffer = 0;
        int      width         = 0;
        int      height        = 0;

        //static RenderTarget Create(void);
        static RenderTarget Create(int width, int height, PixelFormat pixelFormat = PixelFormat::Depth24Stencil8);
        static void         Destroy(RenderTarget& renderTarget);

        void Clear(int flags = ClearFlag::Color);
        void Present(const Shader& shader);
    };

    namespace GL
    {
        bool Setup(const GraphicsSettings& settings = GraphicsSettings());
        void Shutdown(void);

        void ClearBuffer(int flags = ClearFlag::Color);
        void SwapBuffers(void);

        void Enable(GraphicsMode mode);
        void Disable(GraphicsMode mode);
        bool IsEnabled(GraphicsMode mode);

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

        void DrawArrays(DrawType type, int count, int offset = 0);
        void DrawIndices(DrawType type, DataType dataType, int count, int offset = 0);

        void DrawArrays(DrawType type, const Shader& shader, const VertexArray& array, int count, int offset = 0);
        void DrawIndices(DrawType type, const Shader& shader, const VertexArray& array, const IndexBuffer& indices, int count, int offset = 0);

        void DrawArrays(DrawType type, const Shader& shader, const VertexArray& array, const Texture& texture, int count, int offset = 0);
        void DrawIndices(DrawType type, const Shader& shader, const VertexArray& array, const Texture& texture, const IndexBuffer& indices, int count, int offset = 0);
    }
}