#include "ShaderProgram.h"
#include "ShaderUniform.h"
#include "utils/Logger.h"
#include "Engine.h"

using namespace v3d;
using namespace renderer;

CShaderProgram::CShaderProgram(const ShaderDataPtr& data)
    : m_shaderProgID(0)
    , m_enable(true)
    , m_shaderData(data)
{
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

void CShaderProgram::addShader(const ShaderPtr& shader)
{
    if (shader)
    {
        m_shaderList.push_back(shader);
    }
}

void CShaderProgram::destroyShader(const ShaderPtr& shader)
{
    if (shader)
    {
        auto current = std::find(m_shaderList.begin(), m_shaderList.end(), shader);
        if (current == m_shaderList.end())
        {
            LOG_WARNING("CShaderProgram: Shader Program not found");
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

bool CShaderProgram::create(const std::string& vertex, const std::string& fragment, u32 arg, ...)
{
    if (vertex.empty() || fragment.empty())
    {
        ASSERT(false && "Empty Shader file name");
        return false;
    }

    ShaderPtr vshader = RENDERER->makeSharedShader();
    vshader->create(vertex, CShader::eVertex);
    CShaderProgram::addShader(vshader);

    ShaderPtr fshader = RENDERER->makeSharedShader();
    fshader->create(fragment, CShader::eFragment);
    CShaderProgram::addShader(fshader);

    va_list argList;
    va_start(argList, arg);
    for (u32 i = 0; i < arg; i += 2)
    {
        char* strName = va_arg(argList, char*);
        int type = va_arg(argList, int);

        ShaderPtr shader = RENDERER->makeSharedShader();
        shader->create(strName, (CShader::EShaderType)type);
        CShaderProgram::addShader(shader);
    }
    va_end(argList);

    bool status = create();

    return status;
}
