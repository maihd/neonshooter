#pragma once

inline namespace Mojo
{
    struct AudioSource
    {
        unsigned _handle;

        static AudioSource Create(void);
        static void        Destroy(AudioSource& source);

        
    };

    struct AudioBuffer
    {
        unsigned _handle;

        static AudioBuffer Create(void);
        static void        Destroy(AudioBuffer& source);
    };

    namespace AL
    {
        bool Setup(void);
        void Shutdown(void);

        void SetListenerPosition(float x, float y, float z = 0.0f);
        void SetListenerVelocity(float x, float y, float z = 0.0f);
        void SetListenerOrientation(float x0, float y0, float x1, float y1);
        void SetListenerOrientation(float x0, float y0, float z0, float x1, float y1, float z1);
    }
}