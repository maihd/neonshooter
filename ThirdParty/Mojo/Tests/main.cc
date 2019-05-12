#include <Mojo/Math.h>
#include <Mojo/Native/System.h>
#include <Mojo/Core/Coroutine.h>
#include <Mojo/JobSystem.h>
#include <Mojo/JobCounter.h>
#include <Mojo/FileSystem.h>

#include <Mojo/Mojo.h>
#include <Mojo/Graphics.h>
#include <Mojo/Graphics/Shader.h>
#include <Mojo/Graphics/Texture.h>

#include <Mojo/Native/Window.h>

#include <Mojo/Core/ConfigVar.h>
#include <Mojo/Core/HashTable.h>

#include <stdio.h>
#include <stdint.h>

static ConfigVar _windowTitle("Window::Title", "Title of window", 0, "Mojo Tests");
static ConfigVar _windowWidth("Window::Width", "Title of window", 0, 960);
static ConfigVar _windowHeight("Window::Height", "Title of window", 0, 540);

static ConfigVar _graphicsMultisamples("Window::Multisamples", "", 0, 16);

struct GameEngine : Engine
{   
    Shader      shader;
    VertexArray vertexArray;

    bool Setup(void) override
    {
        const char* vertexShader =
            "#version 330 core\n"
            "layout (location = 0) in vec2 vertex;"
            "void main() {"
            "gl_Position = vec4(vertex, 0, 1.0);"
            "}";

        const char* pixelsShader =
            "#version 330 core\n"
            "out vec4 pixelColor;"
            "void main() {"
            "pixelColor = vec4(1.0);"
            "}";

        shader = Shader::Create(vertexShader, pixelsShader);

        float2 vertices[] = {
            float2(-0.5f, -0.5f),
            float2(0.0f,  0.5f),
            float2(0.5f, -0.5f),
        };

        vertexArray = VertexArray::Create();

        VertexBuffer vertexBuffer = VertexBuffer::Create();
        vertexBuffer.SetData(vertices, sizeof(vertices), BufferUsage::StaticDraw);

        vertexArray.SetAttribute(vertexBuffer, shader.FindAttribute("vertex"), 2, DataType::Float, false, sizeof(float) * 2);

        return true;
    }

    void Shutdown(void) override
    {
        
    }

    void Update(void) override
    {

    }

    void Render(void) override
    {
        Graphics::BindShader(shader);
        Graphics::BindVertexArray(vertexArray);
        Graphics::DrawArrays(DrawType::Triangles, 3, 0);
        Graphics::BindVertexArray(0);
        Graphics::BindShader(0);
    }
};

int main(void)
{
    GameEngine engine;
    return Mojo::RunEngine(&engine);
}