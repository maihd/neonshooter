#pragma once

inline namespace Mojo
{
    struct Allocator;
    struct FileDevice;

    namespace FileOpen
    {
        enum
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
        };
    };

    enum struct FileSeek
    {
        Set     = 0,
        End     = 2,
        Current = 1,
    };
   
    struct File
    {
        void* _handle = 0;

        // Open file context at given path
        bool Open(const char* path, int flags);

        // Open file context at given path
        bool Open(const char* path, const char* flags);

        // Close file context
        void Close(void);

        // Set the position of file pointer
        int  Seek(FileSeek whence, int count);

        // Tell the position of file pointer
        int  Tell(void);

        // Get size of file pointer
        int  Size(void);

        // Read content of file with context
        int  Read(void* buffer, int length);

        // Read content of file with context
        int  Write(const void* buffer, int length);

        // Read content of file at given path, with async progress
        //AsyncFile* ReadAsync(void* buffer, int length);

        // Write content to file at given path, with async progress
        //AsyncFile* WriteAsync(const void* buffer, int length);
    };

    struct AsyncFile
    {
        File        file;
        const char* path;

        bool const isDone;
        bool const isSuccess;

        //int   const priority;
        float const progress;

        const void* const buffer;
              int   const length;

        AsyncFile(void)
            : file()
            , path(0)
            , isDone(false)
            , isSuccess(false)
            , progress(0.0f)
            , buffer(0)
            , length(0)
        {
        }

        void Wait(void);
    };

    namespace FileSystem
    {
        bool Setup(FileDevice* fileDevice, Allocator* allocator = 0);
        void Shutdown(void);

        // Make new directory at given path
        bool MakeDirectory(const char* path);

        // Remove the directory at given path
        bool RemoveDirectory(const char* path);

        // Is file or directory exist at path
        bool Exists(const char* path);

        // Read content of file at given path
        int        ReadFile(const char* path, void* buffer, int length);

        // Write content to file at given path
        int        WriteFile(const char* path, const void* buffer, int length);

        // Read content of file at given path, with async progress
        AsyncFile* ReadFileAsync(const char* path, void* buffer, int length);

        // Write content to file at given path, with async progress
        AsyncFile* WriteFileAsync(const char* path, const void* buffer, int length);
    }
}