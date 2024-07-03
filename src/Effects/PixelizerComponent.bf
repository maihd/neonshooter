namespace NeonShooter;

using Raylib;

static struct PixelizerComponent
{
	public const char8* ShaderSource =
		"""
			#version 330
	
			// Input vertex attributes (from vertex shader)
			in vec2 fragTexCoord;
			in vec4 fragColor;
	
			// Input uniform values
			uniform sampler2D texture0;
			uniform vec4 colDiffuse;
	
			// Output fragment color
			out vec4 finalColor;
	
			// NOTE: Add here your custom variables
	
			// NOTE: Render size values must be passed from code
			uniform float pixelWidth = 1.1;
			uniform float pixelHeight = 1.1;
	
			void main()
			{
				ivec2 size = textureSize(texture0, 0);
				float renderWidth = size.x;
				float renderHeight = size.y;

			    float dx = pixelWidth*(1.0/renderWidth);
			    float dy = pixelHeight*(1.0/renderHeight);
	
			    vec2 coord = vec2(dx*floor(fragTexCoord.x/dx), dy*floor(fragTexCoord.y/dy));
	
			    vec3 tc = texture(texture0, coord).rgb;
	
			    finalColor = vec4(tc, 1.0);
			}	
		""";

	private static Shader			shader;
	private static RenderTexture 	frameBuffer;

	public static void Load()
	{
		shader = LoadShaderFromMemory(null, ShaderSource);
		frameBuffer = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
	}

	public static void Unload()
	{
		UnloadShader(shader);
		shader = default;

		UnloadRenderTexture(frameBuffer);
		frameBuffer = default;
	}

	public static void Begin(Color clearColor)
	{
		BeginTextureMode(frameBuffer);
		ClearBackground(clearColor);
	}

	public static void End(bool noEffect = false)
	{
		EndTextureMode();

		if (!noEffect)
		{
			BeginShaderMode(shader);
			DrawTextureRec(frameBuffer.texture, .(0, 0, GetScreenWidth(), -GetScreenHeight()), .Zero, .WHITE);
			EndShaderMode();
		}
		else
		{
			DrawTextureRec(frameBuffer.texture, .(0, 0, GetScreenWidth(), -GetScreenHeight()), .Zero, .WHITE);
		}
	}

	public static void Draw(RenderTexture frameBuffer, bool noEffect)
	{
		if (!noEffect)
		{
			BeginShaderMode(shader);
			DrawTextureRec(frameBuffer.texture, .(0, 0, GetScreenWidth(), -GetScreenHeight()), .Zero, .WHITE);
			EndShaderMode();
		}
		else
		{
			DrawTextureRec(frameBuffer.texture, .(0, 0, GetScreenWidth(), -GetScreenHeight()), .Zero, .WHITE);
		}
	}
}