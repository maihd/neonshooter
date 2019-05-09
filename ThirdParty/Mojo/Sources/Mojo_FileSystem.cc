#include <Mojo/Array.h>
#include <Mojo/Thread.h>
#include <Mojo/FileSystem.h>

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Shlwapi.h>
#include <fcntl.h>
#include <direct.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#pragma comment(lib, "Shlwapi.lib")

#undef RemoveDirectory

inline namespace Mojo
{
    void FileAsyncOperation::Wait(void)
    {
        while (!isDone) 
        {
            ::Sleep(1);
        };
    }

    template <int size>
    struct AsyncFilePool
    {
        int       head;
        int       tail;
        FileAsyncOperation data[size];

        AsyncFilePool(void)
            : head(0)
            , tail(0)
            , data()
        {
        }

        inline int Push(void)
        {
            int index = -1;
            int next = (head + 1) % size;
            if (next != tail)
            {
                index = head;
                head = next;
            }

            return index;
        }

        inline int Pop(void)
        {
            int index = -1;
            if (tail != head)
            {
                index = tail;
                tail = (tail + 1) % size;
            }

            return index;
        }
    };

    namespace
    {
        static AsyncFilePool<128> _asyncFilePool;
        static Array<const char*> _searchPaths;
    }

    struct OsFile : File
    {
        HANDLE handle;

        int Seek(int count, SeekWhence whence) override
        {
            return (int)::SetFilePointer(handle, count, NULL, (int)whence);
        }

        int Tell(void) override
        {

            return (int)::SetFilePointer(handle, 0, NULL, FILE_CURRENT);
        }

        int Size(void) override
        {
            return (int)::GetFileSize(handle, NULL);
        }

        int Read(void* buffer, int length) override
        {
            DWORD nbytes;
            if (::ReadFile((HANDLE)handle, buffer, (DWORD)length, &nbytes, NULL))
            {
                return (int)nbytes;
            }
            else
            {
                DWORD error = ::GetLastError();
                LPSTR messageBuffer = nullptr;
                DWORD size = ::FormatMessageA(
                    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                    NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

                ::LocalFree(messageBuffer);
                return -1;
            }
        }

        int Write(const void* buffer, int length) override
        {
            DWORD nbytes;
            if ((int)::WriteFile((HANDLE)handle, buffer, (DWORD)length, &nbytes, NULL))
            {
                return (int)nbytes;
            }
            else
            {
                return -1;
            }
        }

        void Close(void) override
        {
            ::CloseHandle((HANDLE)handle);
            delete this;
        }
    };

    namespace FileSystem
    {
        bool Setup(void)
        {
            return true;
        }

        void Shutdown(void)
        {

        }

        void  AddSearchPath(const char* path)
        {
            _searchPaths.Push(path);
        }

        void  RemoveSearchPath(const char* path)
        {
            for (int i = 0, n = _searchPaths.count; i < n; i++)
            {
                if (_stricmp(_searchPaths[i], path) == 0)
                {
                    _searchPaths.Erase(i);
                    break;
                }
            }
        }

        bool MakeDirectory(const char* path)
        {
            return ::_mkdir(path) == 0;
        }

        bool RemoveDirectory(const char* path)
        {
            return ::_rmdir(path) == 0;
        }

        bool Exists(const char* path, bool withSearchPath)
        {
            if (!::PathFileExistsA(path))
            {
                if (withSearchPath)
                {
                    char pathBuffer[2048];
                    if (!FileSystem::Exists(path, false))
                    {
                        const char* originPath = path;
                        for (int i = 0, n = _searchPaths.count; i < n; i++)
                        {
                            const char* searchPath = _searchPaths[i];

                            ::sprintf(pathBuffer, "%s/%s", searchPath, originPath);
                            if (FileSystem::Exists(pathBuffer, false))
                            {
                                return true;
                            }
                        }
                    }
                }

                return false;
            }
            else
            {
                return true;
            }
        }
        
        const char* GetExistsPath(const char* path)
        {
            if (!::PathFileExistsA(path))
            {
                thread_local char pathBuffer[2048];
                if (!FileSystem::Exists(path, false))
                {
                    const char* originPath = path;
                    for (int i = 0, n = _searchPaths.count; i < n; i++)
                    {
                        const char* searchPath = _searchPaths[i];

                        ::sprintf(pathBuffer, "%s/%s", searchPath, originPath);
                        if (FileSystem::Exists(pathBuffer, false))
                        {
                            return pathBuffer;
                        }
                    }
                }

                return NULL;
            }
            else
            {
                return path;
            }
        }

        File* Open(const char* path, int flags)
        {
            path = GetExistsPath(path);
            if (!path)
            {
                return NULL;
            }

            DWORD access = 0;
            DWORD shared = 0;
            DWORD disposition = 0;
            DWORD attributes = 0;

            switch (flags & (FileOpen::ReadWrite))
            {
            case FileOpen::Read:
                access |= FILE_GENERIC_READ;
                break;

            case FileOpen::Write:
                access |= FILE_GENERIC_WRITE;
                break;

            default:
                return NULL;
            }

            if (flags & FileOpen::Append)
            {
                access &= ~FILE_WRITE_DATA;
                access |= FILE_APPEND_DATA;
            }

            shared = FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE;

            switch (flags & (FileOpen::Create | FileOpen::Existing | FileOpen::Truncate))
            {
            case 0:
            case FileOpen::Existing:
                disposition = OPEN_EXISTING;
                break;

            case FileOpen::Create:
                disposition = OPEN_ALWAYS;
                break;

            case FileOpen::Create | FileOpen::Existing:
            case FileOpen::Create | FileOpen::Truncate | FileOpen::Existing:
                disposition = CREATE_NEW;
                break;

            case FileOpen::Truncate:
            case FileOpen::Truncate | FileOpen::Existing:
                disposition = TRUNCATE_EXISTING;
                break;

            case FileOpen::Create | FileOpen::Truncate:
                disposition = CREATE_ALWAYS;
                break;

            default:
                return NULL;
            }

            attributes |= FILE_ATTRIBUTE_NORMAL;
            if (flags & FileOpen::Create)
            {
                if (!(flags & FileOpen::Write))
                {
                    attributes |= FILE_ATTRIBUTE_READONLY;
                }
            }

            if (flags & FileOpen::Temporary)
            {
                attributes |= FILE_FLAG_DELETE_ON_CLOSE | FILE_ATTRIBUTE_TEMPORARY;
                access |= DELETE;
            }

            if (flags & FileOpen::ShortLive)
            {
                attributes |= FILE_ATTRIBUTE_TEMPORARY;
            }

            switch (flags & (FileOpen::Sequence | FileOpen::Random))
            {
            case 0:
                break;

            case FileOpen::Sequence:
                attributes |= FILE_FLAG_SEQUENTIAL_SCAN;
                break;

            case FileOpen::Random:
                attributes |= FILE_FLAG_RANDOM_ACCESS;
                break;

            default:
                return NULL;
            }

            if (flags & FileOpen::Direct)
            {
                /*
                 * FILE_APPEND_DATA and FILE_FLAG_NO_BUFFERING are mutually exclusive.
                 * Windows returns 87, ERROR_INVALID_PARAMETER if these are combined.
                 *
                 * FILE_APPEND_DATA is included in FILE_GENERIC_WRITE:
                 *
                 * FILE_GENERIC_WRITE = STANDARD_RIGHTS_WRITE |
                 *                      FILE_WRITE_DATA |
                 *                      FILE_WRITE_ATTRIBUTES |
                 *                      FILE_WRITE_EA |
                 *                      FILE_APPEND_DATA |
                 *                      SYNCHRONIZE
                 *
                 * Note: Appends are also permitted by FILE_WRITE_DATA.
                 *
                 * In order for direct writes and direct appends to succeed, we therefore
                 * exclude FILE_APPEND_DATA if FILE_WRITE_DATA is specified, and otherwise
                 * fail if the user's sole permission is a direct append, since this
                 * particular combination is invalid.
                 */
                if (access & FILE_APPEND_DATA)
                {
                    if (access & FILE_WRITE_DATA)
                    {
                        access &= ~FILE_APPEND_DATA;
                    }
                    else
                    {
                        return NULL;
                    }
                }
                attributes |= FILE_FLAG_NO_BUFFERING;
            }

            switch (flags & (FileOpen::DataSync | FileOpen::Sync))
            {
            case 0:
                break;

            case FileOpen::Sync:
            case FileOpen::DataSync:
                attributes |= FILE_FLAG_WRITE_THROUGH;
                break;

            default:
                return NULL;
            }

            /* Setting this flag makes it possible to open a directory. */
            if (flags & FileOpen::Directory)
            {
                attributes |= FILE_FLAG_BACKUP_SEMANTICS;
            }

            // Async read and write file
            if (flags & FileOpen::NonBlock)
            {
                //attributes |= FILE_FLAG_OVERLAPPED;
            }

            HANDLE handle = CreateFileA(path,
                access,
                shared,
                NULL,
                disposition,
                attributes,
                NULL);

            if (handle == INVALID_HANDLE_VALUE)
            {
                DWORD error = GetLastError();
                if (error == ERROR_FILE_EXISTS && (flags & FileOpen::Create) && !(flags & FileOpen::Existing))
                {
                    /* Special case: when ERROR_FILE_EXISTS happens and UV_FS_O_CREAT was
                     * specified, it means the path referred to a directory. */
                     //SET_REQ_UV_ERROR(req, UV_EISDIR, error);
                }
                else
                {
                    //SET_REQ_WIN32_ERROR(req, GetLastError());
                }
                return NULL;
            }
            else
            {
                OsFile* file = new OsFile();
                file->handle = handle;

                return file;
            }
        }

        int ReadFile(const char* path, void* buffer, int length)
        {
            File* file = Open(path, FileOpen::Read | FileOpen::Direct);
            if (file)
            {
                int ret = file->Read(buffer, length); file->Close();
                return (int)ret;
            }
            else
            {
                return 0;
            }
        }

        int WriteFile(const char* path, const void* buffer, int length)
        {
            File* file = Open(path, FileOpen::Read | FileOpen::Direct);
            if (file)
            {
                int ret = file->Write(buffer, length); file->Close();
                return (int)ret;
            }
            else
            {
                return 0;
            }
        }

        int ReadFileAsync_ThreadEntry(void* args)
        {
            constexpr int ChunkSize = 1024;

            FileAsyncOperation* asyncFile = (FileAsyncOperation*)args;

            int nbytes = 0;
            for (int i = 0, n = asyncFile->length; i < n;)
            {
                nbytes = asyncFile->file->Read((char*)asyncFile->buffer + i, ChunkSize);
                if (nbytes < 0)
                {
                    break;
                }

                *((float*)&asyncFile->progress) = (float)(i += ChunkSize) / n;
            }

            *((bool*)&asyncFile->isDone)    = true;
            *((bool*)&asyncFile->isSuccess) = nbytes != -1;
            if (!asyncFile->isSuccess)
            {
                *((int*)&asyncFile->length) = 0;
            }
            else
            {
                *((float*)&asyncFile->progress) = 1.0f;
            }

            // Close file
            asyncFile->file->Close();

            // Job complete, return data back to pool
            _asyncFilePool.Pop();

            return 0;
        }

        int WriteFileAsync_ThreadEntry(void* args)
        {
            constexpr int ChunkSize = 1024;

            FileAsyncOperation* asyncFile = (FileAsyncOperation*)args;

            int nbytes = 0;
            for (int i = 0, n = asyncFile->length; i < n;)
            {
                nbytes = asyncFile->file->Write((char*)asyncFile->buffer + i, ChunkSize);
                if (nbytes < 0)
                {
                    break;
                }

                *((float*)&asyncFile->progress) = (float)(i += ChunkSize) / n;
            }

            *((bool*)&asyncFile->isDone) = true;
            *((bool*)&asyncFile->isSuccess) = nbytes != -1;
            if (!asyncFile->isSuccess)
            {
                *((int*)&asyncFile->length) = 0;
            }
            else
            {
                *((float*)&asyncFile->progress) = 1.0f;
            }

            // Close file
            asyncFile->file->Close();

            // Job complete, return data back to pool
            _asyncFilePool.Pop();

            return 0;
        }

        FileAsyncOperation* ReadFileAsync(const char* path, void* buffer, int length)
        {
            if (!buffer)
            {
                return NULL;
            }

            File* file = Open(path, FileOpen::Read | FileOpen::Direct);
            if (file)
            {
                return NULL;
            }
            
            int fileSize = file->Size();
            if (length < fileSize)
            {
                file->Close();
                return NULL;
            }

            int asyncFileIndex = _asyncFilePool.Push();
            if (asyncFileIndex == -1)
            {
                file->Close();
                return NULL;
            }

            FileAsyncOperation* asyncFile = &_asyncFilePool.data[asyncFileIndex];
            ::memset(asyncFile, 0, sizeof(FileAsyncOperation));

            *((File**)&asyncFile->file) = file;
            *((void**)&asyncFile->path) = (void*)path;

            *((void**)&asyncFile->buffer) = (void*)buffer;
            *((int*  )&asyncFile->length) = fileSize;

            Thread::Run(ReadFileAsync_ThreadEntry, asyncFile);

            return asyncFile;
        }

        FileAsyncOperation* WriteFileAsync(const char* path, const void* buffer, int length)
        {
            if (!buffer)
            {
                return NULL;
            }

            File* file = Open(path, FileOpen::Read | FileOpen::Direct);
            if (file)
            {
                return NULL;
            }
            
            int fileSize = file->Size();
            if (length < fileSize)
            {
                file->Close();
                return NULL;
            }

            int asyncFileIndex = _asyncFilePool.Push();
            if (asyncFileIndex == -1)
            {
                file->Close();
                return NULL;
            }

            FileAsyncOperation* asyncFile = &_asyncFilePool.data[asyncFileIndex];
            ::memset(asyncFile, 0, sizeof(FileAsyncOperation));

            *((File**)&asyncFile->file) = file;
            *((void**)&asyncFile->path) = (void*)path;

            *((void**)&asyncFile->buffer) = (void*)buffer;
            *((int*  )&asyncFile->length) = fileSize;

            Thread::Run(WriteFileAsync_ThreadEntry, asyncFile);

            return asyncFile;
        }
    }
}