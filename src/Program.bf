namespace NeonShooter;

using System;

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

		var game = new Game();
		game.Init();

		while (true)
		{
			const float dt = 0.0f;
			game.Update(dt);
			game.Render();
		}
		
		Console.Read();
	}
}