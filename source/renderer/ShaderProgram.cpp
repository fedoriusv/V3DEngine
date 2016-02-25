#include "ShaderProgram.h"
#include "ShaderUniform.h"
#include "utils/Logger.h"
#include "Engine.h"

namespace v3d
{
namespace renderer
{

CShaderProgram::CShaderProgram()
    : m_enable(true)
    , m_compileStatus(false)
{
}

CShaderProgram::~CShaderProgram()
{
    m_shaderList.clear();
    m_shaderDataList.clear();
    m_varyingsList.clear();
}

bool CShaderProgram::isEnable() const
{
    return m_enable;
}

void CShaderProgram::setEnable(bool enable)
{
    m_enable = enable;
}

bool CShaderProgram::getCompileStatus() const
{
    return m_compileStatus;
}

void CShaderProgram::attachShader(const ShaderPtr& shader)
{
    if (shader)
    {
        m_shaderList.push_back(shader);
        m_compileStatus = false;
    }
}

void CShaderProgram::detachShader(const ShaderPtr& shader)
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
        m_compileStatus = false;
    }
}

void CShaderProgram::addShaderData(const ShaderDataPtr& data)
{
    if (data)
    {
        m_shaderDataList.push_back(data);
        m_compileStatus = false;
    }
}

void CShaderProgram::addVaryingsAttibutes(const std::vector<const c8*>& list)
{
    if (!list.empty())
    {
        m_varyingsList = list;
        m_compileStatus = false;
    }
}

bool CShaderProgram::create(const std::string& vertex, const std::string& fragment, u32 arg, ...)
{
    if (vertex.empty() || fragment.empty())
    {
        ASSERT(false, "Empty Shader file name");
        return false;
    }

    ShaderPtr vshader = RENDERER->makeSharedShader();
    vshader->create(vertex, CShader::eVertex);
    CShaderProgram::attachShader(vshader);

    ShaderPtr fshader = RENDERER->makeSharedShader();
    fshader->create(fragment, CShader::eFragment);
    CShaderProgram::attachShader(fshader);

    va_list argList;
    va_start(argList, arg);
    for (u32 i = 0; i < arg; i += 2)
    {
        char* strName = va_arg(argList, char*);
        int type = va_arg(argList, int);

        ShaderPtr shader = RENDERER->makeSharedShader();
        shader->create(strName, (CShader::EShaderType)type);
        CShaderProgram::attachShader(shader);
    }
    va_end(argList);

    m_compileStatus = create();

    return m_compileStatus;
}

} //namespace renderer
} //namespace v3d
