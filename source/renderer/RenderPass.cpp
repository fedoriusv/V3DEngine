#include "RenderPass.h"

#include "Engine.h"
#include "utils/Logger.h"
#include "renderer/GL/ShaderProgramGL.h"

using namespace v3d;
using namespace v3d::renderer;

CRenderPass::CRenderPass()
    : m_program(nullptr)
    , m_shaderData(nullptr)
    , m_renderState(nullptr)
{
    CRenderPass::init();
}

CRenderPass::~CRenderPass()
{
}

ShaderProgramPtr CRenderPass::getShaderProgram() const
{
    return m_program;
}

void CRenderPass::setShaderProgram(const ShaderProgramPtr& program)
{
    m_program = program;
}

ShaderDataPtr CRenderPass::getShaderData() const
{
    return m_shaderData;
}

void CRenderPass::setShaderData(const ShaderDataPtr& data)
{
    m_shaderData = data;
}

RenderStatePtr CRenderPass::getRenderState() const
{
    return m_renderState;
}

void CRenderPass::setRenderState(const RenderStatePtr& state)
{
    m_renderState = state;
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

    tinyxml2::XMLElement* varElement = root->FirstChildElement("var");
    while (varElement)
    {
        const std::string varName = varElement->Attribute("name");
        if (varName.empty())
        {
            LOG_ERROR("Cannot find uniform name from pass '%s'", m_name.c_str());

            varElement = varElement->NextSiblingElement("var");
            continue;
        }

        const std::string varType = varElement->Attribute("type");
        if (varType.empty())
        {
            LOG_ERROR("Cannot find uniform type from pass '%s' in '%s'", m_name.c_str(), varName.c_str());

            varElement = varElement->NextSiblingElement("var");
            continue;
        }

        const std::string varVal = varElement->Attribute("val");
        if (varVal.empty())
        {
            LOG_ERROR("Cannot find uniform val from pass '%s' in '%s'", m_name.c_str(), varName.c_str());

            varElement = varElement->NextSiblingElement("var");
            continue;
        }

        EDefaultUniformData uniformName = CShaderUniform::getShaderUniformValueByName(varVal);
        EShaderDataType  uniformType = (uniformName == EDefaultUniformData::eUserUniform)
            ? EShaderDataType::eDataNone : CShaderData::getShaderDataTypeByName(varType);

        m_shaderData->addDefaultUniform(varName, uniformType, uniformName);

        varElement = varElement->NextSiblingElement("var");
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

    tinyxml2::XMLElement* varElement = root->FirstChildElement("var");
    while (varElement)
    {
        const std::string varName = varElement->Attribute("name");
        if (varName.empty())
        {
            LOG_ERROR("Cannot find uniform name from pass '%s'", m_name.c_str());

            varElement = varElement->NextSiblingElement("var");
            continue;
        }

        const std::string varVal = varElement->Attribute("val");
        if (varVal.empty())
        {
            LOG_ERROR("Cannot find uniform val from pass '%s' in '%s'", m_name.c_str(), varName.c_str());

            varElement = varElement->NextSiblingElement("var");
            continue;
        }

        EShaderAttribute attribureName = CShaderAttribute::getShaderAttributeTypeByName(varVal);
        m_shaderData->addAttribute(varName, attribureName);

        varElement = varElement->NextSiblingElement("var");
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

    tinyxml2::XMLElement* varElement = root->FirstChildElement("var");
    while (varElement)
    {
        const std::string varName = varElement->Attribute("name");
        if (varName.empty())
        {
            LOG_ERROR("Cannot find sampler name from pass '%s'", m_name.c_str());

            varElement = varElement->NextSiblingElement("var");
            continue;
        }

        m_shaderData->addSampler(varName);

        varElement = varElement->NextSiblingElement("var");
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
        ShaderPtr shader = v3d::CEngine::getInstance()->getRenderer()->makeSharedShader();
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
            type = CShader::getShaderTypeByName(shaderType);
        }

        const std::string shaderPath = shaderElement->Attribute("path");
        if (!shaderPath.empty())
        {
            LOG_INFO("Info parse. Create vshader from file: %s", shaderPath.c_str());
            if (!shader->load(shaderPath, type))
            {
                LOG_ERROR("Error Load Shader %s", shaderPath.c_str());
            }
        }
        else
        {
            const std::string shaderBody = shaderElement->GetText();
            if (shaderBody.empty())
            {
                LOG_WARRNING("Warrning parse. empty vshader body");
            }

            LOG_INFO("Info parse. Create vshader from data");
            if (!shader->create(shaderBody, type))
            {
                LOG_ERROR("Error Load Shader body");
            }
        }

        m_program->addShader(shader);

        shaderElement = shaderElement->NextSiblingElement("shader");
    }

    m_program->create();

    return true;
}

void CRenderPass::init()
{
    m_shaderData = std::make_shared<CShaderData>();
    m_program = v3d::CEngine::getInstance()->getRenderer()->makeSharedProgram(m_shaderData);
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