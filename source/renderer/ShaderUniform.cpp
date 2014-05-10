#include "ShaderUniform.h"

namespace f3d
{
namespace renderer
{
	CShaderUniform::CShaderUniform()
		: CObject()
		, m_uniformType( EShaderUniformType::eUniformInvalid )
		, m_uniformValue ( nullptr )
		, m_attribute ( "" )
	{
		m_type = EObjectType::eTypeShaderUniform;
	}

	CShaderUniform::~CShaderUniform()
	{
		deallocMemory();
	}

	void CShaderUniform::setUniform( EShaderUniformType type, const std::string& attribute, void* value )
	{
		m_uniformType  = type;
		m_attribute    = attribute;
		m_uniformValue = allocMemory(type, value);
	}

	void* CShaderUniform::allocMemory( EShaderUniformType type, void* value )
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
}
}