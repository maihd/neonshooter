namespace Mojo;

using System;

struct Allocator;
struct FileDevice;

enum FileOpen
{
    Sync        = 1 << 15,
    DataSync    = 1 << 14,
    NonBlock    = 1 << 13,

    NoLink      = 1 << 12,
    Append      = 1 << 11,
    Create      = 1 << 10,
    Direct      = 1 << 9,
    Existing    = 1 << 8,
    Truncate    = 1 << 7,

    Directory   = 1 << 6, 
    ShortLive   = 1 << 5,
    Temporary   = 1 << 4,

    Random      = 1 << 3,
    Sequence    = 1 << 2,

    Read        = 1 << 0,
    Write       = 1 << 1,
    ReadWrite   = Read | Write,
}

public abstract class File : Stream
{
    // Close file
    public abstract void Close();

    // Read content of file at given path, with async progress
    //virtual FileAsyncOperation* ReadAsync(void* buffer, int GetLength);

    // Write content to file at given path, with async progress
    //virtual FileAsyncOperation* WriteAsync(const void* buffer, int GetLength);
}

public struct FileAsyncOperation
{
    public File*	file;
    public String 	path;

    public bool 	isDone;
    public bool 	isSuccess;

    //int   const priority;
    public float 	progress;

    public void* 	buffer;
    public int   	length;

    public this()
    {
		this = default;
    }

    public void Wait()
	{

	}
}

public static class FileSystem
{
    public static bool Setup()
	{
		return false;
	}

    public static void Shutdown()
	{

	}

    // Add search path
    // @note: performance sensitive
    public static void AddSearchPath(StringView path)
	{

	}

    // Remove search path
    // @note: performance sensitive
    public static void RemoveSearchPath(StringView path)
	{

	}

    // Get exists path
    // @return: exists path of file or directory, if no result return invalid path
    public static void GetExistsPath(String result, StringView expectPath)
	{

	}

    // Is file or directory exists
    public static bool Exists(StringView path, bool withSearchPath = true)
	{
		return false;
	}

    // Make new directory at given path
    public static bool MakeDirectory(StringView path)
	{
		return false;
	}

    // Remove the directory at given path
    public static bool RemoveDirectory(StringView path)
	{
		return false;
	}

    // Open file _context at given path
    public static File Open(StringView path, FileOpen flags)
	{
		return null;
	}

    // Read content of file at given path
    public static int ReadFile(StringView path, void* buffer, int length)
	{
		return 0;
	}

    // Write content to file at given path
    public static int WriteFile(StringView path, void* buffer, int length)
	{
		return 0;
	}

    // Read content of file at given path, with async progress
    public static FileAsyncOperation* ReadFileAsync(StringView path, void* buffer, int length)
	{
		return null;
	}

    // Write content to file at given path, with async progress
    public static FileAsyncOperation* WriteFileAsync(StringView path, void* buffer, int length)
	{
		return null;
	}
}