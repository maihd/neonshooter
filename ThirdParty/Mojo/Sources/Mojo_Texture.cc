#include <stdint.h>
#include <assert.h>
#include <GL/glew.h>

#include <Mojo/Texture.h>

inline namespace Mojo
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
        glGenTextures(1, (GLuint*)&texture._handle);

        GLint bindingTexture;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &bindingTexture);

        glBindTexture(GL_TEXTURE_2D, texture._handle);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, (GLuint)bindingTexture);

        return texture;
    }

    void Texture::Destroy(Texture& texture)
    {
        ::glDeleteTextures(1, &texture._handle);

        texture._handle = 0;
    }

    void Texture::SetWrap(TextureWrap wrap)
    {
        this->SetWrap(wrap, wrap);
    }

    void Texture::SetWrap(TextureWrap wrapU, TextureWrap wrapV)
    {
        GLint bindingTexture;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &bindingTexture);

        glBindTexture(GL_TEXTURE_2D, _handle);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, ConvertTextureWrap(wrapU));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, ConvertTextureWrap(wrapV));
        glBindTexture(GL_TEXTURE_2D, (GLuint)bindingTexture);

        //glTextureParameteri(_handle, GL_TEXTURE_WRAP_S, ConvertTextureWrap(wrapU));
        //glTextureParameteri(_handle, GL_TEXTURE_WRAP_T, ConvertTextureWrap(wrapV));

        HandleError();
    }

    void Texture::SetWrapU(TextureWrap wrapU)
    {
        GLint bindingTexture;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &bindingTexture);

        glBindTexture(GL_TEXTURE_2D, _handle);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, ConvertTextureWrap(wrapU));
        glBindTexture(GL_TEXTURE_2D, (GLuint)bindingTexture);

        //glTextureParameteri(_handle, GL_TEXTURE_WRAP_S, ConvertTextureWrap(wrapU));
        //glTextureParameteri(_handle, GL_TEXTURE_WRAP_T, ConvertTextureWrap(wrapV));

        HandleError();
    }

    void Texture::SetWrapV(TextureWrap wrapV)
    {
        GLint bindingTexture;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &bindingTexture);

        glBindTexture(GL_TEXTURE_2D, _handle);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, ConvertTextureWrap(wrapV));
        glBindTexture(GL_TEXTURE_2D, (GLuint)bindingTexture);

        //glTextureParameteri(_handle, GL_TEXTURE_WRAP_S, ConvertTextureWrap(wrapU));
        //glTextureParameteri(_handle, GL_TEXTURE_WRAP_T, ConvertTextureWrap(wrapV));

        HandleError();
    }

    void Texture::SetFilter(TextureFilter filter)
    {
        this->SetFilter(filter, filter);
    }

    void Texture::SetFilter(TextureFilter minFilter, TextureFilter magFilter)
    {
        glTextureParameteri(_handle, GL_TEXTURE_MIN_FILTER, ConvertTextureFilter(minFilter));
        glTextureParameteri(_handle, GL_TEXTURE_MAG_FILTER, ConvertTextureFilter(magFilter));

        HandleError();
    }

    void Texture::SetMinFilter(TextureFilter minFilter)
    {
        glTextureParameteri(_handle, GL_TEXTURE_MIN_FILTER, ConvertTextureFilter(minFilter));
    }

    void Texture::SetMagFilter(TextureFilter magFilter)
    {
        glTextureParameteri(_handle, GL_TEXTURE_MAG_FILTER, ConvertTextureFilter(magFilter));
    }

    void Texture::SetPixels(int w, int h, PixelFormat format, const void* pixels, PixelFormat targetFormat)
    {
        if (w <= 0 || h <= 0)
        {
            return;
        }

        this->width = w;
        this->height = h;

        glTextureImage2DEXT(_handle, GL_TEXTURE_2D, 0, ConvertPixelFormat(targetFormat), w, h, 0, ConvertPixelFormat(format), GL_UNSIGNED_BYTE, pixels);
        HandleError();
    }
}