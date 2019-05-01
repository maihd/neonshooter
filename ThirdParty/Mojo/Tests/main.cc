#include <Mojo/Math.h>
#include <Mojo/System.h>
#include <Mojo/Coroutine.h>
#include <Mojo/JobSystem.h>
#include <Mojo/JobCounter.h>
#include <Mojo/FileSystem.h>

#include <Mojo/GL.h>
#include <Mojo/Window.h>
#include <Mojo/HashTable.h>

#include <stdio.h>
#include <stdint.h>
#include <Windows.h>

#undef Yield

AtomicI32 printTime;

void JobPrint(void* args)
{
    int min = (int)(intptr_t)args;
    int max = (int)(intptr_t)args + 10000000;

    while (min < max)
    {
        min++;
        printTime++;
        //printf("Hello world %d\n", min);
    }
}

int main(void)
{
    //JobSystem::Init(1000000 >> 2);
    //
    //constexpr int JobCount = 100;
    //Job* jobs = new Job[JobCount];
    //
    //DWORD startTime = GetTickCount();
    ////for (int i = 0; i < JobCount; i++)
    ////{
    ////    jobs[i] = Job(JobPrint, (void*)(intptr_t)i);
    ////}
    //
    ////JobCounter counter(JobCount);
    ////JobSystem::Execute(jobs, JobCount, &counter);
    ////
    ////JobSystem::WaitCounter(&counter);
    //for (int i = 0; i < JobCount; i++)
    //{
    //    JobPrint((void*)(intptr_t)i);
    //}
    //printf("Jobs done! Time: %ums - Total print: %d\n", GetTickCount() - startTime, printTime.value);
    //
    //JobSystem::Shutdown();

    HashTable<int> hashTable;
    hashTable.SetValue(0, 100);

    int value;
    if (hashTable.TryGetValue(0, &value) && value == 100) 
    {
        printf("Test hashTable success.");
    }

    static char buffer[1024 * 1024];
    AsyncFile* file = FileSystem::ReadFileAsync("D:/a.out.js", buffer, sizeof(buffer));
    if (file)
    {
        file->Wait();

        buffer[file->length] = 0;
        printf("Read Content: %s\n", buffer);
    }
    else
    {
        printf("Read file failed!\n");
    }

    if (!Window::Init("Mojo", 1280, 720)) 
    {
        return 1;
    }

    const char* vertexShader =
        "#version 330 core\n"
        "layout (location = 0) in vec2 vertex;"
        "uniform mat4 MVP;"
        "void main() {"
        "gl_Position = MVP * vec4(vertex, 0, 1.0);"
        "}";

    const char* pixelsShader =
        "#version 330 core\n"
        "out vec4 pixelColor;"
        "void main() {"
        "pixelColor = vec4(1.0);"
        "}";

    mat4 mvp = mat4::ortho(-1, 1, -1, 1, -1.0f, 1.0f);

    Shader shader = Shader::Create(vertexShader, pixelsShader);

    vec2 vertices[] = {
        vec2(-0.5f, -0.5f),
        vec2( 0.0f,  0.5f),
        vec2( 0.5f, -0.5f),
    };

    VertexArray vertexArray = VertexArray::Create();

    VertexBuffer vertexBuffer = VertexBuffer::Create();
    vertexBuffer.SetData(vertices, sizeof(vertices), BufferUsage::StaticDraw);

    vertexArray.SetAttribute(vertexBuffer, shader.FindAttribute("vertex"), 2, DataType::Float, false, sizeof(float) * 2);

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

        Window::ClearBuffer();

        GL::DrawArrays(DrawType::Triangles, shader, vertexArray, 3, 0);

        Window::SwapBuffers();
    }

    Window::Shutdown();
    return 0;
}