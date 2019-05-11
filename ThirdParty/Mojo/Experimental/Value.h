#pragma once

#include "./Core.h"
#include "./Array.h"
#include "./HashTable.h"

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

            Quat,
            Float2,
            Float3,
            Float4,
            Float4x4,

            //Array,
            //Table,
            //String,
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

            //Mojo::String            asString;
            //Mojo::Array<Value>      asArray;  
            //Mojo::HashTable<Value>  asTable;          
        };

        inline Value(void)
            : type(Null)
        {    
        }

        inline Value(float value)
            : type(Float)
            , asFloat(value)
        {    
        }
    };
}