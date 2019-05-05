#include <Mojo/Audio.h>

#include <AL/al.h>
#include <AL/alc.h>

inline namespace Mojo
{
    namespace
    {
        ALCdevice*  _device;
        ALCcontext* _context;
    }

    AudioSource AudioSource::Create(void)
    {
        AudioSource source;
        alGenSources(1, &source._handle);
        return source;
    }

    void AudioSource::Destroy(AudioSource& source)
    {
        alDeleteSources(1, &source._handle);
        source._handle = 0;
    }

    AudioBuffer AudioBuffer::Create(void)
    {
        AudioBuffer buffer;
        alGenBuffers(1, &buffer._handle);
        return buffer;
    }

    void AudioBuffer::Destroy(AudioBuffer& buffer)
    {
        alDeleteBuffers(1, &buffer._handle);
        buffer._handle = 0;
    }

    namespace Audio
    {
        bool Setup(void)
        {
            _device = alcOpenDevice(0);
            if (!_device)
            {
                //fprintf(stderr, "audio::init(): Cannot open audio _device '%s'\n", alcGetString(NULL, alcGetError(NULL)));
                return false;
            }

            //fprintf(stderr, "audio::init(): %s\n", alcGetString(_device, ALC_DEVICE_SPECIFIER));

            _context = alcCreateContext(_device, 0);
            if (!_context)
            {
                //fprintf(stderr, "audio::init(): Cannot create audio _context '%s'", alcGetString(_device, alcGetError(_device)));
                return false;
            }

            if (!alcMakeContextCurrent(_context))
            {
                return false;
            }
            //printf("Audio version: %s\n", alGetString(AL_VERSION));
            //printf("Audio renderer: %s\n", alGetString(AL_RENDERER));

            alListener3f(AL_POSITION, 0, 0, 0);
            alListener3f(AL_VELOCITY, 0, 0, 0);

            ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
            alListenerfv(AL_ORIENTATION, listenerOri);

            return true;
        }

        void Shutdown(void)
        {
            alcDestroyContext(_context);
        }

        void SetListenerPosition(float x, float y, float z)
        {
            alListener3f(AL_POSITION, x, y, z);
        }

        void SetListenerVelocity(float x, float y, float z)
        {
            alListener3f(AL_VELOCITY, x, y, z);
        }

        void SetListenerOrientation(float x0, float y0, float x1, float y1)
        {
            ALfloat listenerOri[] = { x0, y0, 0.0f, x1, y1, 0.0f };
            alListenerfv(AL_ORIENTATION, listenerOri);
        }

        void SetListenerOrientation(float x0, float y0, float z0, float x1, float y1, float z1)
        {
            ALfloat listenerOri[] = { x0, y0, z1, x1, y1, z1 };
            alListenerfv(AL_ORIENTATION, listenerOri);
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