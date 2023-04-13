namespace NeonShooter;

using Raylib;

class Program : RaylibApp
{
	const int SCREEN_WIDTH  = 1280;
	const int SCREEN_HEIGHT = 720;
	
	int  	 frameCount		 = 0;

	float    timer           = 0.0f;
	float    timeStep        = 1.0f / 60.0f;

	int      fpsCount        = 0;
	int      fpsValue        = 0;
	float    fpsTimer        = 0.0f;
	float    fpsInterval     = 1.0f;

	World 				world;
	

	Shader           	bloomShader;
	RenderTexture    	framebuffer;

	public static void Main()
	{
		Self app = scope .();

 		app.title = "NeonShooter";
		app.width = SCREEN_WIDTH;
		app.height = SCREEN_HEIGHT;
		app.Run();
	}

	public override void Init()
	{
		base.Init();

		SetConfigFlags(.FLAG_VSYNC_HINT);

		Assets.InitCacheTextures();

		GameAudio.Init();
		//GameInput.Init();

		bloomShader = LoadShaderFromMemory(null, Shaders.BloomShaderSource);
		framebuffer = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);

		world = new World();

		GameAudio.PlayMusic();
	}

	public override void Close()
	{
		delete world;

		//GameInput.Release();
		GameAudio.Release();

		Assets.ClearCacheTextures();
		base.Close();
	}

	public override void Update(float dt)
	{
		frameCount++;

		GameAudio.Update();

		float deltaTime = GetFrameTime();

		fpsTimer += deltaTime;
		if (fpsTimer >= fpsInterval)
		{
		    fpsTimer -= fpsInterval;

		    fpsValue = (int)(fpsCount / fpsInterval);
		    fpsCount = 0;
		}

		timer += deltaTime;
		while (timer >= timeStep)
		{
		    fpsCount++;

		    timer -= timeStep;

		    Vector2 aimDirection = GameInput.GetAimDirection(world.player.position);
		   	Vector2 moveDirection = GameInput.GetMoveDirection();

		    world.Update(moveDirection.x, moveDirection.y, aimDirection, GameInput.IsShooting(), timeStep);

		    //UpdateParticles(&world, timeStep);
		}
	}

	public override void Draw()
	{
		ClearBackground(.BLACK);

		BeginTextureMode(framebuffer);
        ClearBackground(.BLACK);

		Camera2D camera = .(
			Vector2(GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f),
		    Vector2(0, 0),
		    0,
		    0.5f
		);
		BeginMode2D(camera);
		{
		    world.Render();
		    //DrawParticles();
		}
		EndMode2D();

		EndTextureMode();

		BeginShaderMode(bloomShader);

		DrawTextureRec(framebuffer.texture, Rectangle(0, 0, SCREEN_WIDTH, -SCREEN_HEIGHT), Vector2(0, 0), .WHITE);
		EndShaderMode();

		DrawText(TextFormat("CPU FPS: %d", fpsValue), 0, 0, 18, .RAYWHITE);
		DrawText(TextFormat("GPU FPS: %d", GetFPS()), 0, 24, 18, .RAYWHITE);
	}
}