#pragma once

#include <Mojo/Core/Array.h>

inline namespace Mojo
{
    struct Font;
    struct FontAtlas;
    struct FontGlyph;
    struct TextureHandle;

    struct FontGlyph
    {
        int   code;
        float advanceX;
        float x0, y0, x1, y1;
        float u0, v0, u1, v1;
    };

    struct FontAtlas
    {
        int              flags;

        TextureHandle*   texture;
        int              textureDesiredWidth;
        int              textureGlyphPadding;

        void*            pixelsAlpha8;
        void*            pixelsRGBA32;

        float            scaleU;
        float            scaleV;
        float            whitePixelU;
        float            whitePixelV;

        Array<Font*>     fonts;

        FontAtlas(void);
        ~FontAtlas(void);
    };

    struct Font
    {
        Array<int>       indexLookup;
        Array<FontGlyph> glyphs;

        FontAtlas*       atlas;
        int              fallbackChar;

        Font(void);
        ~Font(void);
    };
}