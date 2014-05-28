#include "ShaderUniform.h"

using namespace f3d;
using namespace f3d::renderer;

//////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string k_shaderDataType[EShaderDataType::eUniformTypeCount] = {
    "",
    "int",
    "float",
    "vec2",
    "vec3",
    "vec4",
    "mat3",
    "mat4"
};

const std::string& getShaderDataNameByType(EShaderDataType type)
{
    return k_shaderDataType[type];
}

const EShaderDataType getShaderDataTypeByName(const std::string& name)
{
    for (int i = 0; i < EShaderDataType::eUniformTypeCount; ++i)
    {
        if (k_shaderDataType[i].compare(name) == 0)
        {
            return (EShaderDataType)i;
        }
    }

    return eUniformNone;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string CShaderUniform::s_shaderUniformName[EDefaultUniformData::eUniformsCount] = {

    "transform.projectionMatrix",
    "transform.modelMatrix",
    "transform.viewMatrix",
    "transform.normalMatrix",
    "transform.viewPosition",
    "transform.orthoMatrix"

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
	: CObject()
    , m_uniformType(EShaderDataType::eUniformNone)
    , m_defaultData(EDefaultUniformData::eUserUniform)
	, m_uniformValue (nullptr)
	, m_attribute ("")
{
	m_type = EObjectType::eTypeShaderUniform;
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
    if (data == EDefaultUniformData::eUserUniform)
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
	case eUniformInt:
		{
			uniformValue = new int();
			memcpy(uniformValue, value, sizeof(int));

			return uniformValue;
		}
	case eUniformFloat:
		{
			uniformValue = new float();
			memcpy(uniformValue, value, sizeof(float));

			return uniformValue;
		}
	case eUniformVector2:
		{
			uniformValue = new core::Vector2D();
			memcpy(uniformValue, value, sizeof(core::Vector2D));

			return uniformValue;
		}
	case eUniformVector3:
		{
			uniformValue = new core::Vector3D();
			memcpy(uniformValue, value, sizeof(core::Vector3D));

			return uniformValue;
		}
	case eUniformVector4:
		{
			uniformValue = new core::Vector4D();
			memcpy(uniformValue, value, sizeof(core::Vector4D));

			return uniformValue;
		}
	case eUniformMatrix3:
		{
			uniformValue = new core::Matrix3D();
			memcpy(uniformValue, value, sizeof(core::Matrix3D));

			return uniformValue;
		}
	case eUniformMatrix4:
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

