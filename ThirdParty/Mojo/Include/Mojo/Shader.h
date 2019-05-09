#pragma once

inline namespace Mojo
{
    struct ShaderHandle;

    struct Shader
    {
        unsigned handle = 0;

        int FindAttribute(const char* name);
        int FindUniform(const char* name);

        bool SetFloat(int location, float x);
        bool SetFloat(const char* name, float x);
        
        bool SetFloat2(int location, float x, float y);
        bool SetFloat2(const char* name, float x, float y);
        
        bool SetFloat3(int location, float x, float y, float z);
        bool SetFloat3(const char* name, float x, float y, float z);
        
        bool SetFloat4(int location, float x, float y, float z, float w);
        bool SetFloat4(const char* name, float x, float y, float z, float w);

        bool SetFloat4x4(int location, const float* value, bool transpose = false);
        bool SetFloat4x4(const char* name, const float* value, bool transpose = false);

        static Shader Create(const char* vertexSource, const char* pixelsSource);
        static void   Destroy(Shader& shader);
        
        static Shader CreateEffect(const char* pixelsSource);

        inline operator ShaderHandle*(void) const
        {
            return (ShaderHandle*)(long long)handle;
        }
    };
}