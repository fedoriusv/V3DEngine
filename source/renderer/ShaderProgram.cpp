#include "ShaderProgram.h"
#include "ShaderUniform.h"
#include "utils/Logger.h"

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
            LOG_ERROR("DestroyShader : Shader Program not found");
            return;
		}

		//TODO : not final
		m_shaderList.erase(shader);
	}
}