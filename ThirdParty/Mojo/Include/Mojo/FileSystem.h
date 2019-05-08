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

    enum struct SeekWhence
    {
        Set     = 0,
        End     = 2,
        Current = 1,
    };
   
    struct Stream
    {
        int  (*seekFunc)(Stream*, int, SeekWhence);
        int  (*tellFunc)(Stream*);
        int  (*sizeFunc)(Stream*);
        int  (*readFunc)(Stream*, void* buffer, int length);
        int  (*writeFunc)(Stream*, const void* buffer, int length);

        // Set the cursor position of stream
        inline int Seek(int count, SeekWhence whence = SeekWhence::Current)
        {
            return seekFunc(this, count, whence);
        }

        // Tell the cursor position of stream
        inline int Tell(void)
        {
            return tellFunc(this);
        }

        // Get size of stream
        inline int Size(void)
        {
            return sizeFunc(this);
        }

        // Read content of file with _context
        inline int Read(void* buffer, int length)
        {
            return readFunc(this, buffer, length);
        }

        // Read content of file with _context
        inline int Write(const void* buffer, int length)
        {
            return writeFunc(this, buffer, length);
        }
    };

    struct File : Stream
    {
        void (*closeFunc)(File*);

        // Close file _context
        inline void Close(void)
        {
            closeFunc(this);
        }

        // Read content of file at given path, with async progress
        //virtual FileAsyncOperation* ReadAsync(void* buffer, int length);

        // Write content to file at given path, with async progress
        //virtual FileAsyncOperation* WriteAsync(const void* buffer, int length);
    };

    struct FileAsyncOperation
    {
        File*       file;
        const char* path;

        bool const isDone;
        bool const isSuccess;

        //int   const priority;
        float const progress;

        const void* const buffer;
              int   const length;

        FileAsyncOperation(void)
            : file(0)
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
        bool Setup(void);
        void Shutdown(void);

        // Make new directory at given path
        bool MakeDirectory(const char* path);

        // Remove the directory at given path
        bool RemoveDirectory(const char* path);

        // Is file or directory exist at path
        bool Exists(const char* path);

        // Open file _context at given path
        File* Open(const char* path, int flags);

        // Read content of file at given path
        int   ReadFile(const char* path, void* buffer, int length);

        // Write content to file at given path
        int   WriteFile(const char* path, const void* buffer, int length);

        // Read content of file at given path, with async progress
        FileAsyncOperation* ReadFileAsync(const char* path, void* buffer, int length);

        // Write content to file at given path, with async progress
        FileAsyncOperation* WriteFileAsync(const char* path, const void* buffer, int length);
    }
}