namespace NeonShooter;

using System;
using System.Diagnostics;

public static class SceneManager
{
    private static Scene s_CurrentScene;
    private static Scene s_NextScene;

    public static void Init()
    {

    }

    public static void Shutdown()
    {

    }

    public static void ChangeScene(Scene scene)
    {
        Debug.Assert(scene != null);
        Debug.Assert(s_NextScene != null);

        s_NextScene = scene;
    }

    public static void Update()
    {
        // Change scene
        if (s_NextScene != null)
        {
            if (s_CurrentScene != null)
            {
                s_CurrentScene.OnExit();
                delete s_CurrentScene;
            }

            s_CurrentScene = s_NextScene;
            s_CurrentScene.OnEnter();

            s_NextScene = null;
        }

        // Run update
        if (s_CurrentScene != null)
        {
            s_CurrentScene.Update();
        }
    }
}