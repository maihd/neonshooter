#include <Mojo/Audio.h>

#include <AL/al.h>
#include <AL/alc.h>

inline namespace Mojo
{
    namespace
    {
        ALCdevice*  device;
        ALCcontext* context;
    }

    namespace Audio
    {
        void Setup(void)
        {

        }

        void Shutdown(void)
        {

        }

        void Preload(const char* path)
        {
            (void)path;
        }

        void Play(const char* path, bool loop)
        {
            (void)path;
            (void)loop;
        }
    }
}