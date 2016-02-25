#include "Shader.h"
#include "utils/Logger.h"
#include "scene/ShaderManager.h"


#include "tinyxml2.h"

namespace v3d
{
namespace renderer
{

using namespace stream;
using namespace scene;
using namespace resources;

/////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string CShader::s_shaderTypeName[eShaderCount] = {

    "vertex",
    "fragment",
    "geometry",
    "compute"
};

const std::string& CShader::getShaderTypeNameByType(EShaderType type)
{
    return s_shaderTypeName[type];
}

CShader::EShaderType CShader::getShaderTypeByName(const std::string& name)
{
    for (u32 i = 0; i < eShaderCount; ++i)
    {
        if (s_shaderTypeName[i].compare(name) == 0)
        {
            return (EShaderType)i;
        }
    }

    return eShaderUnknown;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

CShader::CShader()
    : m_type(eShaderUnknown)
    , m_name("")
    , m_data("")

    , m_compileStatus(false)
{
}

CShader::~CShader()
{
}

CShader::EShaderType CShader::getShaderType() const
{
    return m_type;
}

bool CShader::getCompileStatus() const
{
    return m_compileStatus;
}

const std::string& CShader::getName() const
{
    return m_name;
}

void CShader::setName(const std::string& name)
{
    m_name = name;
}

bool CShader::parse(const tinyxml2::XMLElement* root)
{
    m_data.clear();

    if (!root)
    {
        LOG_ERROR("CShader: Not exist xml shader element");
        return false;
    }

    if (!root->Attribute("name"))
    {
        LOG_ERROR("CShader: Empty shader name");
        return false;
    }
    const std::string shaderName = root->Attribute("name");
    CShader::setName(shaderName);


    if (!root->Attribute("type"))
    {
        LOG_ERROR("CShader: Empty shader type");
        return false;
    }
    else
    {
        const std::string shaderType = root->Attribute("type");
        m_type = CShader::getShaderTypeByName(shaderType);
    }

    if (!root->Attribute("file"))
    {
        LOG_INFO("CShader: Create shader [%s] from data", shaderName.c_str());
        const std::string shaderBody = root->GetText();
        if (shaderBody.empty())
        {
            LOG_ERROR("CShader: Empty shader body");
            return false;
        }

        m_data = shaderBody;
    }
    else
    {
        const std::string shaderPath = root->Attribute("file");
        LOG_INFO("CShader: Create shader from file: %s", shaderPath.c_str());

        const CShaderSourceData* source = CShaderManager::getInstance()->load(shaderPath);
        if (!source)
        {
            LOG_ERROR("CShader: Error load shader %s", shaderPath.c_str());
            return false;
        }

        m_data = source->getBody();
        if (m_data.empty())
        {
            LOG_ERROR("CShader: Error load shader %s", shaderPath.c_str());
            return false;
        }
    }

    return true;
}

} //namespace renderer
} //namespace v3d
