namespace Mojo;

using System;

[AllowDuplicates]
public enum WindowFlags
{
	None       	= 0,
	Visible    	= 1 << 0,
	Resizable  	= 1 << 1,
	Fullscreen 	= 1 << 2,
	Borderless 	= 1 << 3,

	Default 	= Visible,
}