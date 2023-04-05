namespace Mojo;

using System;
using OpenGL;

public enum TextureFilter
{
	None,
	Linear,
	Nearest,
}

public enum TextureWrap
{
	None,
	Clamp,
	Repeat,
	MirrorClamp,
	MirrorRepeat,
}

public enum PixelFormat : uint32
{
	None,

	RGB,
	RGBA,
}


public class Texture
{
	private Handle			handle		= 0;
	private int32			width		= 0;
	private int32 			height		= 0;
	private PixelFormat		format		= .None;
	private TextureWrap		wrap		= .None;
	private TextureFilter	filter 		= .None;

	// Properties

	public Handle Handle
	{
		get => handle;
	}

	public int32 Width
	{
		get => width;
	}

	public int32 Height
	{
		get => height;
	}

	public PixelFormat Format
	{
		get => format;
	}

	public TextureWrap Wrap
	{
		get => wrap;
		set
		{
			wrap = value;

			int32 bindingTexture;
			GL.GetIntegerv(GL.TEXTURE_2D, out bindingTexture);

			uint32 wrapValue = GLUtils.ConvertTextureWrap(value);

			GL.BindTexture(GL.TEXTURE_2D, handle);
			GL.TexParameteri(GL.TEXTURE_2D, GL.TEXTURE_WRAP_S, wrapValue);
			GL.TexParameteri(GL.TEXTURE_2D, GL.TEXTURE_WRAP_T, wrapValue);
			GL.BindTexture(GL.TEXTURE_2D, (.)bindingTexture);

			HandleError();
		}
	}

	public TextureFilter Filter
	{
		get => filter;
		set
		{
			filter = value;

			int32 bindingTexture;
			GL.GetIntegerv(GL.TEXTURE_2D, out bindingTexture);

			uint32 filterValue = GLUtils.ConvertTextureFilter(value);
	
			GL.BindTexture(GL.TEXTURE_2D, handle);
			GL.TexParameteri(GL.TEXTURE_2D, GL.TEXTURE_WRAP_S, filterValue);
			GL.TexParameteri(GL.TEXTURE_2D, GL.TEXTURE_WRAP_T, filterValue);
			GL.TexParameteri(GL.TEXTURE_2D, GL.TEXTURE_MIN_FILTER, GL.LINEAR);
			GL.TexParameteri(GL.TEXTURE_2D, GL.TEXTURE_MIN_FILTER, GL.LINEAR);
			GL.BindTexture(GL.TEXTURE_2D, (.)bindingTexture);

			HandleError();
		}
	}

	// Constructors

	public static Result<Texture> Create()
	{
		uint32 handle = 0;
		GL.GenTextures(1, &handle);
		if (handle == 0)
		{
			return .Err;
		}

		int32 bindingTexture;
		GL.GetIntegerv(GL.TEXTURE_2D, out bindingTexture);

		GL.BindTexture(GL.TEXTURE_2D, handle);
		GL.TexParameteri(GL.TEXTURE_2D, GL.TEXTURE_WRAP_S, GL.REPEAT);
		GL.TexParameteri(GL.TEXTURE_2D, GL.TEXTURE_WRAP_T, GL.REPEAT);
		GL.TexParameteri(GL.TEXTURE_2D, GL.TEXTURE_MIN_FILTER, GL.LINEAR);
		GL.TexParameteri(GL.TEXTURE_2D, GL.TEXTURE_MIN_FILTER, GL.LINEAR);
		GL.BindTexture(GL.TEXTURE_2D, (.)bindingTexture);

		Texture texture = new Texture() {
			handle 	= handle,
			wrap 	= .Repeat,
			filter 	= .Linear
		};

		return .Ok(texture);
	}

	public static void Destroy(Texture texture)
	{
		if (texture != null)
		{
			GL.DeleteTextures(1, &texture.handle);
			delete texture;
		}
	}

	// Methods

	public void UpdatePixels(int32 width, int32 height, PixelFormat format, void* pixels, PixelFormat targetFormat = .RGBA)
	{
		if (width <= 0 || height <= 0)
		{
		    return;
		}

		this.width  = width;
		this.height = height;
		this.format = targetFormat;
		
		int32 bindingTexture;
		GL.GetIntegerv(GL.TEXTURE_2D, out bindingTexture);

		GL.BindTexture(GL.TEXTURE_2D, handle);
		GL.TexImage2D(GL.TEXTURE_2D, 0, ConvertPixelFormat(targetFormat), width, height, 0, ConvertPixelFormat(format), GL.UNSIGNED_BYTE, pixels);
		GL.BindTexture(GL.TEXTURE_2D, (.)bindingTexture);

		HandleError();
	}

	// Utils

	[Inline]
	private static void HandleError()
	{
	    GL.GetError();
	}

	[Inline]
	private static uint ConvertTextureWrap(TextureWrap wrap)
	{
	    switch (wrap)
	    {
	    case .Clamp:
	        return GL.CLAMP_TO_EDGE;

	    case .Repeat:
	        return GL.REPEAT;

	    case .MirrorClamp:
	        return GL.CLAMP_TO_EDGE; // HACKS

	    case .MirrorRepeat:
	        return GL.MIRRORED_REPEAT;

		default:
			return 0;
	    }
	}

	[Inline]
	private static uint ConvertTextureFilter(TextureFilter filter)
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

	[Inline]
	private static uint ConvertPixelFormat(PixelFormat format)
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