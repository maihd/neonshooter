namespace Mojo;

using System;
using SDL2;

public static class Window
{
	internal static SDL.Window*			_mainWindow;
	internal static SDL.SDL_GLContext	_mainContext;

	static WindowFlags			_windowFlags;

	public static Result<void, StringView> Setup(StringView title, int32 width, int32 height, WindowFlags flags)
	{
		let sdlErrorCode = SDL.Init(.Video);
		if (sdlErrorCode != 0)
		{
			Console.Error.WriteLine("[Error] SDL_Init: {0}", SDL.GetError());
			return .Err(.(SDL.GetError()));
		}

		if (_mainWindow != null)
		{
			return .Ok;
		}

		var sdlFlags = SDL.WindowFlags.OpenGL;
		if (flags.HasFlag(.Visible))
		{
		    sdlFlags |= .Shown;
		}
		if (flags.HasFlag(.Resizable))
		{
		    sdlFlags |= .Resizable;
		}

		switch (flags & (.Borderless | .Fullscreen))
		{
		case .Borderless, .Borderless | .Fullscreen:
			sdlFlags |= .FullscreenDesktop;
			sdlFlags &= ~.Resizable;

		case .Fullscreen:
			sdlFlags |= .Fullscreen;
			sdlFlags &= ~.Resizable;

		default:
		}

		// @note: maybe redundant?
		SDL.GL_SetAttribute(.GL_MULTISAMPLEBUFFERS, 0);
		SDL.GL_SetAttribute(.GL_MULTISAMPLESAMPLES, 0);

		let window = SDL.CreateWindow(title.Ptr, .Centered, .Centered, width, height, sdlFlags);
		if (window == null)
		{
			return .Err(.(SDL.GetError()));
		}

		_mainWindow = window;
		_windowFlags = flags;

		return .Ok;
	}

	public static void Shutdown()
	{
		SDL.DestroyWindow(_mainWindow);
		SDL.QuitSubSystem(.Video);
	}

	/// Process all coming events
	/// Return false if has quit message, true otherwise
	public static bool PollEvents()
	{
		SDL.Event event;
		while (SDL.PollEvent(out event) != 0)
		{
		    if (event.type == .Quit)
		    {
		        return false;
		    }
		    else
		    {
		        //HandleEvent(event);
		    }
		}

		return true;
	}

	/// Wait a events and then process all coming events
	/// Return false if has quit message, true otherwise
	public static bool WaitAndPollEvents()
	{
		return false;
	}
}