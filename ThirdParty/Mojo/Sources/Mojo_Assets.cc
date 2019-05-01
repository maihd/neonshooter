#include <Mojo/Array.h>
#include <Mojo/Assets.h>
#include <Mojo/FileSystem.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

inline namespace Mojo
{
    static Array<const char*> _searchPaths;

    namespace Assets
    {
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

        void* LoadImage(const char* path, int* width, int* height, int* channel)
        {
            if (FileSystem::Exists(path))
            {
                return ::stbi_load(path, width, height, channel, 0);
            }

            for (int i = 0, n = _searchPaths.count; i < n; i++)
            {
                const char* searchPath = _searchPaths[i];

                char targetPath[1024];
                ::sprintf(targetPath, "%s/%s", searchPath, path);
                if (FileSystem::Exists(targetPath))
                {
                    return ::stbi_load(targetPath, width, height, channel, 0);
                }
            }
            return NULL;
        }

        void  FreeImage(void* pixels)
        {
            ::stbi_image_free(pixels);
        }
    }
}