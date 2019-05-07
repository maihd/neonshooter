#include <Mojo/Audio.h>

#include <stdio.h>
#include <AL/al.h>
#include <AL/alc.h>

inline namespace Mojo
{
    namespace
    {
        ALCdevice*  _device;
        ALCcontext* _context;

        ALenum ConvertAudioFormat(AudioFormat format)
        {
            switch (format)
            {
            case AudioFormat::Mono8:
                return AL_FORMAT_MONO8; 

            case AudioFormat::Mono16:
                return AL_FORMAT_MONO16;

            case AudioFormat::Stereo8:
                return AL_FORMAT_STEREO8;

            case AudioFormat::Stereo16:
                return AL_FORMAT_STEREO16;
            }
        }
    }

    AudioSource AudioSource::Create(void)
    {
        AudioSource source;
        alGenSources(1, &source._handle);
        
        source.SetGain(1.0f);
        source.SetPitch(1.0f);
        source.SetLooping(false);
        source.SetPosition(0, 0, 0);
        source.SetVelocity(0, 0, 0);

        return source;
    }

    void AudioSource::Destroy(AudioSource& source)
    {
        alDeleteSources(1, &source._handle);
        source._handle = 0;
    }

    void AudioSource::SetGain(float value)
    {
        alSourcef(_handle, AL_GAIN, value);

        ALenum error = alGetError();
        if (error != AL_NO_ERROR)
        {
            printf("AudioSource::SetGain: %s\n", alGetString(error));
        }
    }

    void AudioSource::SetPitch(float value)
    {
        alSourcef(_handle, AL_PITCH, value);

        ALenum error = alGetError();
        if (error != AL_NO_ERROR)
        {
            printf("AudioSource::SetPitch: %s\n", alGetString(error));
        }
    }

    void AudioSource::SetLooping(bool value)
    {
        alSourcei(_handle, AL_LOOPING, value);

        ALenum error = alGetError();
        if (error != AL_NO_ERROR)
        {
            printf("AudioSource::SetLooping: %s\n", alGetString(error));
        }
    }

    void AudioSource::SetPosition(float x, float y, float z)
    {
        alSource3f(_handle, AL_POSITION, x, y, z);

        ALenum error = alGetError();
        if (error != AL_NO_ERROR)
        {
            printf("AudioSource::SetPosition: %s\n", alGetString(error));
        }
    }

    void AudioSource::SetVelocity(float x, float y, float z)
    {
        alSource3f(_handle, AL_VELOCITY, x, y, z);

        ALenum error = alGetError();
        if (error != AL_NO_ERROR)
        {
            printf("AudioSource::SetVelocity: %s\n", alGetString(error));
        }
    }

    void AudioSource::SetBuffer(const AudioBuffer* buffer)
    {
        alSourcei(_handle, AL_BUFFER, buffer ? buffer->_handle : 0);

        ALenum error = alGetError();
        if (error != AL_NO_ERROR)
        {
            printf("AudioSource::SetBuffer: %s\n", alGetString(error));
        }
    }

    void AudioSource::Play(void)
    {
        alSourcePlay(_handle);
    }

    void AudioSource::Stop(void)
    {
        alSourceStop(_handle);
    }

    void AudioSource::Pause(void)
    {
        alSourcePause(_handle);
    }
    
    void AudioSource::Resume(void)
    {
        alSourcePlay(_handle);
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

    void AudioBuffer::SetData(void* data, int size, int frequency, AudioFormat format)
    {
        alBufferData(_handle, ConvertAudioFormat(format), data, size, frequency);

        ALenum error = alGetError();
        if (error != AL_NO_ERROR)
        {
            printf("AudioBuffer::SetData: %s\n", alGetString(error));
        }
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