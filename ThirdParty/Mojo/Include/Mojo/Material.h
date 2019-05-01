#pragma once

#include <Mojo/GL.h>
#include <Mojo/Math.h>
#include <Mojo/HashTable.h>

inline namespace Mojo
{
    struct Value
    {
        enum Type
        {
            Null,
            
            Int,
            Uint,
            Float,
            Double,

            Vector2,
            Vector3,
            Vector4,
            Matrix4,
        };

        Type _type;

        union
        {
            int      asInt;
            unsigned asUint;
            float    asFloat;
            double   asDouble;

            vec2     asVector2;
            vec3     asVector3;
            vec4     asVector4;
            mat4     asMatrix4;
        };

        Value(float value);
    };

    struct Material
    {
        Shader              _shader;
        bool                _isDirty;
        HashTable<Value>    _uniforms;

        static Material Create(const Shader& shader);
        static void     Destroy(Material& material);

        void SetFloat(const char* name, float value);

        void SetVector2(const char* name, const vec2& value);
        void SetVector3(const char* name, const vec3& value);
        void SetVector4(const char* name, const vec4& value);

        void SetMatrix4(const char* name, const mat4& value);
    };
}