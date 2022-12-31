namespace Mojo;

using OpenGL;

internal static class GLUtils
{
	public static void HandleError()
	{
	    GL.glGetError();
	}

	public static uint32 ConvertTextureWrap(TextureWrap wrap)
	{
	    switch (wrap)
	    {
	    case .Clamp:
	        return GL.GL_CLAMP_TO_EDGE;

	    case .Repeat:
	        return GL.GL_REPEAT;

	    case .MirrorClamp:
	        return GL.GL_CLAMP_TO_EDGE;

	    case .MirrorRepeat:
	        return GL.GL_MIRRORED_REPEAT;

		default:
			return 0;
	    }
	}

	public static uint32 ConvertTextureFilter(TextureFilter filter)
	{
	    switch (filter)
	    {
	    case .Linear:
	        return GL.GL_LINEAR;

	    case .Nearest:
	        return GL.GL_NEAREST;

		default:
			return 0;
	    }
	}

	public static uint32 ConvertPixelFormat(PixelFormat format)
	{
	    switch (format)
	    {
	    case .RGB:
	        return GL.GL_RGB;

	    case .RGBA:
	        return GL.GL_RGBA;

	    //case PixelFormat::Depth24Stencil8:
	    //    return GL_DEPTH24_STENCIL8;

		default:
			return 0;
	    }
	}
}