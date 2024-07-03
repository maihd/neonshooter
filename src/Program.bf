namespace NeonShooter;

using System;
using NeonShooter.Entity;

class Program : Raylib.RaylibApp
{
	var isPause = false;
	var noBloomEffect = false;

	static void Main()
	{
		let program = scope Program()
			{
				title = "NeonShooter",
				width = 1280,
				height = 720,
				config = .FLAG_VSYNC_HINT
			};

		program.Run();
	}

	protected override void Init()
	{
		Raylib.HideCursor();

		Art.Load();
		Audios.Load();
		GameRoot.Init();

		BloomComponent.Load();
		PixelizerComponent.Load();

		EntityManager.Init();
		EntityManager.AddEntity(PlayerShip.Instance);

		EnemySpawner.Init();

		//Raylib.ToggleFullscreen();
	}

	protected override void Close()
	{
		EnemySpawner.Deinit();

		PlayerShip.DestroyInstance();
		EntityManager.Deinit();

		PixelizerComponent.Unload();
		BloomComponent.Unload();
		
		GameRoot.Deinit();
		Audios.Unload();
		Art.Unload();
	}

	protected override void Update(float dt)
	{
		GameRoot.Update();
		Audios.Update();

		Input.Update();

		if (Raylib.IsKeyPressed(.KEY_GRAVE))
		{
			isPause = !isPause;
		}

		if (Raylib.IsKeyPressed(.KEY_SPACE))
		{
			noBloomEffect = !noBloomEffect;
		}

		if (Raylib.IsKeyPressed(.KEY_ENTER))
		{
			Raylib.ToggleFullscreen();
		}

		if (isPause)
		{
			return;
		}

		if (!PlayerStatus.IsGameOver)
		{
			EntityManager.Update(dt);
			EnemySpawner.Update();
		}
		
		PlayerStatus.Update();
	}

	protected override void Draw()
	{
		Raylib.ClearBackground(.BLACK);

		// Draw game world
		
		BloomComponent.Begin(.BLACK);

		GameRoot.WarpGrid.Draw();

		EntityManager.Draw();
		GameRoot.Draw();

		BloomComponent.End(noBloomEffect);
		PixelizerComponent.Draw(BloomComponent.FinalResult, noBloomEffect);

		// Draw ui

		Raylib.DrawTextEx(Art.Font, scope $"Lives: {PlayerStatus.Lives}\0", .(5, 5), Art.Font.baseSize, 0.0f, .WHITE);
		DrawRightAlignedString(scope $"Score: {PlayerStatus.Score}\0", 5);
		DrawRightAlignedString(scope $"Multiplier: {PlayerStatus.Multiplier}\0", 35);

		if (PlayerStatus.IsGameOver)
		{
			let text = scope $"""
			Game Over

			Your Score: {PlayerStatus.Score}

			High Score: {PlayerStatus.HighScore}\0
			""";

			let textSize = Raylib.MeasureTextEx(Art.Font, text.Ptr, Art.Font.baseSize, 5.0f);
			Raylib.DrawTextEx(Art.Font, text.Ptr, GameRoot.Size / 2 - textSize / 2, Art.Font.baseSize, 0.0f, .WHITE);
		}

		if (isPause)
		{
			Raylib.DrawRectangleRec(GameRoot.Viewport, .(0, 0, 0, 156));
			Raylib.DrawText("GAME IS PAUSING", 6, 6, 64, .WHITE);
			Raylib.DrawText("> For waiting code change", 6, 78, 64, .WHITE);
			Raylib.DrawText("> from Hot Compiling", 6, 142, 64, .WHITE);
		}

		// draw the custom mouse cursor
		if (Input.IsAimingWithMouse)
		{
			Raylib.DrawTextureV(Art.Pointer, Input.MousePosition, .WHITE);
		}
	}

	private void DrawRightAlignedString(StringView text, float y)
	{
		let textWidth = Raylib.MeasureTextEx(Art.Font, text.Ptr, Art.Font.baseSize, 0.0f).x;
		Raylib.DrawTextEx(Art.Font, text.Ptr, .(GameRoot.Size.width - textWidth - 5, y), Art.Font.baseSize, 0.0f, .WHITE);
	}
}