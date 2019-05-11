#pragma once

#include <Mojo/Math.h>
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

            quat        asQuat;
            float2      asFloat2;
            float3      asFloat3;
            float4      asFloat4;
            float4x4    asFloat4x4;   
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

        void SetVector2(const char* name, const float2& value);
        void SetVector3(const char* name, const float3& value);
        void SetVector4(const char* name, const float4& value);

        void SetMatrix4(const char* name, const float4x4& value);
    };
}