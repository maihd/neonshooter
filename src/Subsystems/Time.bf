namespace NeonShooter;

using Raylib;

public static class Time
{
    public static float TimeScale
    {
        get; set;
    }

    public static float DeltaTime
    {
        get
        {
            return TimeScale * Raylib.GetFrameTime();
        }
    }

    public static float UnscaledDeltaTime
    {
        get
        {
            return Raylib.GetFrameTime();
        }
    }
}