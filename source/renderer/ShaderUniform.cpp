#include "ShaderUniform.h"

namespace f3d
{
namespace renderer
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    const std::string k_shaderUniformType[EShaderUniformDataType::eUniformTypeCount] = {
        "",
        "int",
        "float",
        "vec2",
        "vec3",
        "vec4",
        "mat3",
        "mat4"
    };

    const std::string& getShaderUniformNameByType(EShaderUniformDataType type)
    {
        return k_shaderUniformType[type];
    }

    const EShaderUniformDataType getShaderUniformTypeByName(const std::string& name)
    {
        for (int i = 0; i < EShaderUniformDataType::eUniformTypeCount; ++i)
        {
            if (k_shaderUniformType[i].compare(name) == 0)
            {
                return (EShaderUniformDataType)i;
            }
        }

        return eUniformNone;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    const std::string k_shaderUniformName[EDefaultShaderData::eDataCount] = {
        "",

        /*Uniforms*/
        "transform.projectionMatrix",
        "transform.modelMatrix",
        "transform.viewMatrix",
        "transform.normalMatrix",
        "transform.viewPosition",
        "transform.orthoMatrix"

        /*Attributes*/
        "mesh.positions",
        "mesh.colors",
        "mesh.normals",
        "mesh.benormals",
        "mesh.tangets",
        "mesh.texture0",
        "mesh.texture1",
        "mesh.texture2",
        "mesh.texture3",

    };


    const std::string& getShaderUniformNameByValue(EDefaultShaderData type)
    {
        return k_shaderUniformName[type];
    }

    const EDefaultShaderData getShaderUniformValueByName(const std::string& name)
    {
        for (int i = 0; i < EDefaultShaderData::eDataCount; ++i)
        {
            if (k_shaderUniformName[i].compare(name) == 0)
            {
                return (EDefaultShaderData)i;
            }
        }

        return eUserUniform;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////

	CShaderUniform::CShaderUniform()
		: CObject()
        , m_uniformType(EShaderUniformDataType::eUniformNone)
        , m_defaultData(EDefaultShaderData::eUserUniform)
		, m_uniformValue (nullptr)
		, m_attribute ("")
	{
		m_type = EObjectType::eTypeShaderUniform;
	}

	CShaderUniform::~CShaderUniform()
	{
		deallocMemory();
	}

    void CShaderUniform::setUniform(EShaderUniformDataType type, const std::string& attribute, void* value, EDefaultShaderData data)
	{
		m_uniformType  = type;
		m_attribute    = attribute;
        m_defaultData  = data;
        if (data != EDefaultShaderData::eUserUniform)
        {
            m_uniformValue = allocMemory(type, value);
        }
	}

    void* CShaderUniform::allocMemory(EShaderUniformDataType type, void* value)
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