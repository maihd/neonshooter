namespace Mojo;

using OpenGL;

internal static class GLUtils
{
	public static void HandleError()
	{
	    GL.GetError();
	}

	public static uint32 ConvertTextureWrap(TextureWrap wrap)
	{
	    switch (wrap)
	    {
	    case .Clamp:
	        return GL.CLAMP_TO_EDGE;

	    case .Repeat:
	        return GL.REPEAT;

	    case .MirrorClamp:
	        return GL.CLAMP_TO_EDGE;

	    case .MirrorRepeat:
	        return GL.MIRRORED_REPEAT;

		default:
			return 0;
	    }
	}

	public static uint32 ConvertTextureFilter(TextureFilter filter)
	{
	    switch (filter)
	    {
	    case .Linear:
	        return GL.LINEAR;

	    case .Nearest:
	        return GL.NEAREST;

		default:
			return 0;
	    }
	}

	public static uint32 ConvertPixelFormat(PixelFormat format)
	{
	    switch (format)
	    {
	    case .RGB:
	        return GL.RGB;

	    case .RGBA:
	        return GL.RGBA;

	    //case PixelFormat::Depth24Stencil8:
	    //    return GL_DEPTH24_STENCIL8;

		default:
			return 0;
	    }
	}
}