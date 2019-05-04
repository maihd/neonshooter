#pragma once 

inline namespace Mojo
{
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
}