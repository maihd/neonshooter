#include <stdio.h>

#include <Mojo/Graphics.h>
#include <Mojo/Native/System.h>
#include <Mojo/Native/Window.h>

// Request high performance GPU
extern "C"
{
	__declspec(dllexport) int   NvOptimusEnablement                  = 0x0;
	__declspec(dllexport) int   AmdPowerXpressRequestHighPerformance = 0x0;
}

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

    WindowFlag winFlags = WindowFlag::Visible;
    if (!Window::Setup("Neon Shooter", 1280, 720, winFlags))
    {
        return 1;
    }

    if (!Graphics::Setup())
    {
        return 1;
    }

    Game::Init();

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

        Graphics::Clear();

        Game::Render();

        //Graphics::DrawIndices(DrawType::Triangles, _spriteShader, _spriteVertexArray, texture, _spriteIndexBuffer, 3, 0);

        Graphics::Present();
    }

    Graphics::Shutdown();
    Window::Shutdown();
    return 0;
}