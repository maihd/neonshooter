#include <Mojo/Thread.h>
#include <Mojo/FileSystem.h>

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Shlwapi.h>
#include <fcntl.h>
#include <direct.h>

#pragma comment(lib, "Shlwapi.lib")

#undef RemoveDirectory

inline namespace Mojo
{
    void AsyncFile::Wait(void)
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
        AsyncFile data[size];

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

    inline namespace FileSystem_Variables
    {
        static AsyncFilePool<128> _asyncFilePool;
    }

    bool File::Open(const char* path, int flags)
    {
        if (_handle)
        {
            return true;
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

        HANDLE file = CreateFileA(path,
            access,
            shared,
            NULL,
            disposition,
            attributes,
            NULL);

        if (file == INVALID_HANDLE_VALUE)
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

        return (_handle = file) != NULL;
    }

    bool File::Open(const char* path, const char* flags)
    {
        int intFlags = 0;
        for (int i = *flags; i; i = *flags++)
        {
            switch (i)
            {
            case 'r':
                intFlags |= FileOpen::Read;
                break;

            case 'w':
                intFlags |= FileOpen::Write;
                break;

            case 'a':
                intFlags |= FileOpen::Append;
                break;

            case '+':
                intFlags |= FileOpen::Create;
                break;
            }
        }

        return this->Open(path, intFlags);
    }

    int File::Seek(FileSeek whence, int count)
    {
        return (int)::SetFilePointer(_handle, count, NULL, (int)whence);
    }

    int File::Tell(void)
    {

        return (int)::SetFilePointer(_handle, 0, NULL, FILE_CURRENT);
    }

    int File::Size(void)
    {
        return (int)::GetFileSize(_handle, NULL);
    }

    int File::Read(void* buffer, int length)
    {
        DWORD nbytes;
        if (::ReadFile((HANDLE)_handle, buffer, (DWORD)length, &nbytes, NULL))
        {
            return (int)nbytes;
        }
        else
        {
            return -1;
        }
    }

    int File::Write(const void* buffer, int length)
    {
        DWORD nbytes;
        if ((int)::WriteFile((HANDLE)_handle, buffer, (DWORD)length, &nbytes, NULL))
        {
            return (int)nbytes;
        }
        else
        {
            return -1;
        }
    }

    void File::Close(void)
    {
        ::CloseHandle((HANDLE)_handle);
        _handle = NULL;
    }

    namespace FileSystem
    {
        bool Init(FileDevice* fileDevice, Allocator* allocator)
        {
            (void)fileDevice;
            (void)allocator;

            return true;
        }

        void Shutdown(void)
        {

        }

        bool MakeDirectory(const char* path)
        {
            return ::_mkdir(path) == 0;
        }

        bool RemoveDirectory(const char* path)
        {
            return ::_rmdir(path) == 0;
        }

        bool Exists(const char* path)
        {
            return ::PathFileExistsA(path);
        }

        int ReadFile(const char* path, void* buffer, int length)
        {
            File file;
            if (file.Open(path, FileOpen::Read | FileOpen::Direct))
            {
                int ret = file.Read(buffer, length); file.Close();
                return (int)ret;
            }
            else
            {
                return 0;
            }
        }

        int WriteFile(const char* path, const void* buffer, int length)
        {
            File file;
            if (file.Open(path, FileOpen::Write | FileOpen::Create | FileOpen::Direct))
            {
                int ret = file.Write(buffer, length); file.Close();
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

            AsyncFile* asyncFile = (AsyncFile*)args;

            int nbytes = 0;
            for (int i = 0, n = asyncFile->length; i < n;)
            {
                nbytes = asyncFile->file.Read((char*)asyncFile->buffer + i, ChunkSize);
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
            asyncFile->file.Close();

            // Job complete, return data back to pool
            _asyncFilePool.Pop();

            return 0;
        }

        int WriteFileAsync_ThreadEntry(void* args)
        {
            constexpr int ChunkSize = 1024;

            AsyncFile* asyncFile = (AsyncFile*)args;

            int nbytes = 0;
            for (int i = 0, n = asyncFile->length; i < n;)
            {
                nbytes = asyncFile->file.Write((char*)asyncFile->buffer + i, ChunkSize);
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
            asyncFile->file.Close();

            // Job complete, return data back to pool
            _asyncFilePool.Pop();

            return 0;
        }

        AsyncFile* ReadFileAsync(const char* path, void* buffer, int length)
        {
            if (!buffer)
            {
                return NULL;
            }

            File file;
            if (!file.Open(path, FileOpen::Read | FileOpen::Direct))
            {
                return NULL;
            }
            
            int fileSize = file.Size();
            if (length < fileSize)
            {
                file.Close();
                return NULL;
            }

            int asyncFileIndex = _asyncFilePool.Push();
            if (asyncFileIndex == -1)
            {
                file.Close();
                return NULL;
            }

            AsyncFile* asyncFile = &_asyncFilePool.data[asyncFileIndex];
            ::memset(asyncFile, 0, sizeof(AsyncFile));

            *((File* )&asyncFile->file) = file;
            *((void**)&asyncFile->path) = (void*)path;

            *((void**)&asyncFile->buffer) = (void*)buffer;
            *((int*  )&asyncFile->length) = fileSize;

            Thread::Run(ReadFileAsync_ThreadEntry, asyncFile);

            return asyncFile;
        }

        AsyncFile* WriteFileAsync(const char* path, const void* buffer, int length)
        {
            if (!buffer)
            {
                return NULL;
            }

            File file;
            if (!file.Open(path, FileOpen::Write | FileOpen::Direct))
            {
                return NULL;
            }
            
            int fileSize = file.Size();
            if (length < fileSize)
            {
                file.Close();
                return NULL;
            }

            int asyncFileIndex = _asyncFilePool.Push();
            if (asyncFileIndex == -1)
            {
                file.Close();
                return NULL;
            }

            AsyncFile* asyncFile = &_asyncFilePool.data[asyncFileIndex];
            ::memset(asyncFile, 0, sizeof(AsyncFile));

            *((File* )&asyncFile->file) = file;
            *((void**)&asyncFile->path) = (void*)path;

            *((void**)&asyncFile->buffer) = (void*)buffer;
            *((int*  )&asyncFile->length) = fileSize;

            Thread::Run(WriteFileAsync_ThreadEntry, asyncFile);

            return asyncFile;
        }
    }
}