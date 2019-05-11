#include <Mojo/Graphics/Material.h>

inline namespace Mojo
{
    /*
    struct Material
    {
        Shader              _shader;
        bool                _isDirty;
        HashTable<Value>    _uniforms;
    };*/

    Material Material::Create(const Shader& shader)
    {
        Material material;
        material._shader = shader;
        material._isDirty = false;

        return (Material&&)material;
    }

    void Material::Destroy(Material& material)
    {

    }

    void Material::SetFloat(const char* name, float value)
    {
        int location = this->_shader.FindUniform(name);
        if (location > -1)
        {
            //_uniforms.SetValue(location, Value(value));
        }
    }

    void Material::SetVector2(const char* name, const float2& value)
    {

    }

    void Material::SetVector3(const char* name, const float3& value)
    {

    }

    void Material::SetVector4(const char* name, const float4& value)
    {

    }

    void Material::SetMatrix4(const char* name, const float4x4& value)
    {

    }
}