#pragma once 

inline namespace Mojo
{
    namespace Time
    {
        bool        Setup(int fps, float timeScale);
        void        Shutdown(void);

        void        NewFrame(void);
        void        EndFrame(void);

        void        NewFixedFrame(void);
        void        EndFixedFrame(void);

        float       GetTimeScale(void);
        void        SetTimeScale(float scale);

        float       GetUnscaleDeltaTime(void);
        float       GetUnscaleTotalTime(void);

        float       GetDeltaTime(void);
        float       GetTotalTime(void);

        int         GetFrameCount(void);
        float       GetCurrentFrameRate(void);
        float       GetCurrentFixedFrameRate(void);

        int         GetTargetFrameRate(void);
        void        SetTargetFrameRate(int fps);

        int         GetTargetFixedFrameRate(void);
        void        SetTargetFixedFrameRate(int fps);
    };
};