#pragma once

#include <Mojo/Core/Types.h>

inline namespace Mojo
{
    struct Texture;

    // Graphics shader
    // @note: use for graphics only, compute shader is ComputeShader
    struct Shader
    {
        Handle handle = 0;

        int FindAttribute(const char* name);
        int FindUniform(const char* name);

        bool SetFloat(int location, float x);
        bool SetFloat(const char* name, float x);
        
        bool SetFloat2(int location, float x, float y);
        bool SetFloat2(const char* name, float x, float y);
        
        bool SetVector3(int location, float x, float y, float z);
        bool SetVector3(const char* name, float x, float y, float z);
        
        bool SetVector4(int location, float x, float y, float z, float w);
        bool SetVector4(const char* name, float x, float y, float z, float w);

        bool SetMatrix4(int location, const float* value, bool transpose = false);
        bool SetMatrix4(const char* name, const float* value, bool transpose = false);

        bool SetTransform(const Matrix4& matrix);
        bool SetProjection(const Matrix4& matrix);

        bool SetMainColor(const Vector4& color);
        bool SetMainTexture(const Texture& texture);

        //-------------------------------------------------
        // Create and destroy shaders
        //-------------------------------------------------

        static Shader Create(const char* vertexSource = 0, const char* pixelsSource = 0);
        static void   Destroy(Shader& shader);
        
        static Shader CreateEffect(const char* pixelsSource);
    };
}