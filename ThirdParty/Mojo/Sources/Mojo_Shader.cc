#include <stdint.h>
#include <assert.h>
#include <GL/glew.h>

#include <Mojo/Shader.h>

inline namespace Mojo
{
    namespace
    {
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

        shader._handle = program;
        return shader;
    }

    void Shader::Destroy(Shader& shader)
    {
        glDeleteProgram(shader._handle);
        shader._handle = 0;
    }

    int Shader::FindAttribute(const char* name)
    {
        return glGetAttribLocation(_handle, name);
    }

    int Shader::FindUniform(const char* name)
    {
        return glGetUniformLocation(_handle, name);
    }

    bool Shader::SetFloat(int location, float x)
    {
        if (location < 0)
        {
            return false;
        }

        glProgramUniform1f(_handle, location, x);
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

        glProgramUniform2f(_handle, location, x, y);
        return true;
    }

    bool Shader::SetFloat2(const char* name, float x, float y)
    {
        return this->SetFloat2(this->FindUniform(name), x, y);
    }

    bool Shader::SetFloat3(int location, float x, float y, float z)
    {
        if (location < 0)
        {
            return false;
        }

        glProgramUniform3f(_handle, location, x, y, z);
        return true;
    }

    bool Shader::SetFloat3(const char* name, float x, float y, float z)
    {
        return this->SetFloat3(this->FindUniform(name), x, y, z);
    }

    bool Shader::SetFloat4(int location, float x, float y, float z, float w)
    {
        if (location < 0)
        {
            return false;
        }

        glUniform4f(location, x, y, z, w);
        //::glProgramUniform4f(_handle, location, x, y, z, w);
        return true;
    }

    bool Shader::SetFloat4(const char* name, float x, float y, float z, float w)
    {
        return this->SetFloat4(this->FindUniform(name), x, y, z, w);
    }

    bool Shader::SetFloat4x4(int location, const float* value, bool transpose)
    {
        if (location < 0)
        {
            return false;
        }

        glUniformMatrix4fv(location, 1, transpose, value);
        //::glProgramUniformMatrix4fv(_handle, location, 1, transpose, value);
        return true;
    }

    bool Shader::SetFloat4x4(const char* name, const float* value, bool transpose)
    {
        return this->SetFloat4x4(this->FindUniform(name), value, transpose);
    }
}