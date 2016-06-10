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
{
}

IShader::~IShader()
{
    if (!IShader::isFlagPresent(IShader::eDeleted))
    {
        ASSERT(false, "Shader incorrect deleted");
    }
}

const std::string& IShader::getName() const
{
    return m_data.getName();
}

ShaderPtr IShader::clone() const
{
    ShaderPtr shader = RENDERER->makeSharedShader();

    CShaderSource& data = shader->m_data;
    data.setHeader(m_data.getHeader());
    data.setBody(m_data.getBody());
    data.setType(IShader::getType());
    data.setName(IShader::getName());

    shader->setFlag(IShader::eLoaded);

    return shader;
}

void IShader::setName(const std::string& name)
{
    m_data.setName(name);
}

void IShader::setType(EShaderType type)
{
    m_data.setType(type);
}

void IShader::setFlag(EShaderFlags flag)
{
    m_flags = flag;
}

void IShader::addFlag(EShaderFlags flag)
{
    m_flags |= flag;
}

const CShaderSource& IShader::getShaderSource() const
{
    return m_data;
}

bool IShader::parse(const tinyxml2::XMLElement* root, CShaderSource& data)
{
    if (!root)
    {
        LOG_ERROR("IShader: Not exist xml shader element");
        return false;
    }

    if (!root->Attribute("name"))
    {
        LOG_ERROR("IShader: Empty shader name");
        return false;
    }
    const std::string shaderName(root->Attribute("name"));


    if (!root->Attribute("type"))
    {
        LOG_ERROR("IShader: Empty shader type");
        return false;
    }
    const std::string shaderType(root->Attribute("type"));

    if (!root->Attribute("file"))
    {
        LOG_INFO("IShader: Create shader [%s] from data", shaderName.c_str());
        std::string shaderBody(root->GetText());
        if (shaderBody.empty())
        {
            LOG_ERROR("IShader: Empty shader body");
            return false;
        }

        data.setBody(shaderBody);
        data.setType(CShaderSource::getShaderTypeByName(shaderType));
        data.setName(shaderName);

        return true;
    }
    else
    {
        const std::string shaderPath = root->Attribute("file");
        LOG_INFO("IShader: Create shader from file: %s", shaderPath.c_str());

        const CShaderSourceData* source = CShaderManager::getInstance()->load(shaderPath);
        if (!source)
        {
            LOG_ERROR("IShader: Error load shader %s", shaderPath.c_str());
            return false;
        }

        std::string shaderBody(source->getBody());
        if (shaderBody.empty())
        {
            LOG_ERROR("IShader: Error load shader %s", shaderPath.c_str());
            return false;
        }

        data.setBody(shaderBody);
        data.setType(CShaderSource::getShaderTypeByName(shaderType));
        data.setName(shaderName);

        return true;
    }

    return false;
}

EShaderType IShader::getType() const
{
    return m_data.getType();
}

u16 IShader::getFlags() const
{
    return m_flags;
}

bool IShader::isFlagPresent(EShaderFlags flag)
{
    return (m_flags & flag) != 0;
}

bool IShader::create(EShaderType type, const std::string& body, const ShaderDefinesList& defines)
{
     std::string tempHeader = CShaderSource::buildHeader(defines, body);
    if (m_data.getType() == type && m_data.getHeader() == tempHeader && m_data.getBody() == body)
    {
        LOG_WARNING("CShaderGL::create: Shader already created");
        return false;
    }

    if (m_data.getType() != type && IShader::isFlagPresent(IShader::eCreated))
    {
        destroy();
        IShader::setType(type);

        IShader::setFlag(IShader::eLoaded);
    }

    if (m_data.getHeader() != tempHeader || m_data.getBody() != body)
    {
        m_flags &= ~IShader::eCompiled;
    }

    CShaderSource shaderData;

    shaderData.setHeader(tempHeader);
    shaderData.setBody(body);
    shaderData.setType(type);
    shaderData.setName("");

    return create(std::move(shaderData));
}

} //namespace renderer
} //namespace v3d
