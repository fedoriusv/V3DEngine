#include "RenderPass.h"
#include "Fedoria3D.h"
#include "renderer/GL/ShaderProgramGL.h"
#include "renderer/GL/ShaderGL.h"

using namespace f3d;
using namespace f3d::renderer;

CRenderPass::CRenderPass()
    : m_program(nullptr)
{
    CRenderPass::init();
}

CRenderPass::~CRenderPass()
{
}

bool CRenderPass::parse(tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("Error parse.  Cannot read render pass element");
        return false;
    }

    const std::string passName = root->Attribute("name");
    if (passName.empty())
    {
        LOG_ERROR("Error parse. Cannot read render pass name");
        return false;
    }

    m_name = passName;

    //uniforms
    tinyxml2::XMLElement*  uniformsElement = root->FirstChildElement("uniforms");
    if (uniformsElement)
    {
        if (!parseUniforms(uniformsElement))
        {
            return false;
        }
    }

    //attributes
    tinyxml2::XMLElement*  attributesElement = root->FirstChildElement("attributes");
    if (attributesElement)
    {
        if (!parseAttributes(attributesElement))
        {
            return false;
        }
    }

    //samplers
    tinyxml2::XMLElement*  samplersElement = root->FirstChildElement("samplers");
    if (samplersElement)
    {
        if (!parseSamplers(samplersElement))
        {
            return false;
        }
    }

    //shaders
    tinyxml2::XMLElement*  shadersElement = root->FirstChildElement("shaders");
    if (shadersElement)
    {
        if (!parseShaders(shadersElement))
        {
            return false;
        }
    }

    //rendertarget
    tinyxml2::XMLElement*  rendertargetElement = root->FirstChildElement("rendertarget");
    if (rendertargetElement)
    {
        if (!parseRenderTarget(rendertargetElement))
        {
            return false;
        }
    }

    //renderstate
    tinyxml2::XMLElement*  renderstateElement = root->FirstChildElement("renderstate");
    if (renderstateElement)
    {
        if (!parseRenderState(renderstateElement))
        {
            return false;
        }
    }

    return true;
}

bool CRenderPass::parseUniforms(tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("Error parse. Not exist xml uniforms element");
        return false;
    }

    tinyxml2::XMLElement* varElement = root;
    while (varElement)
    {
        const std::string varName = varElement->Attribute("name");
        if (varName.empty())
        {
            LOG_ERROR("Cannot find uniform name from pass '%s'", m_name);
            return false;
        }

        const std::string varType = varElement->Attribute("type");
        if (varType.empty())
        {
            LOG_ERROR("Cannot find uniform type from pass '%s' in '%s'", m_name, varName);
            return false;
        }

        const std::string varVal = varElement->Attribute("val");
        if (varVal.empty())
        {
            LOG_ERROR("Cannot find uniform val from pass '%s' in '%s'", m_name, varName);
            return false;
        }

        EDefaultUniformData uniformName = getShaderUniformTypeByName(varName);
        EShaderDataType  uniformType = (uniformName == EDefaultUniformData::eUserUniform)
            ? EShaderDataType::eUniformNone : getShaderDataTypeByName(varType);

        m_shaderData->addDefaultUniform(varName, uniformType, uniformName);

        varElement = varElement->NextSiblingElement("uniforms");
    }

    return true;
}

bool CRenderPass::parseAttributes(tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("Error parse. Not exist xml attributes element");
        return false;
    }

    tinyxml2::XMLElement* varElement = root;
    while (varElement)
    {
        const std::string varName = varElement->Attribute("name");
        if (varName.empty())
        {
            LOG_ERROR("Cannot find uniform name from pass '%s'", m_name);
            return false;
        }

        const std::string varVal = varElement->Attribute("val");
        if (varVal.empty())
        {
            LOG_ERROR("Cannot find uniform val from pass '%s' in '%s'", m_name, varName);
            return false;
        }

        EShaderAttribute attribureName = getShaderAttributeTypeByName(varName);
        m_shaderData->addAttribute(varName, attribureName);

        varElement = varElement->NextSiblingElement("uniforms");
    }

    return true;
}

bool CRenderPass::parseSamplers(tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("Error parse. Not exist xml samplers element");
        return false;
    }

    tinyxml2::XMLElement* varElement = root;
    while (varElement)
    {
        const std::string varName = varElement->Attribute("name");
        if (varName.empty())
        {
            LOG_ERROR("Cannot find sampler name from pass '%s'", m_name);
            return false;
        }

        m_shaderData->addSampler(varName);

        varElement = varElement->NextSiblingElement("uniforms");
    }

    return true;
}

bool CRenderPass::parseShaders(tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("Error parse. Not exist xml shader element");
        return false;
    }

    //vshader
    tinyxml2::XMLElement*  shaderElement = root;
    while (shaderElement)
    {
        ShaderPtr shader = CRenderPass::makeSharedShader();
        if (!shader)
        {
            LOG_ERROR("Error parse. Could not create shader");
            return false;
        }

        const std::string shaderName = shaderElement->Attribute("name");
        if (!shaderName.empty())
        {
            shader->setName(shaderName);
        }
        else
        {
            LOG_WARRNING("Warrning parse. empty vshader name");
        }

        EShaderType type = EShaderType::eTypeVertex;
        const std::string shaderType = shaderElement->Attribute("type");
        if (shaderType.empty())
        {
            type = EShaderType::eTypeVertex;
            LOG_WARRNING("Warrning parse. Shader have not type. Set Vertex type");
        }
        else
        {
            type = getShaderTypeByName(shaderType);
        }

        const std::string shaderPath = shaderElement->Attribute("path");
        if (!shaderPath.empty())
        {
            LOG_INFO("Info parse. Create vshader from file: %s", shaderPath);
            shader->load(shaderPath, type);
        }
        else
        {
            const std::string shaderBody = shaderElement->GetText();
            if (shaderBody.empty())
            {
                LOG_WARRNING("Warrning parse. empty vshader body");
            }

            LOG_INFO("Info parse. Create vshader from data");
            shader->create(shaderBody, type);
        }

        m_program->addShader(shader);

        shaderElement = shaderElement->NextSiblingElement("shader");
    }

    m_program->create();

    return true;
}

void CRenderPass::init()
{
    platform::EDriverType type = f3d::CEngine::getInstance()->getPlatform()->getDriverType();

    switch (type)
    {
        case platform::EDriverType::eDriverOpenGL:
        {
            m_program = std::make_shared<CShaderProgramGL>();
        }
            break;

        case platform::EDriverType::eDriverDirect3D:
        {
            //m_geometry = std::make_shared<CGeometry>(CGeometryGL());
        }
            break;

        default:
        {
            LOG_ERROR("Can`t Choose driver type");
        }
    }

    m_shaderData = std::make_shared<CShaderData>();
}

ShaderPtr CRenderPass::makeSharedShader() const
{
    platform::EDriverType type = f3d::CEngine::getInstance()->getPlatform()->getDriverType();

    switch (type)
    {
        case platform::EDriverType::eDriverOpenGL:
        {
           return std::make_shared<CShaderGL>();
        }

        case platform::EDriverType::eDriverDirect3D:
        {
           //return std::make_shared<CShaderD3D>();
        }

        default:
        {
           LOG_ERROR("Can`t Choose driver type");
           return nullptr;
        }
    }

    return nullptr;
}

bool CRenderPass::parseRenderTarget(tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("Error parse. Not exist xml element");
        return false;
    }
}

bool CRenderPass::parseRenderState(tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("Error parse. Not exist xml element");
        return false;
    }
}