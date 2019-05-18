#pragma once 

inline namespace Mojo
{
    struct TextureHandle;

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
    };

    // Graphics texture 2D
    // @note: no oop support, so Texture tmp = TextureCube() is illegal.
    struct Texture final
    {
        unsigned handle = 0;
        float    width  = 0;
        float    height = 0;
        
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

        inline operator TextureHandle*(void) const
        {
            return (TextureHandle*)(long long)handle;
        }
    };
}