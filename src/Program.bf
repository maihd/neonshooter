namespace NeonShooter;

using System;
using Mojo;

class Program
{
	// Request high performance NVidia GPU acceleration
	[Export]
	static int32   NvOptimusEnablement                  = 0x0;

	// Request high performance AMD GPU acceleration
	[Export]
	static int32   AmdPowerXpressRequestHighPerformance = 0x0;

	public static void Main()
	{
		Console.WriteLine("NeonShooter - Win32 - OpenGL - OpenAL - v1.0\n");
		Console.WriteLine("============================================\n");

		WindowFlags winFlags = WindowFlags.Visible;
		if (Window.Setup("Neon Shooter", 1280, 720, winFlags) case .Err(let error))
		{
			Console.Error.WriteLine("[Error] Setup window failure: {0}", error);
		    return;
		}
		defer
		{
			Window.Shutdown();
		}

		GraphicsSettings graphicsSettings = .();
		if (Graphics.Setup(graphicsSettings) case .Err(let error))
		{
			Console.Error.WriteLine("[Error] Setup graphics context failure: {0}", error);
			return;
		}
		defer
		{
			Graphics.Shutdown();
		}

		let game = new Game();
		defer delete game;
		game.Init();

		MainLoop: while (true)
		{
			if (!Window.PollEvents())
			{
			    // Handle quit event
			    break MainLoop;
			}

			const float dt = 0.0f;
			game.Update(dt);

			Graphics.Clear();
			game.Render();
			Graphics.Present();
		}
	}
}