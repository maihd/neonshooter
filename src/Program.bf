namespace NeonShooter;

using System;
using Mojo;

class Program
{
	public static void Main()
	{
		Console.WriteLine("NeonShooter - Win32 - OpenGL - OpenAL - v1.0");
		Console.WriteLine("============================================");

		// Create window
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

		// Create graphics context
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

		// Create game
		let game = new Game();
		defer delete game;
		game.Init();

		// Main loop
		MainLoop: while (true)
		{
			if (!Window.PollEvents())
			{
			    // Handle quit event
			    break MainLoop;
			}

			Update:
			{
				const float dt = 0.0f;
				game.Update(dt);
			}
			
			Render:
			{
				Graphics.Clear();
				game.Render();
				Graphics.Present();
			}
		}

		// No need to put destruction here
		// We have defer on each system Shutdown() call
		// +1 point for Beef
	}
}