namespace NeonShooter;

using Raylib;

static struct BloomComponent
{
	public const char8* BloomShaderSource =
		"""
			#version 330 core

		    in vec2 fragTexCoord;
		    in vec4 fragColor;

		    out vec4 finalColor;

		    uniform sampler2D image;

			void main() {
		    	float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

			    vec2 texSize = textureSize(image, 0);
			    float size = max(texSize.x, texSize.y);
	
			    vec2 texOffset = 1.0 / vec2(size, size);
			    vec3 result = texture(image, fragTexCoord).rgb * weight[0];
	
			    for(int i = 1; i < 5; ++i) {
				    result += texture(image, fragTexCoord + vec2(texOffset.x * i, 0.0)).rgb * weight[i];
				    result += texture(image, fragTexCoord - vec2(texOffset.x * i, 0.0)).rgb * weight[i];
				    result += texture(image, fragTexCoord + vec2(0.0, texOffset.y * i)).rgb * weight[i];
				    result += texture(image, fragTexCoord - vec2(0.0, texOffset.y * i)).rgb * weight[i];
				}

			    result = texture(image, fragTexCoord).rgb + result;
			    result = vec3(1.0) - exp(-result * 0.5f);
			    result = pow(result, vec3(1.0 / 2.2));
			    finalColor = vec4(result, 1.0);
		    }
		""";

	public const char8* BloomShaderSource1 =
		"""
			#version 330 core

			in vec2 fragTexCoord;
			in vec4 fragColor;
	
			out vec4 finalColor;
	
			uniform sampler2D image;
	
			uniform float bloom_spread = 1;
			uniform float bloom_intensity = 2;
	
			void main() {
				ivec2 size = textureSize(image, 0);
		
				float uv_x = fragTexCoord.x * size.x;
				float uv_y = fragTexCoord.y * size.y;
		
				vec4 sum = vec4(0.0);
				for (int n = 0; n < 4; ++n) {
				    uv_y = (fragTexCoord.y * size.y) + (bloom_spread * float(n - 4));
				    vec4 h_sum = vec4(0.0);
				    h_sum += texelFetch(image, ivec2(uv_x - (4.0 * bloom_spread), uv_y), 0);
				    h_sum += texelFetch(image, ivec2(uv_x - (3.0 * bloom_spread), uv_y), 0);
				    h_sum += texelFetch(image, ivec2(uv_x - (2.0 * bloom_spread), uv_y), 0);
				    h_sum += texelFetch(image, ivec2(uv_x - bloom_spread, uv_y), 0);
				    h_sum += texelFetch(image, ivec2(uv_x, uv_y), 0);
				    h_sum += texelFetch(image, ivec2(uv_x + bloom_spread, uv_y), 0);
				    h_sum += texelFetch(image, ivec2(uv_x + (2.0 * bloom_spread), uv_y), 0);
				    h_sum += texelFetch(image, ivec2(uv_x + (3.0 * bloom_spread), uv_y), 0);
				    h_sum += texelFetch(image, ivec2(uv_x + (4.0 * bloom_spread), uv_y), 0);
				    sum += h_sum / 9.0;
				}
		
				finalColor = ((sum / 9.0) * bloom_intensity);		
			}
		""";

	public const char8* GlowShaderSource =
		"""
		#version 330

		// Input vertex attributes (from vertex shader)
		in vec2 fragTexCoord;
		in vec4 fragColor;

		// Input uniform values
		uniform sampler2D texture0;
		uniform sampler2D texture1;
		uniform vec4 colDiffuse;

		// Output fragment color
		out vec4 finalColor;

		// NOTE: Add here your custom variables

		//const vec2 size = vec2(800, 450);   // Framebuffer size
		const float samples = 7.0;          // Pixels per axis; higher = bigger glow, worse performance
		const float quality = 0.5;          // Defines size factor: Lower = smaller glow, better quality

		void main()
		{
			ivec2 size = textureSize(texture1, 0);

		    vec4 sum = vec4(0);
		    vec2 sizeFactor = vec2(1)/size*quality;

		    // Texel color fetching from texture sampler
		    vec4 source = texture(texture0, fragTexCoord);

		    const int range = 2;            // should be = (samples - 1)/2;

		    for (int x = -range; x <= range; x++)
		    {
		        for (int y = -range; y <= range; y++)
		        {
		            sum += texture(texture1, fragTexCoord + vec2(x, y)*sizeFactor);
		        }
		    }

		    // Calculate final fragment color
		    finalColor = texture(texture1, fragTexCoord) + ((sum/(samples*samples)) + source)*colDiffuse;
		}
		""";

	private static Shader			glowShader;
	private static Shader 			bloomShader;

	private static RenderTexture 	frameBuffer;
	private static RenderTexture 	frameBuffer1;

	public static RenderTexture FinalResult => frameBuffer;

	public static void Load()
	{
		glowShader = LoadShaderFromMemory(null, GlowShaderSource);
		bloomShader = LoadShaderFromMemory(null, BloomShaderSource1);

		frameBuffer = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
		frameBuffer1 = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
	}

	public static void Unload()
	{
		UnloadShader(bloomShader);
		bloomShader = default;

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

		// @todo(maihd): add multiple pass

		if (!noEffect)
		{
			BeginTextureMode(frameBuffer1);
			BeginShaderMode(bloomShader);
			DrawTextureRec(frameBuffer.texture, .(0, 0, GetScreenWidth(), -GetScreenHeight()), .Zero, .WHITE);
			EndShaderMode();
			EndTextureMode();
			
			BeginTextureMode(frameBuffer);

			BeginShaderMode(glowShader);
			SetShaderValueTexture(glowShader, GetShaderLocation(glowShader, "texture1"), frameBuffer.texture);
			DrawTextureRec(frameBuffer1.texture, .(0, 0, GetScreenWidth(), -GetScreenHeight()), .Zero, .WHITE);
			EndShaderMode();

			EndTextureMode();
		}
		else
		{
			//DrawTextureRec(frameBuffer.texture, .(0, 0, GetScreenWidth(), -GetScreenHeight()), .Zero, .WHITE);
		}
	}
}