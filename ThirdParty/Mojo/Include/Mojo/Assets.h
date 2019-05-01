#pragma once 

inline namespace Mojo
{
    namespace Assets 
    {
        void  AddSearchPath(const char* path);
        void  RemoveSearchPath(const char* path);

        void* LoadImage(const char* path, int* width, int* height, int* channel);
        void  FreeImage(void* pixels);
    }
}