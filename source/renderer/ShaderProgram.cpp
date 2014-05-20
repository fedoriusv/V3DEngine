#include "ShaderProgram.h"

using namespace f3d;
using namespace f3d::renderer;

CShaderProgram::CShaderProgram()
	: CObject()
	, m_shaderProgID( -1 )
	, m_enable ( true )
	, m_isActive ( false )
{
	m_type = EObjectType::eTypeShaderProgram;
}

CShaderProgram::~CShaderProgram()
{
}
	
u32 CShaderProgram::getShaderID() const
{
	return m_shaderProgID;
}

bool CShaderProgram::isEnable() const
{
	return m_enable;
}

void CShaderProgram::setEnable( bool enable )
{
	m_enable = enable;
}

bool CShaderProgram::isExistAttribute( const std::string& attribute )
{
	UniformList::iterator iter = m_uniformList.begin();

	while (iter != m_uniformList.end())
	{
		if ( (*iter).first.compare(attribute)  == 0 )
		{
			return true;
		}
		++iter;
	}
	return false;
}

void CShaderProgram::addShader( ShaderPtr shaderProgram )
{
	if (shaderProgram)
	{
		m_shaderList.push_back(shaderProgram);
	}
}

void CShaderProgram::destroyShader( ShaderPtr shaderProgram )
{
	if (shaderProgram)
	{
		auto shader = std::find(m_shaderList.begin(), m_shaderList.end(), shaderProgram);
		if (shader == m_shaderList.end())
		{
			//assert "Shader Program not found";
			return;
		}
		//TODO : not final
		m_shaderList.erase(shader);
	}
}

void CShaderProgram::setUniformInt( const std::string& attribute, const u32 value )
{
	if ( isExistAttribute(attribute) )
	{
		m_uniformList[attribute]->setUniform( eUniformInt, attribute, (void*)(&value) );
	}
	else
	{
		UniformPtr uniform = std::make_shared<CShaderUniform>();

		uniform->setUniform( eUniformInt, attribute, (void*)(&value) );
		m_uniformList[attribute] = uniform;
	}
}

void CShaderProgram::setUniformFloat  ( const std::string& attribute, const f32 value )
{
	if ( isExistAttribute(attribute) )
	{
		m_uniformList[attribute]->setUniform( eUniformInt, attribute, (void*)(&value) );
	}
	else
	{
		UniformPtr uniform = std::make_shared<CShaderUniform>();

		uniform->setUniform( eUniformInt, attribute, (void*)(&value) );
		m_uniformList[attribute] = uniform;
	}
}

void CShaderProgram::setUniformVector2( const std::string& attribute, const core::Vector2D& vector )
{
	if ( isExistAttribute(attribute) )
	{
		m_uniformList[attribute]->setUniform( eUniformInt, attribute, (void*)(&vector) );
	}
	else
	{
		UniformPtr uniform = std::make_shared<CShaderUniform>();

		uniform->setUniform( eUniformInt, attribute, (void*)(&vector) );
		m_uniformList[attribute] = uniform;
	}
}

void CShaderProgram::setUniformVector3( const std::string& attribute, const core::Vector3D& vector )
{
	if ( isExistAttribute(attribute) )
	{
		m_uniformList[attribute]->setUniform( eUniformInt, attribute, (void*)(&vector) );
	}
	else
	{
		UniformPtr uniform = std::make_shared<CShaderUniform>();

		uniform->setUniform( eUniformInt, attribute, (void*)(&vector) );
		m_uniformList[attribute] = uniform;
	}
}

void CShaderProgram::setUniformVector4( const std::string& attribute, const core::Vector4D& vector )
{
	if ( isExistAttribute(attribute) )
	{
		m_uniformList[attribute]->setUniform( eUniformInt, attribute, (void*)(&vector) );
	}
	else
	{
		UniformPtr uniform = std::make_shared<CShaderUniform>();

		uniform->setUniform( eUniformInt, attribute, (void*)(&vector) );
		m_uniformList[attribute] = uniform;
	}
}

void CShaderProgram::setUniformMatrix3( const std::string& attribute, const core::Matrix3D& matrix )
{
	if ( isExistAttribute(attribute) )
	{
		m_uniformList[attribute]->setUniform( eUniformInt, attribute, (void*)(&matrix) );
	}
	else
	{
		UniformPtr uniform = std::make_shared<CShaderUniform>();

		uniform->setUniform( eUniformInt, attribute, (void*)(&matrix) );
		m_uniformList[attribute] = uniform;
	}
}

void CShaderProgram::setUniformMatrix4( const std::string& attribute, const core::Vector4D& matrix )
{
	if ( isExistAttribute(attribute) )
	{
		m_uniformList[attribute]->setUniform( eUniformInt, attribute, (void*)(&matrix) );
	}
	else
	{
		UniformPtr uniform = std::make_shared<CShaderUniform>();

		uniform->setUniform( eUniformInt, attribute, (void*)(&matrix) );
		m_uniformList[attribute] = uniform;
	}
}

void CShaderProgram::setDefaultUniform(const std::string& name, EShaderUniformType type, EDefaultShaderUniforms val)
{
    if (isExistAttribute(name))
    {
        m_uniformList[name]->setUniform(type, name, 0, val);
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(type, name, 0, val);
        m_uniformList[name] = uniform;
    }
}
