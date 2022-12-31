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
			GL.glGetIntegerv(GL.GL_TEXTURE_2D, out bindingTexture);

			uint32 wrapValue = GLUtils.ConvertTextureWrap(value);

			GL.glBindTexture(GL.GL_TEXTURE_2D, handle);
			GL.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_WRAP_S, wrapValue);
			GL.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_WRAP_T, wrapValue);
			GL.glBindTexture(GL.GL_TEXTURE_2D, (.)bindingTexture);
		}
	}

	public TextureFilter Filter
	{
		get => filter;
		set
		{
			filter = value;

			int32 bindingTexture;
			GL.glGetIntegerv(GL.GL_TEXTURE_2D, out bindingTexture);

			uint32 filterValue = GLUtils.ConvertTextureFilter(value);
	
			GL.glBindTexture(GL.GL_TEXTURE_2D, handle);
			GL.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_WRAP_S, filterValue);
			GL.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_WRAP_T, filterValue);
			GL.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_MIN_FILTER, GL.GL_LINEAR);
			GL.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_MIN_FILTER, GL.GL_LINEAR);
			GL.glBindTexture(GL.GL_TEXTURE_2D, (.)bindingTexture);
		}
	}

	// Constructors

	public static Result<Texture> Create()
	{
		uint32 handle = 0;
		GL.glGenTextures(1, &handle);
		if (handle == 0)
		{
			return .Err;
		}

		int32 bindingTexture;
		GL.glGetIntegerv(GL.GL_TEXTURE_2D, out bindingTexture);

		GL.glBindTexture(GL.GL_TEXTURE_2D, handle);
		GL.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_WRAP_S, GL.GL_REPEAT);
		GL.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_WRAP_T, GL.GL_REPEAT);
		GL.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_MIN_FILTER, GL.GL_LINEAR);
		GL.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_MIN_FILTER, GL.GL_LINEAR);
		GL.glBindTexture(GL.GL_TEXTURE_2D, (.)bindingTexture);

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
			GL.glDeleteTextures(1, &texture.handle);
			delete texture;
		}
	}

	// Methods

	public void UpdatePixels(int32 width, int32 height, PixelFormat format, void* pixels, PixelFormat targetFormat = .RGBA)
	{

	}
}