namespace Mojo;

using SDL2;
using System;
using OpenGL;
using internal Mojo;

public struct GraphicsSettings
{
	public int32 MultiSamples 		= 1;
	public bool  HighPerformance 	= false;
}

public static class Graphics
{
	// Request high performance NVidia GPU acceleration
	[Export]
	static int32   NvOptimusEnablement                  = 0x0;

	// Request high performance AMD GPU acceleration
	[Export]
	static int32   AmdPowerXpressRequestHighPerformance = 0x0;

	// Graphics Context

	public static Result<void, StringView> Setup(GraphicsSettings settings = .())
	{
		// Make sure we have a window to display on
		if (Window._mainWindow == null)
		{
			return .Err("Window is not setup!");
		}

		// Context have been created
		if (Window._mainContext != 0)
		{
			// Check if the context is still available
			if (SDL.SDL_GL_MakeCurrent(Window._mainWindow, Window._mainContext) != 0)
			{
				return .Err(.(SDL.GetError()));
			}

			return .Ok;
		}

		SDL.GL_SetAttribute(.GL_ACCUM_RED_SIZE, 8);
		SDL.GL_SetAttribute(.GL_GREEN_SIZE, 8);
		SDL.GL_SetAttribute(.GL_BLUE_SIZE, 8);
		SDL.GL_SetAttribute(.GL_ALPHA_SIZE, 8);

		SDL.GL_SetAttribute(.GL_ACCUM_RED_SIZE, 8);
		SDL.GL_SetAttribute(.GL_ACCUM_GREEN_SIZE, 8);
		SDL.GL_SetAttribute(.GL_ACCUM_BLUE_SIZE, 8);
		SDL.GL_SetAttribute(.GL_ACCUM_ALPHA_SIZE, 8);

		SDL.GL_SetAttribute(.GL_DEPTH_SIZE, 24);
		SDL.GL_SetAttribute(.GL_STENCIL_SIZE, 8);

		SDL.GL_SetAttribute(.GL_CONTEXT_MAJOR_VERSION, 3);
		SDL.GL_SetAttribute(.GL_CONTEXT_MINOR_VERSION, 3);
		SDL.GL_SetAttribute(.GL_CONTEXT_PROFILE_MASK, .GL_CONTEXT_PROFILE_CORE);

		let context = SDL.GL_CreateContext(Window._mainWindow);
		if (context == 0)
		{
		    return .Err(.(SDL.GetError()));
		}
		
		// Load OpenGL functions from driver
		Window._mainContext = context;
		if (SDL.SDL_GL_MakeCurrent(Window._mainWindow, Window._mainContext) != 0)
		{
			return .Err(.(SDL.GetError()));
		}
		GL.LoadProcs((procname) => SDL.SDL_GL_GetProcAddress(procname.Ptr));

		// Apply GPU performance
		if (settings.HighPerformance)
		{
			NvOptimusEnablement 					= 0x1;
			AmdPowerXpressRequestHighPerformance 	= 0x1;
		}
		else
		{
			NvOptimusEnablement 					= 0x0;
			AmdPowerXpressRequestHighPerformance 	= 0x0;
		}

		return .Ok;
	}

	public static void Shutdown()
	{
		SDL.GL_DeleteContext(Window._mainContext);
		Window._mainContext = 0;
	}

	public static void Clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f)
	{
		GL.ClearColor(r, g, b, a);
		GL.Clear(GL.COLOR_BUFFER_BIT | GL.DEPTH_BUFFER_BIT | GL.STENCIL_BUFFER_BIT);
	}

	public static void Present()
	{
		SDL.GL_SwapWindow(Window._mainWindow);
	}
}