#include "ShaderUniform.h"

using namespace v3d;
using namespace v3d::renderer;


CShaderUniform::CShaderUniform()
: m_uniformType(EDataType::eTypeNone)
	, m_uniformValue (nullptr)
	, m_attribute ("")
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
