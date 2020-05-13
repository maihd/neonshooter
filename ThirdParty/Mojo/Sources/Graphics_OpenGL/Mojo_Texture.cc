#include <stdint.h>
#include <assert.h>
#include <GL/glew.h>

#include <Mojo/Graphics/Texture.h>

namespace Mojo
{
    namespace
    {
        static void HandleError()
        {
            glGetError();
        }

        static GLenum ConvertTextureWrap(TextureWrap wrap)
        {
            switch (wrap)
            {
            case TextureWrap::Clamp:
                return GL_CLAMP_TO_EDGE;

            case TextureWrap::Repeat:
                return GL_REPEAT;

            case TextureWrap::MirrorClamp:
                return GL_MIRROR_CLAMP_TO_EDGE;

            case TextureWrap::MirrorRepeat:
                return GL_MIRRORED_REPEAT;
            }

            return 0;
        }

        static GLenum ConvertTextureFilter(TextureFilter filter)
        {
            switch (filter)
            {
            case TextureFilter::Linear:
                return GL_LINEAR;

            case TextureFilter::Nearest:
                return GL_NEAREST;
            }

            return 0;
        }

        static GLenum ConvertPixelFormat(PixelFormat format)
        {
            switch (format)
            {
            case PixelFormat::RGB:
                return GL_RGB;

            case PixelFormat::RGBA:
                return GL_RGBA;

            //case PixelFormat::Depth24Stencil8:
            //    return GL_DEPTH24_STENCIL8;
            }

            return 0;
        }
    }

    Texture Texture::Create(void)
    {
        Texture texture;
        glGenTextures(1, (GLuint*)&texture.handle);

        GLint bindingTexture;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &bindingTexture);

        glBindTexture(GL_TEXTURE_2D, texture.handle);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, (GLuint)bindingTexture);

        return texture;
    }

    void Texture::Destroy(Texture& texture)
    {
        glDeleteTextures(1, (GLuint*)&texture.handle);

        texture = {}; // Clear all memory
    }

    void Texture::SetWrap(TextureWrap wrap)
    {
        GLint bindingTexture;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &bindingTexture);

        glBindTexture(GL_TEXTURE_2D, handle);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, ConvertTextureWrap(wrap));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, ConvertTextureWrap(wrap));
        glBindTexture(GL_TEXTURE_2D, (GLuint)bindingTexture);

        HandleError();
    }

    void Texture::SetFilter(TextureFilter filter)
    {
        glTextureParameteri(handle, GL_TEXTURE_MIN_FILTER, ConvertTextureFilter(filter));
        glTextureParameteri(handle, GL_TEXTURE_MAG_FILTER, ConvertTextureFilter(filter));

        HandleError();
    }

    void Texture::SetPixels(int w, int h, PixelFormat format, const void* pixels, PixelFormat targetFormat)
    {
        if (w <= 0 || h <= 0)
        {
            return;
        }

        this->width  = (float)w;
        this->height = (float)h;
        this->format = targetFormat;

        glTextureImage2DEXT(handle, GL_TEXTURE_2D, 0, ConvertPixelFormat(targetFormat), w, h, 0, ConvertPixelFormat(format), GL_UNSIGNED_BYTE, pixels);
        HandleError();
    }
}