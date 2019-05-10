#pragma once

#include <Mojo/Math.h>
#include <Mojo/Shader.h>
#include <Mojo/HashTable.h>

inline namespace Mojo
{
    struct Value
    {
        enum Type
        {
            Null,
            
            Int32,
            Uint32,
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

            float2     asVector2;
            float3     asVector3;
            float4     asVector4;
            float4x4     asMatrix4;
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

        void SetVector2(const char* name, const float2& value);
        void SetVector3(const char* name, const float3& value);
        void SetVector4(const char* name, const float4& value);

        void SetMatrix4(const char* name, const float4x4& value);
    };
}