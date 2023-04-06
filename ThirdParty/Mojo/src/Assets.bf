namespace Mojo;

using System;

public static class Assets 
{
    public static void AddSearchPath(StringView path)
	{

	}

    public static void RemoveSearchPath(StringView path)
	{

	}

    //void* LoadWave(StringView path, int* length, int* frequency, int* format);
    //void  FreeWave(void* wave);

    public static void* LoadImage(StringView path, out int32 width, out int32 height, out int32 channel)
	{
		width = 0;
		height = 0;
		channel = 0;
		return null;
	}

    public static void FreeImage(void* pixels)
	{

	}
}