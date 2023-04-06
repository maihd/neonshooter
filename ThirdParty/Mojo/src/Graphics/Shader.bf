namespace Mojo;

using System;

class Shader
{
	// Default sources

	public const char8* DefaultEffectVertexSource =
		"""
			#version 330 core

			layout (location = 0) in vec4 vertex;
			out vec2 VertexTexCoord;

			void main()
			{
				VertexTexCoord = vertex.zw;
				gl_Position = vec4(vertex.xy, 0, 1.0);
			}
		""";

	public const char8* DefaultVertexSource =
		"""
			#version 330 core
			layout (location = 0) in vec3 position;
			layout (location = 1) in vec2 texcoord;
			layout (location = 2) in vec4 color;
	
			out     vec2 VaryingTexCoord;
			out     vec4 VaryingColor;
	
			uniform vec4 MainColor;
			uniform mat4 TransformMatrix;
			uniform mat4 ProjectionMatrix;
	
			vec4 VertexPosition;
			vec2 VertexTexCoord;
			vec4 VertexColor;
	
			void main()
			{
			    VertexPosition  = vec4(position, 1.0);
			    VertexTexCoord  = texcoord;
			    VertexColor     = color;
		
			    VaryingTexCoord = VertexTexCoord;
			    VaryingColor    = MainColor * VertexColor;
			    
			    gl_Position     = ProjectionMatrix * TransformMatrix * VertexPosition;
			}
		""";

	public const char8* DefaultPixelsSource =
	    """
			#version 330 core
	
			in      vec2       VaryingTexCoord;
			in      vec4       VaryingColor;
	
			uniform vec4       MainColor;
			uniform sampler2D  MainTexture;
	
			out     vec4       ResultColor;
	
			void main()
			{
				ResultColor = texture(MainTexture, VaryingTexCoord) * VaryingColor;
			}
		""";

	// Type structure

	Handle handle = 0;

	public this()
	{
	}

	public this(StringView? vertexSource = null, StringView? pixelsSource = null)
	{
		Create(vertexSource, pixelsSource);
	}

	public ~this()
	{
		if (handle != 0)
		{
			Destroy();
		}
	}

	// Main functions

	public int FindAttribute(StringView name)
	{
		return GL.GetAttribLocation(handle, name.Ptr);
	}

	public int FindUniform(StringView name)
	{
		return GL.GetUniformLocation(handle, name.Ptr);
	}

	public bool SetFloat(int location, float x)
	{
		if (location < 0)
		{
		    return false;
		}

		GL.Uniform1f(location, x);
		//glProgramUniform1f(handle, location, x);
		return true;
	}

	public bool SetFloat(StringView name, float x)
	{
		return SetFloat(FindUniform(name), x);
	}

	public bool SetFloat2(int location, float x, float y)
	{
		if (location < 0)
		{
		    return false;
		}

		GL.Uniform2f(location, x, y);
		//glProgramUniform2f(handle, location, x, y);
		return true;
	}

	public bool SetFloat2(StringView name, float x, float y)
	{
		return SetFloat2(FindUniform(name), x, y);
	}

	public bool SetVector3(int location, float x, float y, float z)
	{
		if (location < 0)
		{
		    return false;
		}

		GL.Uniform3f(location, x, y, z);
		//glProgramUniform3f(handle, location, x, y, z);
		return true;
	}

	public bool SetVector3(StringView name, float x, float y, float z)
	{
		return SetVector3(FindUniform(name), x, y, z);
	}

	public bool SetVector4(int location, float x, float y, float z, float w)
	{
		if (location < 0)
		{
		    return false;
		}

		GL.Uniform4f(location, x, y, z, w);
		//glProgramUniform4f(handle, location, x, y, z, w);
		return true;
	}

	public bool SetVector4(StringView name, float x, float y, float z, float w)
	{
		return SetVector4(FindUniform(name), x, y, z, w);
	}

	public bool SetMatrix4(int location, float* value, bool transpose = false)
	{
		if (location < 0)
		{
		    return false;
		}

		GL.UniformMatrix4fv(location, 1, transpose ? 1 : 0, value);
		//glProgramUniformMatrix4fv(handle, location, 1, transpose, value);
		return true;
	}

	public bool SetMatrix4(String name, float* value, bool transpose = false)
	{
		return SetMatrix4(FindUniform(name), value, transpose);
	}

	// Graphics default parameters

	public bool SetTransform(ref Matrix4 matrix)
	{
        return SetMatrix4("TransformMatrix", (float*)&matrix, false);
	}

	public bool SetProjection(ref Matrix4 matrix)
	{
		return SetMatrix4("ProjectionMatrix", (float*)&matrix, false);
	}

	public bool SetMainColor(Vector4 color)
	{
		return SetVector4("MainColor", color.x, color.y, color.z, color.w);
	}

	public bool SetMainTexture(Texture texture)
	{
		return true;
	}

	//-------------------------------------------------
	// Create and destroy
	//-------------------------------------------------

	public bool Create(StringView? inVertexSource = null, StringView? inPixelsSource = null)
	{
		let vertexSource = inVertexSource != null ? inVertexSource?.Ptr : DefaultVertexSource;
		let pixelsSource = inPixelsSource != null ? inPixelsSource?.Ptr : DefaultPixelsSource;

		let vshader = GLUtils.CreateGLShader(GL.VERTEX_SHADER, vertexSource);
		if (vshader == 0)
		{
		    return false;
		}

		let fshader = GLUtils.CreateGLShader(GL.FRAGMENT_SHADER, pixelsSource);
		if (fshader == 0)
		{
		    return false;
		}

		let program = GLUtils.CreateGLProgram(vshader, fshader);
		if (program == 0)
		{
			return false;
		}

		handle = (.)program;
		return true;
	}

	public bool CreateEffect(StringView pixelsSource, StringView? vertexSource = DefaultEffectVertexSource)
	{
		return Create(vertexSource, pixelsSource);
	}

	public void Destroy()
	{
		GL.DeleteProgram(handle);
		handle = 0;
	}
}