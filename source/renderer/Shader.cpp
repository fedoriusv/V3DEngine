#include "Shader.h"
#include "utils/Logger.h"
#include "scene/ShaderManager.h"


#include "tinyxml2.h"

using namespace v3d;
using namespace renderer;
using namespace stream;
using namespace scene;
using namespace resources;

/////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string CShader::s_shaderTypeName[EShaderType::eShaderCount] = {

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
    for (int i = 0; i < eShaderCount; ++i)
    {
        if (s_shaderTypeName[i].compare(name) == 0)
        {
            return (EShaderType)i;
        }
    }

    return EShaderType::eShaderUnknown;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

CShader::CShader()
    : m_shaderID(0)
    , m_type(eShaderUnknown)
    , m_data(nullptr)
    , m_name("")
    , m_compileStatus(false)
{
}

CShader::~CShader()
{
    CShader::clear();
}

u32 CShader::getShaderID() const
{
    return m_shaderID;
}

CShader::EShaderType CShader::getShaderType() const
{
    return m_type;
}

bool CShader::getCompileStatus() const
{
    return m_compileStatus;
}

const std::string CShader::getName() const
{
    return m_name;
}

void CShader::setName(const std::string& name)
{
    m_name = name;
}

void CShader::clear()
{
    if (m_data != nullptr)
    {
        free(m_data);
        m_data = nullptr;
    }
}

bool CShader::parse(const tinyxml2::XMLElement* root)
{
    CShader::clear();

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
        LOG_INFO("CRenderPass: Create shader [%s] from data", shaderName.c_str());
        const std::string shaderType = root->Attribute("type");
        m_type = CShader::getShaderTypeByName(shaderType);
    }

    if (!root->Attribute("file"))
    {
        const std::string shaderBody = root->GetText();
        if (shaderBody.empty())
        {
            LOG_ERROR("CShader: Empty shader body");
            return false;
        }

        c8* data = (c8*)malloc(shaderBody.size() + 1);
        memcpy(data, shaderBody.data(), shaderBody.size());
        data[shaderBody.size()] = '\0';
        m_data = reinterpret_cast<void*>(data);
    }
    else
    {
        const std::string shaderPath = root->Attribute("file");
        LOG_INFO("CRenderPass: Create shader from file: %s", shaderPath.c_str());

        const ShaderSourceDataPtr& source = CShaderManager::getInstance()->load(shaderPath);

        c8* data = (c8*)malloc(source->getBody().size() + 1);
        memcpy(data, source->getBody().data(), source->getBody().size());
        data[source->getBody().size()] = '\0';
        m_data = reinterpret_cast<void*>(data);
        if (!m_data)
        {
            LOG_ERROR("CShader: Error load shader %s", shaderPath.c_str());
            return false;
        }
    }

    return true;
}
