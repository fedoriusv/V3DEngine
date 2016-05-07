#include "Shader.h"
#include "utils/Logger.h"
#include "scene/ShaderManager.h"
#include "resources/ShaderSouceData.h"
#include "Engine.h"

#include "tinyxml2.h"

namespace v3d
{
namespace renderer
{

using namespace stream;
using namespace scene;
using namespace resources;

IShader::IShader()
    : m_flags(IShader::eInvalid)
    , m_data(new CShaderSource())
{
}

IShader::~IShader()
{
    if (m_data)
    {
        delete m_data;
        m_data = nullptr;
    }

    if (!IShader::isFlagPresent(IShader::eDeleted))
    {
        ASSERT(false, "Shader incorrect deleted");
    }
}

const std::string& IShader::getName() const
{
    ASSERT(m_data, "Shader data is nullptr");
    return m_data->getName();
}

ShaderPtr IShader::clone()
{
    ShaderPtr shader = RENDERER->makeSharedShader();

    CShaderSource* data = shader->m_data;
    ASSERT(data, "Shader data is nullptr");

    ASSERT(m_data, "Shader data is nullptr");
    data->setBody(m_data->getBody());
    data->setType(IShader::getType());
    data->setName(IShader::getName());

    shader->setFlag(IShader::eLoaded);

    return shader;
}

void IShader::setName(const std::string& name)
{
    ASSERT(m_data, "Shader data is nullptr");
    m_data->setName(name);
}

void IShader::setType(EShaderType type)
{
    ASSERT(m_data, "Shader data is nullptr");
    m_data->setType(type);
}

void IShader::setFlag(EShaderFlags flag)
{
    m_flags = flag;
}

void IShader::addFlag(EShaderFlags flag)
{
    m_flags |= flag;
}

const CShaderSource* IShader::getShaderSource() const
{
    ASSERT(m_data, "Shader data is nullptr");
    return m_data;
}

CShaderSource* IShader::parse(const tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("IShader: Not exist xml shader element");
        return nullptr;
    }

    if (!root->Attribute("name"))
    {
        LOG_ERROR("IShader: Empty shader name");
        return nullptr;
    }
    const std::string shaderName(root->Attribute("name"));


    if (!root->Attribute("type"))
    {
        LOG_ERROR("IShader: Empty shader type");
        return nullptr;
    }
    const std::string shaderType(root->Attribute("type"));

    if (!root->Attribute("file"))
    {
        LOG_INFO("IShader: Create shader [%s] from data", shaderName.c_str());
        std::string shaderBody(root->GetText());
        if (shaderBody.empty())
        {
            LOG_ERROR("IShader: Empty shader body");
            return nullptr;
        }

        CShaderSource* shaderData = new CShaderSource();

        shaderData->setBody(shaderBody);
        shaderData->setType(CShaderSource::getShaderTypeByName(shaderType));
        shaderData->setName(shaderName);

        return shaderData;
    }
    else
    {
        const std::string shaderPath = root->Attribute("file");
        LOG_INFO("IShader: Create shader from file: %s", shaderPath.c_str());

        const CShaderSourceData* source = CShaderManager::getInstance()->load(shaderPath);
        if (!source)
        {
            LOG_ERROR("IShader: Error load shader %s", shaderPath.c_str());
            return nullptr;
        }

        std::string shaderBody(source->getBody());
        if (shaderBody.empty())
        {
            LOG_ERROR("IShader: Error load shader %s", shaderPath.c_str());
            return nullptr;
        }

        CShaderSource* shaderData = new CShaderSource();

        shaderData->setBody(shaderBody);
        shaderData->setType(CShaderSource::getShaderTypeByName(shaderType));
        shaderData->setName(shaderName);

        return shaderData;
    }

    return nullptr;
}

EShaderType IShader::getType() const
{
    ASSERT(m_data, "Shader data is nullptr");
    return m_data->getType();
}

u16 IShader::getFlags() const
{
    return m_flags;
}

bool IShader::isFlagPresent(EShaderFlags flag)
{
    return m_flags & flag;
}

bool IShader::create(EShaderType type, const std::string& body, const ShaderDefinesList& defines)
{
     std::string tempHeader = CShaderSource::buildHeader(defines, body);
    if (m_data->getType() == type && m_data->getHeader() == tempHeader && m_data->getBody() == body)
    {
        LOG_WARNING("CShaderGL::create: Shader already created");
        return false;
    }

    if (m_data->getType() != type && IShader::isFlagPresent(IShader::eCreated))
    {
        destroy();
        IShader::setType(type);

        IShader::setFlag(IShader::eLoaded);
    }

    if (m_data->getHeader() != tempHeader || m_data->getBody() != body)
    {
        m_flags &= ~IShader::eCompiled;
    }

    CShaderSource* shaderData = new CShaderSource();

    shaderData->setHeader(tempHeader);
    shaderData->setBody(body);
    shaderData->setType(type);
    shaderData->setName("");

    return create(shaderData);
}

} //namespace renderer
} //namespace v3d
