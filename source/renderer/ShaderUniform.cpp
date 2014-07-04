#include "ShaderUniform.h"

using namespace v3d;
using namespace v3d::renderer;

//////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string CShaderUniform::s_uniformName[EUniformData::eUniformsCount] = {

    "transform.projectionMatrix",
    "transform.modelMatrix",
    "transform.viewMatrix",
    "transform.normalMatrix",
    "transform.viewPosition",
    "transform.orthoMatrix",

    "material.ambient",
    "material.diffuse",
    "material.specular",
    "material.emission",
    "material.shininess",

    "lights.count",

    "light.position",
    "light.ambient",
    "light.diffuse",
    "light.specular",
    "light.direction",
    "light.attenuation",
    "light.radius",
};


const std::string& CShaderUniform::getNameByValue(EUniformData type)
{
    return s_uniformName[type];
}

const EUniformData CShaderUniform::getValueByName(const std::string& name)
{
    for (int i = 0; i < EUniformData::eUniformsCount; ++i)
    {
        if (s_uniformName[i].compare(name) == 0)
        {
            return (EUniformData)i;
        }
    }

    return EUniformData::eUniformUser;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////


CShaderUniform::CShaderUniform()
: m_uniformType(EDataType::eTypeNone)
    , m_uniformValue (nullptr)
    , m_attribute ("")
    , m_uniformData(EUniformData::eUniformUser)
{
}

CShaderUniform::~CShaderUniform()
{
	deallocMemory();
}

void CShaderUniform::setUniform(EDataType type, const std::string& attribute, void* value)
{
    m_uniformType  = type;
    m_attribute    = attribute;
    if (value)
    {
        m_uniformValue = allocMemory(type, value);
    }
}

void CShaderUniform::setUniform(const std::string& attribute, EUniformData data)
{
    m_attribute = attribute;
    m_uniformData = data;

}

void* CShaderUniform::allocMemory(EDataType type, void* value)
{
	deallocMemory();
	
	void* uniformValue = nullptr;

	switch(type)
	{
    case EDataType::eTypeInt:
		{
			uniformValue = new int();
			memcpy(uniformValue, value, sizeof(int));

			return uniformValue;
		}
    case EDataType::eTypeFloat:
		{
			uniformValue = new float();
			memcpy(uniformValue, value, sizeof(float));

			return uniformValue;
		}
    case EDataType::eTypeVector2:
		{
			uniformValue = new core::Vector2D();
			memcpy(uniformValue, value, sizeof(core::Vector2D));

			return uniformValue;
		}
    case EDataType::eTypeVector3:
		{
			uniformValue = new core::Vector3D();
			memcpy(uniformValue, value, sizeof(core::Vector3D));

			return uniformValue;
		}
    case EDataType::eTypeVector4:
		{
			uniformValue = new core::Vector4D();
			memcpy(uniformValue, value, sizeof(core::Vector4D));

			return uniformValue;
		}
    case EDataType::eTypeMatrix3:
		{
			uniformValue = new core::Matrix3D();
			memcpy(uniformValue, value, sizeof(core::Matrix3D));

			return uniformValue;
		}
    case EDataType::eTypeMatrix4:
		{
			uniformValue = new core::Matrix4D();
			memcpy(uniformValue, value, sizeof(core::Matrix4D));

			return uniformValue;
		}
	}

	return nullptr;
}

void CShaderUniform::deallocMemory()
{
	if (m_uniformValue != nullptr)
	{
		delete m_uniformValue;
		m_uniformValue = nullptr;
	}
}

CShaderUniform::EDataType CShaderUniform::getUniformType() const
{
    return m_uniformType;
}

void* CShaderUniform::getUniforValue() const
{
    return m_uniformValue;
}
