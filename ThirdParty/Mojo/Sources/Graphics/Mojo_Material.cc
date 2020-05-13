#include <Mojo/Graphics/Material.h>

namespace Mojo
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

    void Material::SetVector2(const char* name, const Vector2& value)
    {

    }

    void Material::SetVector3(const char* name, const Vector3& value)
    {

    }

    void Material::SetVector4(const char* name, const Vector4& value)
    {

    }

    void Material::SetMatrix4(const char* name, const Matrix4& value)
    {

    }
}