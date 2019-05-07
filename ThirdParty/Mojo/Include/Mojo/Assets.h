#pragma once 

inline namespace Mojo
{
    namespace Assets 
    {
        void  AddSearchPath(const char* path);
        void  RemoveSearchPath(const char* path);

        void* LoadWave(const char* path, int* length, int* frequency, int* format);
        void  FreeWave(void* wave);

        void* LoadImage(const char* path, int* width, int* height, int* channel);
        void  FreeImage(void* pixels);
    }
}