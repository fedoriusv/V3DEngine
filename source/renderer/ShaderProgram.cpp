#include "ShaderProgram.h"
#include "ShaderUniform.h"
#include "utils/Logger.h"

using namespace v3d;
using namespace v3d::renderer;

CShaderProgram::CShaderProgram(const ShaderDataPtr& data)
    : CObject()
    , m_shaderProgID(0)
    , m_enable(true)
    , m_isActive(false)
    , m_shaderData(data)
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

void CShaderProgram::setEnable(bool enable)
{
    m_enable = enable;
}

void CShaderProgram::addShader(ShaderPtr shader)
{
    if (shader)
    {
        m_shaderList.push_back(shader);
    }
}

void CShaderProgram::destroyShader(ShaderPtr shader)
{
    if (shader)
    {
        auto current = std::find(m_shaderList.begin(), m_shaderList.end(), shader);
        if (current == m_shaderList.end())
        {
            LOG_ERROR("DestroyShader : Shader Program not found");
            return;
        }

        m_shaderList.erase(current);
    }
}

void CShaderProgram::getShaderIDArray(std::vector<u32>& shaders)
{
    for (u32 i = 0; i < m_shaderList.size(); ++i)
    {
        if (m_shaderList[i]->getCompileStatus())
        {
            shaders.push_back(m_shaderList[i]->getShaderID());
        }
    }
}
