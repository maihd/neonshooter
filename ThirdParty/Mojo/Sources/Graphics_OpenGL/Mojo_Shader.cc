#include <stdint.h>
#include <assert.h>
#include <GL/glew.h>

#include <Mojo/Graphics/Shader.h>

namespace Mojo
{
    namespace
    {
        const char* _defaultEffectVertexSource =
            "#version 330 core\n"
            "layout (location = 0) in vec4 vertex;"
            "out vec2 VertexTexCoord;"
            "void main() {"
            "VertexTexCoord = vertex.zw;"
            "gl_Position = vec4(vertex.xy, 0, 1.0);"
            "}";

        const char*  _defaultVertexSource =
            "#version 330 core\n"
            "layout (location = 0) in vec3 position;"
            "layout (location = 1) in vec2 texcoord;"
            "layout (location = 2) in vec4 color;"
            
            "out     vec2 VaryingTexCoord;"
            "out     vec4 VaryingColor;"

            "uniform vec4 MainColor;"
            "uniform mat4 TransformMatrix;"
            "uniform mat4 ProjectionMatrix;"
            
            "vec4 VertexPosition;"
            "vec2 VertexTexCoord;"
            "vec4 VertexColor;"
            
            "void main() {"
            "VertexPosition  = vec4(position, 1.0);"
            "VertexTexCoord  = texcoord;"
            "VertexColor     = color;"

            "VaryingTexCoord = VertexTexCoord;"
            "VaryingColor    = MainColor * VertexColor;"
            
            "gl_Position     = ProjectionMatrix * TransformMatrix * VertexPosition;"
            
            "}";

        const char*  _defaultPixelsSource =
            "#version 330 core\n"

            "in      vec2       VaryingTexCoord;"
            "in      vec4       VaryingColor;"

            "uniform vec4       MainColor;"
            "uniform sampler2D  MainTexture;"

            "out     vec4       ResultColor;"

            "void main() {"
            "ResultColor = texture(MainTexture, VaryingTexCoord) * VaryingColor;"
            "}";

        static GLuint CreateGLShader(GLenum type, const char* src)
        {
            GLuint shader = glCreateShader(type);
            if (!shader)
            {
                const char* error = (const char*)glewGetErrorString(glGetError());
                return 0;
            }

            glShaderSource(shader, 1, &src, 0);
            glCompileShader(shader);

            GLint status;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
            if (!status)
            {
                char errorLog[1024];
                glGetShaderInfoLog(shader, sizeof(errorLog), 0, errorLog);

                glDeleteShader(shader);
                return 0;
            }
            else
            {
                return shader;
            }
        }

        static GLuint CreateGLProgram(GLuint vshader, GLuint fshader)
        {
            if (!vshader || !fshader)
            {
                return 0;
            }

            GLuint program = glCreateProgram();
            if (!program)
            {
                return 0;
            }

            glAttachShader(program, vshader);
            glAttachShader(program, fshader);
            glLinkProgram(program);

            GLint status;
            glGetProgramiv(program, GL_LINK_STATUS, &status);
            if (!status)
            {
                char errorLog[1024];
                glGetProgramInfoLog(program, sizeof(errorLog), 0, errorLog);
                glDeleteProgram(program);
                return 0;
            }
            else
            {
                return program;
            }
        }
    }

    Shader Shader::Create(const char* vertexSource, const char* pixelsSource)
    {
        Shader shader = {};

        if (!vertexSource)
        {
            vertexSource = _defaultVertexSource;
        }

        if (!pixelsSource)
        {
            pixelsSource = _defaultPixelsSource;
        }

        GLuint vshader = CreateGLShader(GL_VERTEX_SHADER, vertexSource);
        if (!vshader)
        {
            return shader;
        }

        GLuint fshader = CreateGLShader(GL_FRAGMENT_SHADER, pixelsSource);
        if (!fshader)
        {
            return shader;
        }

        GLuint program = CreateGLProgram(vshader, fshader);
        if (!program)
        {
            return shader;
        }

        shader.handle = program;
        return shader;
    }

    Shader Shader::CreateEffect(const char* pixelsSource)
    {
        return Shader::Create(_defaultEffectVertexSource, pixelsSource);
    }

    void Shader::Destroy(Shader& shader)
    {
        glDeleteProgram(shader.handle);
        shader.handle = 0;
    }

    int Shader::FindAttribute(const char* name)
    {
        return glGetAttribLocation(handle, name);
    }

    int Shader::FindUniform(const char* name)
    {
        return glGetUniformLocation(handle, name);
    }

    bool Shader::SetFloat(int location, float x)
    {
        if (location < 0)
        {
            return false;
        }

        glUniform1f(location, x);
        //glProgramUniform1f(handle, location, x);
        return true;
    }

    bool Shader::SetFloat(const char* name, float x)
    {
        return this->SetFloat(this->FindUniform(name), x);
    }

    bool Shader::SetFloat2(int location, float x, float y)
    {
        if (location < 0)
        {
            return false;
        }

        glUniform2f(location, x, y);
        //glProgramUniform2f(handle, location, x, y);
        return true;
    }

    bool Shader::SetFloat2(const char* name, float x, float y)
    {
        return this->SetFloat2(this->FindUniform(name), x, y);
    }

    bool Shader::SetVector3(int location, float x, float y, float z)
    {
        if (location < 0)
        {
            return false;
        }

        glUniform3f(location, x, y, z);
        //glProgramUniform3f(handle, location, x, y, z);
        return true;
    }

    bool Shader::SetVector3(const char* name, float x, float y, float z)
    {
        return this->SetVector3(this->FindUniform(name), x, y, z);
    }

    bool Shader::SetVector4(int location, float x, float y, float z, float w)
    {
        if (location < 0)
        {
            return false;
        }

        glUniform4f(location, x, y, z, w);
        //glProgramUniform4f(handle, location, x, y, z, w);
        return true;
    }

    bool Shader::SetVector4(const char* name, float x, float y, float z, float w)
    {
        return this->SetVector4(this->FindUniform(name), x, y, z, w);
    }

    bool Shader::SetMatrix4(int location, const float* value, bool transpose)
    {
        if (location < 0)
        {
            return false;
        }

        glUniformMatrix4fv(location, 1, transpose, value);
        //glProgramUniformMatrix4fv(handle, location, 1, transpose, value);
        return true;
    }

    bool Shader::SetMatrix4(const char* name, const float* value, bool transpose)
    {
        return this->SetMatrix4(this->FindUniform(name), value, transpose);
    }

    bool Shader::SetTransform(const Matrix4& matrix)
    {
        return this->SetMatrix4("TransformMatrix", (float*)&matrix, false);
    }

    bool Shader::SetProjection(const Matrix4& matrix)
    {
        return this->SetMatrix4("ProjectionMatrix", (float*)&matrix, false);
    }

    bool Shader::SetMainColor(const Vector4& color)
    {
        return this->SetVector4("MainColor", color.x, color.y, color.z, color.w);
    }

    bool Shader::SetMainTexture(const Texture& texture)
    {
        return true;
    }
}