#include "Shader.h"

namespace f3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    const std::string k_shaderTypeName[EShaderType::eShaderTypeCount] = {

        "vertex",
        "fragment",
        "geometry"
    };

    const std::string& getShaderTypeNameByType(EShaderType type)
    {
        return k_shaderTypeName[type];
    }

    EShaderType getShaderTypeByName(const std::string& name)
    {
        for (int i = 0; i < eShaderTypeCount; ++i)
        {
            if (k_shaderTypeName[i].compare(name) == 0)
            {
                return (EShaderType)i;
            }
        }

        return EShaderType::eTypeUnknown;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////

	CShader::CShader()
		: CObject()
		, m_shaderID( -1 )
		, m_shaderType( EShaderType::eTypeUnknown )
		, m_compileStatus( false )
		, m_data( nullptr )
	{
		m_type = EObjectType::eTypeShader;
	}

	CShader::~CShader()
	{
		if (m_data != nullptr)
		{
			free(m_data);
			m_data = nullptr;
		}
	}

	u32 CShader::getShaderID() const
	{
		return m_shaderID;
	}

	EShaderType CShader::getShaderType() const
	{
		return m_shaderType;
	}

	bool CShader::getCompileStatus() const
	{
		return m_compileStatus;
	}
}
}