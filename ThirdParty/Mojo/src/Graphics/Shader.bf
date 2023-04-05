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

	// Main functions

	Handle handle = 0;

	public int FindAttribute(StringView name)
	{
		return 0;
	}

	public int FindUniform(StringView name)
	{
		return 0;
	}

	public bool SetFloat(int location, float x)
	{
		return false;
	}

	public bool SetFloat(StringView name, float x)
	{
		return false;
	}

	public bool SetFloat2(int location, float x, float y)
	{
		return false;
	}

	public bool SetFloat2(StringView name, float x, float y)
	{
		return false;
	}

	public bool SetVector3(int location, float x, float y, float z)
	{
		return false;
	}

	public bool SetVector3(StringView name, float x, float y, float z)
	{
		return false;
	}

	public bool SetVector4(int location, float x, float y, float z, float w)
	{
		return false;
	}

	public bool SetVector4(StringView name, float x, float y, float z, float w)
	{
		return false;
	}

	public bool SetMatrix4(int location, float* value, bool transpose = false)
	{
		return false;
	}

	public bool SetMatrix4(String name, float* value, bool transpose = false)
	{
		return false;
	}

	// Graphics default parameters

	public bool SetTransform(Matrix4 matrix)
	{
		return false;
	}

	public bool SetProjection(Matrix4 matrix)
	{
		return false;
	}

	public bool SetMainColor(Vector4 color)
	{
		return false;
	}

	public bool SetMainTexture(Texture texture)
	{
		return false;
	}

	//-------------------------------------------------
	// Create and destroy shaders
	//-------------------------------------------------

	static Shader Create(StringView? vertexSource = null, StringView? pixelsSource = null)
	{
		return null;
	}

	static void   Destroy(Shader shader)
	{
	}

	static Shader CreateEffect(StringView pixelsSource)
	{
		return null;
	}
}