#include "ShaderProgram.h"

using namespace f3d;
using namespace f3d::renderer;

CShaderProgram::CShaderProgram()
	: CObject()
	, m_shaderProgID(0)
	, m_enable(true)
	, m_isActive(false)
{
	m_type = EObjectType::eTypeShaderProgram;
}

CShaderProgram::~CShaderProgram()
{
    m_uniformList.clear();
    m_shaderList.clear();
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

bool CShaderProgram::isExist(const std::string& name)
{
	UniformList::iterator iter = m_uniformList.begin();

	while (iter != m_uniformList.end())
	{
        if ((*iter).first.compare(name) == 0)
		{
			return true;
		}
		++iter;
	}
	return false;
}

void CShaderProgram::addShader(ShaderPtr shaderProgram)
{
	if (shaderProgram)
	{
		m_shaderList.push_back(shaderProgram);
	}
}

void CShaderProgram::destroyShader(ShaderPtr shaderProgram)
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

void CShaderProgram::setUniformInt(const std::string& name, const u32 value)
{
    if (isExist(name))
	{
        m_uniformList[name]->setUniform(eUniformInt, name, (void*)(&value));
	}
	else
	{
		UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(eUniformInt, name, (void*)(&value));
        m_uniformList[name] = uniform;
	}
}

void CShaderProgram::setUniformFloat(const std::string& name, const f32 value)
{
    if (isExist(name))
	{
        m_uniformList[name]->setUniform(eUniformInt, name, (void*)(&value));
	}
	else
	{
		UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(eUniformInt, name, (void*)(&value));
        m_uniformList[name] = uniform;
	}
}

void CShaderProgram::setUniformVector2(const std::string& name, const core::Vector2D& vector)
{
    if (isExist(name))
	{
        m_uniformList[name]->setUniform(eUniformInt, name, (void*)(&vector));
	}
	else
	{
		UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(eUniformInt, name, (void*)(&vector));
        m_uniformList[name] = uniform;
	}
}

void CShaderProgram::setUniformVector3(const std::string& name, const core::Vector3D& vector)
{
    if (isExist(name))
	{
        m_uniformList[name]->setUniform(eUniformInt, name, (void*)(&vector));
	}
	else
	{
		UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(eUniformInt, name, (void*)(&vector));
        m_uniformList[name] = uniform;
	}
}

void CShaderProgram::setUniformVector4(const std::string& name, const core::Vector4D& vector)
{
    if (isExist(name))
	{
        m_uniformList[name]->setUniform(eUniformInt, name, (void*)(&vector));
	}
	else
	{
		UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(eUniformInt, name, (void*)(&vector));
        m_uniformList[name] = uniform;
	}
}

void CShaderProgram::setUniformMatrix3(const std::string& name, const core::Matrix3D& matrix)
{
    if (isExist(name))
	{
        m_uniformList[name]->setUniform(eUniformInt, name, (void*)(&matrix));
	}
	else
	{
		UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(eUniformInt, name, (void*)(&matrix));
        m_uniformList[name] = uniform;
	}
}

void CShaderProgram::setUniformMatrix4(const std::string& name, const core::Vector4D& matrix)
{
    if (isExist(name))
	{
        m_uniformList[name]->setUniform(eUniformInt, name, (void*)(&matrix));
	}
	else
	{
		UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(eUniformInt, name, (void*)(&matrix));
        m_uniformList[name] = uniform;
	}
}

void CShaderProgram::setDefaultUniform(const std::string& name, EShaderUniformDataType type, EDefaultShaderData data)
{
    if (isExist(name))
    {
        m_uniformList[name]->setUniform(type, name, 0, data);
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(type, name, nullptr, data);
        m_uniformList[name] = uniform;
    }
}
