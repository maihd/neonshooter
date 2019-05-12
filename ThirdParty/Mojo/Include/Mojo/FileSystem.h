#pragma once

#include <Mojo/Core/Stream.h>

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

    struct File : public Stream
    {
        // Close file
        virtual void Close(void) = 0;

        // Read content of file at given path, with async progress
        //virtual FileAsyncOperation* ReadAsync(void* buffer, int GetLength);

        // Write content to file at given path, with async progress
        //virtual FileAsyncOperation* WriteAsync(const void* buffer, int GetLength);
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

        // Add search path
        // @note: performance sensitive
        void AddSearchPath(const char* path);

        // Remove search path
        // @note: performance sensitive
        void RemoveSearchPath(const char* path);

        // Get exists path
        // @return: exists path of file or directory, if no result return invalid path
        const char* GetExistsPath(const char* expectPath);

        // Is file or directory exists
        bool Exists(const char* path, bool withSearchPath = true);

        // Make new directory at given path
        bool MakeDirectory(const char* path);

        // Remove the directory at given path
        bool RemoveDirectory(const char* path);

        // Open file _context at given path
        File* Open(const char* path, int flags);

        // Read content of file at given path
        int   ReadFile(const char* path, void* buffer, int GetLength);

        // Write content to file at given path
        int   WriteFile(const char* path, const void* buffer, int GetLength);

        // Read content of file at given path, with async progress
        FileAsyncOperation* ReadFileAsync(const char* path, void* buffer, int GetLength);

        // Write content to file at given path, with async progress
        FileAsyncOperation* WriteFileAsync(const char* path, const void* buffer, int GetLength);
    }
}