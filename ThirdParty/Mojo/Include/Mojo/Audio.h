#pragma once

inline namespace Mojo
{
    struct AudioBufferHandle;
    struct AudioSourceHandle;

    enum struct AudioFormat
    {
        Mono8,
        Mono16,
        Stereo8,
        Stereo16,
    };

    struct AudioBuffer
    {
        unsigned handle = 0;

        static AudioBuffer Create(void);
        static void        Destroy(AudioBuffer& source);

        void SetData(void* data, int size, int frequency, AudioFormat format);

        inline operator AudioBufferHandle*(void) const
        {
            return (AudioBufferHandle*)(long long)handle;
        }
    };

    struct AudioSource
    {
        unsigned handle = 0;

        static AudioSource Create(void);
        static void        Destroy(AudioSource& source);

        void SetGain(float value);
        void SetPitch(float value);
        void SetLooping(bool value);

        void SetPosition(float x, float y, float z = 0.0f);
        void SetVelocity(float x, float y, float z = 0.0f);

        void SetBuffer(AudioBufferHandle* buffer);

        void Play(void);
        void Stop(void);
        void Pause(void);
        void Resume(void);

        inline operator AudioSourceHandle*(void) const
        {
            return (AudioSourceHandle*)(long long)handle;
        }
    };

    struct AudioListener
    {
        void SetPosition(float x, float y, float z = 0.0f);
        void SetVelocity(float x, float y, float z = 0.0f);
        void SetOrientation(float x0, float y0, float x1, float y1);
        void SetOrientation(float x0, float y0, float z0, float x1, float y1, float z1);
    };

    namespace Audio
    {
        bool Setup(void);
        void Shutdown(void);

        void SetListenerPosition(float x, float y, float z = 0.0f);
        void SetListenerVelocity(float x, float y, float z = 0.0f);
        void SetListenerOrientation(float x0, float y0, float x1, float y1);
        void SetListenerOrientation(float x0, float y0, float z0, float x1, float y1, float z1);
    }
}