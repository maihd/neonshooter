#include <stdio.h>

#include <Mojo/Math.h>
#include <Mojo/System.h>
#include <Mojo/Coroutine.h>
#include <Mojo/JobSystem.h>
#include <Mojo/JobCounter.h>
#include <Mojo/FileSystem.h>

#include <Mojo/GL.h>
#include <Mojo/Window.h>
#include <Mojo/HashTable.h>

// Request high performance GPU
extern "C"
{
	__declspec(dllexport) int   NvOptimusEnablement                  = 0x0;
	__declspec(dllexport) int   AmdPowerXpressRequestHighPerformance = 0x0;
}

Texture LoadTexture(const char* path);

namespace Game
{
    void Init(void);
    void Update(float dt);
    void Render(void);
}

int main(void)
{
    printf("NeonShooter - Win32 - OpenGL - OpenAL - v1.0\n");
    printf("============================================\n");

    int winFlags = WindowFlag::Visible;
    if (!Window::Setup("Neon Shooter", 1280, 720, winFlags))
    {
        return 1;
    }

    GraphicsSettings settings;
    settings.multisamples = 1;
    if (!GL::Setup(settings))
    {
        return 1;
    }

    Game::Init();

    Texture texture = LoadTexture("Art/Black Hole.png");

    long long ticks = System::PerformanceCounter();
    long long frequency = System::PerformanceFrequency();
    long long limitTicks = frequency / 60;

    while (true)
    {
        long long delta = System::PerformanceCounter() - ticks;
        if (delta < limitTicks)
        {
            double sleepSeconds = (double)(limitTicks - delta) / frequency;
            long long sleepMicroSeconds = (long long)(sleepSeconds * 1000 * 1000);
            System::MicroSleep(sleepMicroSeconds);

            delta = limitTicks;
        }
        ticks += delta;

        if (!Window::PollEvents())
        {
            // Handle quit event
            break;
        }

        float deltaTime = (float)((double)delta / frequency);
        Game::Update(deltaTime);

        GL::ClearBuffer(ClearFlag::Color | ClearFlag::Depth);

        Game::Render();

        //GL::DrawIndices(DrawType::Triangles, _spriteShader, _spriteVertexArray, texture, _spriteIndexBuffer, 3, 0);

        GL::SwapBuffers();
    }

    GL::Shutdown();
    Window::Shutdown();
    return 0;
}