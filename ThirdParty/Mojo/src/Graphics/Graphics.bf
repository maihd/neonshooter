namespace Mojo;

using SDL2;
using System;
using OpenGL;
using internal Mojo;

public struct GraphicsSettings
{
	public int32 MultiSamples = 1;
}

public static class Graphics
{
	// Graphics Context

	public static Result<void, StringView> Setup(GraphicsSettings settings = .())
	{
		if (Window._mainWindow == null)
		{
			return .Err("Window is not setup!");
		}

		if (Window._mainContext != 0)
		{
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

		Window._mainContext = context;
		if (SDL.SDL_GL_MakeCurrent(Window._mainWindow, Window._mainContext) != 0)
		{
			return .Err(.(SDL.GetError()));
		}

		GL.Init((procname) => SDL.SDL_GL_GetProcAddress(procname.Ptr));

		return .Ok;
	}

	public static void Shutdown()
	{
		SDL.GL_DeleteContext(Window._mainContext);
		Window._mainContext = 0;
	}

	public static void Clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f)
	{
		GL.glClearColor(r, g, b, a);
		GL.glClear(GL.GL_COLOR_BUFFER_BIT | GL.GL_DEPTH_BUFFER_BIT | GL.GL_STENCIL_BUFFER_BIT);
	}

	public static void Present()
	{
		SDL.GL_SwapWindow(Window._mainWindow);
	}
}