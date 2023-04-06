namespace Mojo;

using System;

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
	        return GL.CLAMP_TO_EDGE; // Hack

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

	public static uint CreateGLShader(uint type, StringView src)
    {
        let shader = GL.CreateShader(type);
        if (shader == 0)
        {
			HandleError();
            return 0;
        }

		let srcCstr = src.Ptr;
		let srcLength = (int32)src.Length;

        GL.ShaderSource(shader, 1, &srcCstr, &srcLength);
        GL.CompileShader(shader);

        int32 status;
        GL.GetShaderiv(shader, GL.COMPILE_STATUS, out status);
        if (status == 0)
        {
            String errorLog = scope String(1024);
            GL.GetShaderInfoLog(shader, errorLog.Length, null, errorLog.Ptr);

            GL.DeleteShader(shader);
            return 0;
        }
        else
        {
            return shader;
        }
    }

    public static uint CreateGLProgram(uint vshader, uint fshader)
    {
        if (vshader == 0 || fshader == 0)
        {
            return 0;
        }

        let program = GL.CreateProgram();
        if (program == 0)
        {
			HandleError();
            return 0;
        }

        GL.AttachShader(program, vshader);
        GL.AttachShader(program, fshader);
        GL.LinkProgram(program);

        int32 status;
        GL.GetProgramiv(program, GL.LINK_STATUS, out status);
        if (status == 0)
        {
            String errorLog = scope String(1024);
            GL.GetProgramInfoLog(program, errorLog.Length, null, errorLog.Ptr);
            GL.DeleteProgram(program);
            return 0;
        }
        else
        {
            return program;
        }
    }
}