#pragma once

#include <Mojo/Core/Types.h>
#include <Mojo/Core/HashTable.h>
#include <Mojo/Graphics/Shader.h>

inline namespace Mojo
{
    struct UniformValue
    {
        enum Type
        {
            Null,
            
            Int32,
            Uint32,
            Float,
            Double,

            Quat,
            Float2,
            Float3,
            Float4,
            Float4x4,
        };

        Type type;

        union
        {
            int         asInt;
            unsigned    asUint;
            float       asFloat;
            double      asDouble;

            Vector2      asFloat2;
            Vector3      asFloat3;
            Vector4      asFloat4;
            Matrix4    asFloat4x4;   
        };

        inline UniformValue(void)
            : type(Null)
        {    
        }

        inline UniformValue(float value)
            : type(Float)
            , asFloat(value)
        {    
        }
    };

    struct Material
    {
        Shader                      _shader;
        bool                        _isDirty;
        HashTable<UniformValue>     _uniforms;

        static Material Create(const Shader& shader);
        static void     Destroy(Material& material);

        void SetFloat(const char* name, float value);

        void SetVector2(const char* name, const Vector2& value);
        void SetVector3(const char* name, const Vector3& value);
        void SetVector4(const char* name, const Vector4& value);

        void SetMatrix4(const char* name, const Matrix4& value);
    };
}