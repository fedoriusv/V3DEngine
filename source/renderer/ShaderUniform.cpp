#include "ShaderUniform.h"

using namespace v3d;
using namespace v3d::renderer;

//////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string CShaderUniform::s_shaderUniformName[EDefaultUniformData::eUniformsCount] = {

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
    "material.shininess"

};


const std::string& CShaderUniform::getShaderUniformNameByValue(EDefaultUniformData type)
{
    return s_shaderUniformName[type];
}

const EDefaultUniformData CShaderUniform::getShaderUniformValueByName(const std::string& name)
{
    for (int i = 0; i < EDefaultUniformData::eUniformsCount; ++i)
    {
        if (s_shaderUniformName[i].compare(name) == 0)
        {
            return (EDefaultUniformData)i;
        }
    }

    return eUserUniform;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

CShaderUniform::CShaderUniform()
    : m_uniformType(EShaderDataType::eNone)
    , m_defaultData(EDefaultUniformData::eUserUniform)
	, m_uniformValue (nullptr)
	, m_attribute ("")
{
}

CShaderUniform::~CShaderUniform()
{
	deallocMemory();
}

void CShaderUniform::setUniform(EShaderDataType type, const std::string& attribute, void* value, EDefaultUniformData data)
{
	m_uniformType  = type;
	m_attribute    = attribute;
    m_defaultData  = data;
    if (value)
    {
        m_uniformValue = allocMemory(type, value);
    }
}

void* CShaderUniform::allocMemory(EShaderDataType type, void* value)
{
	deallocMemory();
	
	void* uniformValue = nullptr;

	switch(type)
	{
    case EShaderDataType::eInt:
		{
			uniformValue = new int();
			memcpy(uniformValue, value, sizeof(int));

			return uniformValue;
		}
    case EShaderDataType::eFloat:
		{
			uniformValue = new float();
			memcpy(uniformValue, value, sizeof(float));

			return uniformValue;
		}
    case EShaderDataType::eVector2:
		{
			uniformValue = new core::Vector2D();
			memcpy(uniformValue, value, sizeof(core::Vector2D));

			return uniformValue;
		}
    case EShaderDataType::eVector3:
		{
			uniformValue = new core::Vector3D();
			memcpy(uniformValue, value, sizeof(core::Vector3D));

			return uniformValue;
		}
    case EShaderDataType::eVector4:
		{
			uniformValue = new core::Vector4D();
			memcpy(uniformValue, value, sizeof(core::Vector4D));

			return uniformValue;
		}
    case EShaderDataType::eMatrix3:
		{
			uniformValue = new core::Matrix3D();
			memcpy(uniformValue, value, sizeof(core::Matrix3D));

			return uniformValue;
		}
    case EShaderDataType::eMatrix4:
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

EShaderDataType CShaderUniform::getUniformType() const
{
    return m_uniformType;
}

void* CShaderUniform::getUniforValue() const
{
    return m_uniformValue;
}

