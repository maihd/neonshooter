#pragma once 

#include <Mojo/Core/Types.h>

inline namespace Mojo
{
    enum struct TextureFilter
    {
        None,
        Linear,
        Nearest,
    };

    enum struct TextureWrap
    {
        None,
        Clamp,
        Repeat,
        MirrorClamp,
        MirrorRepeat,
    };

    enum struct PixelFormat
    {
        None,

        RGB,
        RGBA,
    };

    // Graphics texture 2D
    // @note: no oop support, so Texture tmp = TextureCube() is illegal.
    struct Texture
    {
        Handle          handle  = 0;
        float           width   = 0;
        float           height  = 0;
        PixelFormat     format  = PixelFormat::None;
        TextureWrap     wrap    = TextureWrap::None;
        TextureFilter   filter  = TextureFilter::None;
        
        static Texture   Create(void);
        static void      Destroy(Texture& texture);
    
        void      SetWrap(TextureWrap wrap);
        void      SetFilter(TextureFilter filter);
        void      SetPixels(int width, int height, PixelFormat format, const void* pixels, PixelFormat targetFormat = PixelFormat::RGBA);
    };
}
