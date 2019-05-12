#include <Mojo/Mojo.h>
#include <Mojo/Core/ConfigVar.h>
#include <Mojo/Native/System.h>
#include <Mojo/Native/Window.h>

namespace Mojo
{
    namespace
    {
        //static ConfigVar _configTitle       ("Window::Title"        , "Title of window", 0, "Mojo");
        //static ConfigVar _configWidth       ("Window::Width"        , "Width of window", 0, 1280);
        //static ConfigVar _configHeight      ("Window::Height"       , "Height of window", 0, 720);
    }

    int RunEngine(Engine* engine)
    {
        ConfigVar* windowTitle  = ConfigVar::Find("Window::Title");
        ConfigVar* windowWidth  = ConfigVar::Find("Window::Width");
        ConfigVar* windowHeight = ConfigVar::Find("Window::Height");

        const char* title = windowTitle ? windowTitle->asString : "Mojo";
        int width  = windowWidth ? windowWidth->asInt : 1280;
        int height = windowHeight ? windowHeight->asInt : 720;

        if (!Window::Setup(title, width, height))
        {
            return 1;
        }

        if (!Graphics::Setup())
        {
            return 1;
        }

        if (!engine->Setup())
        {
            return 1;
        }

        long long ticks = System::PerformanceCounter();
        long long frequency = System::PerformanceFrequency();
        long long limitTicks = frequency / 60;

        float deltaTime = 0.0f;
        float totalTime = 0.0f;

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

            deltaTime = (float)((double)delta / frequency);
            totalTime = totalTime + deltaTime;

            if (!Window::PollEvents())
            {
                // Handle quit event
                break;
            }

            // Update
            engine->Update();

            Graphics::ClearBuffer();

            // Renderer
            engine->Render();

            Graphics::SwapBuffers();
        }

        Graphics::Shutdown();
        Window::Shutdown();
        return 0;
    }
}