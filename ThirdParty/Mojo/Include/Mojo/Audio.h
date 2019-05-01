#pragma once

inline namespace Mojo
{
    namespace Audio
    {
        void Setup(void);
        void Shutdown(void);

        void Preload(const char* path);

        void Play(const char* path, bool loop = false);
    }
}