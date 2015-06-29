#include "RenderPass.h"

#include "Engine.h"
#include "utils/Logger.h"
#include "scene/RenderTargetManager.h"

#include "tinyxml2.h"

using namespace v3d;
using namespace renderer;
using namespace scene;

CRenderPass::CRenderPass()
    : m_program(nullptr)
    , m_userShaderData(nullptr)
    , m_defaultShaderData(nullptr)
    , m_renderState(nullptr)
    , m_lods(nullptr)
    , m_enable(true)
    , m_name("")
{
    CRenderPass::init();
}

CRenderPass::~CRenderPass()
{
    m_renderTargets.clear();
}

const ShaderProgramPtr& CRenderPass::getShaderProgram() const
{
    return m_program;
}

void CRenderPass::setShaderProgram(const ShaderProgramPtr& program)
{
    m_program = program;
}

const ShaderDataPtr& CRenderPass::getUserShaderData() const
{
    return m_userShaderData;
}

void CRenderPass::setUserShaderData(const ShaderDataPtr& data)
{
    m_userShaderData = data;
}

const ShaderDataPtr& CRenderPass::getDefaultShaderData() const
{
    return m_defaultShaderData;
}

void CRenderPass::setDefaultShaderData(const ShaderDataPtr& data)
{
    m_defaultShaderData = data;
}

const RenderStatePtr& CRenderPass::getRenderState() const
{
    return m_renderState;
}

void CRenderPass::setRenderState(const RenderStatePtr& state)
{
    m_renderState = state;
}

bool CRenderPass::parse(const tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("CRenderPass: Cannot read render pass element");
        return false;
    }

    const std::string passName = root->Attribute("name");
    if (passName.empty())
    {
        LOG_ERROR("CRenderPass: Cannot read render pass name");
        return false;
    }

    m_name = passName;

    //uniforms
    const tinyxml2::XMLElement* uniformsElement = root->FirstChildElement("uniforms");
    if (uniformsElement)
    {
        if (!parseUniforms(uniformsElement))
        {
            return false;
        }
    }

    //attributes
    const tinyxml2::XMLElement* attributesElement = root->FirstChildElement("attributes");
    if (attributesElement)
    {
        if (!parseAttributes(attributesElement))
        {
            return false;
        }
    }

    //samplers
    const tinyxml2::XMLElement* samplersElement = root->FirstChildElement("samplers");
    if (samplersElement)
    {
        if (!parseSamplers(samplersElement))
        {
            return false;
        }
    }

    //shaders
    const tinyxml2::XMLElement*  shadersElement = root->FirstChildElement("shaders");
    if (shadersElement)
    {
        if (!parseShaders(shadersElement))
        {
            return false;
        }
    }

    //rendertarget
    const tinyxml2::XMLElement*  rendertargetElement = root->FirstChildElement("rendertarget");
    if (rendertargetElement)
    {
        if (!parseRenderTarget(rendertargetElement))
        {
            return false;
        }
    }

    //renderstate
    const tinyxml2::XMLElement*  renderstateElement = root->FirstChildElement("renderstate");
    if (renderstateElement)
    {
        if (!parseRenderState(renderstateElement))
        {
            return false;
        }
    }

    //lod
    const tinyxml2::XMLElement*  renderlodElement = root->FirstChildElement("lod");
    if (renderlodElement)
    {
        if (!parseRenderLOD(renderlodElement))
        {
            return false;
        }
    }

    return true;
}

bool CRenderPass::parseUniforms(const tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("CRenderPass: Not exist xml uniforms element");
        return false;
    }

    const tinyxml2::XMLElement* varElement = root->FirstChildElement("var");
    while (varElement)
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        if (!uniform->parse(varElement))
        {
            LOG_ERROR("CRenderPass: Cannot parse uniform in pass '%s'", m_name.c_str());
            varElement = varElement->NextSiblingElement("var");
            continue;
        }

        bool isDefault = (uniform->getData() != CShaderUniform::eUserUniform);
        if (isDefault)
        {
            m_defaultShaderData->addUniform(uniform);
        }
        else
        {
            m_userShaderData->addUniform(uniform);
        }

        varElement = varElement->NextSiblingElement("var");
    }

    return true;
}

bool CRenderPass::parseAttributes(const tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("CRenderPass: Not exist xml attributes element");
        return false;
    }

    const tinyxml2::XMLElement* varElement = root->FirstChildElement("var");
    while (varElement)
    {
        AttributePtr attribute = std::make_shared<CShaderAttribute>();
        if (!attribute->parse(varElement))
        {
            LOG_ERROR("CRenderPass: Parse error attribute element");
            varElement = varElement->NextSiblingElement("var");
            continue;
        }

        m_defaultShaderData->addAttribute(attribute);

        varElement = varElement->NextSiblingElement("var");
    }

    return true;
}

bool CRenderPass::parseSamplers(const tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("CRenderPass: Not exist xml samplers element");
        return false;
    }

    const tinyxml2::XMLElement* varElement = root->FirstChildElement("var");
    while (varElement)
    {
        SamplerPtr sampler = std::make_shared<CShaderSampler>();
        if (!sampler->parse(varElement))
        {
            LOG_ERROR("CRenderPass: Parse error samplers element");
            varElement = varElement->NextSiblingElement("var");
            continue;
        }

        bool isDefault = sampler->getType() != CShaderSampler::eUserSampler;
        if (isDefault)
        {
            m_defaultShaderData->addSampler(sampler);
        }
        else
        {
            m_userShaderData->addSampler(sampler);
        }

        varElement = varElement->NextSiblingElement("var");
    }

    return true;
}

bool CRenderPass::parseShaders(const tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("CRenderPass: Not exist xml shader element");
        return false;
    }

    const tinyxml2::XMLElement*  shaderElement = root->FirstChildElement("var");
    while (shaderElement)
    {
        ShaderPtr shader = RENDERER->makeSharedShader();
        if (!shader->parse(shaderElement))
        {
            LOG_ERROR("CRenderPass: Shader parse error");
			ASSERT("Shader parse error" && false);

            shaderElement = shaderElement->NextSiblingElement("var");
            continue;
        }

        if (!shader->create())
        {
            LOG_ERROR("CRenderPass: Error create shader");
            shaderElement = shaderElement->NextSiblingElement("var");
            continue;
        }
        m_program->addShader(shader);

        shaderElement = shaderElement->NextSiblingElement("var");
    }

    m_program->addShaderData(m_defaultShaderData);
    m_program->addShaderData(m_userShaderData);

    if (!m_program->create())
    {
        LOG_ERROR("CRenderPass: Error Create Shader Program");
        return false;
    }

    return true;
}

void CRenderPass::init()
{
    m_userShaderData = std::make_shared<CShaderData>();
    m_defaultShaderData = std::make_shared<CShaderData>();
    m_lods = std::make_shared<CRenderLOD>();
    m_program = RENDERER->makeSharedProgram();
    m_renderState = RENDERER->makeSharedRenderState();
}

bool CRenderPass::parseRenderTarget(const tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("CRenderPass: Not exist xml element");
        return false;
    }

    const tinyxml2::XMLElement* varElement = root->FirstChildElement("var");
    while (varElement)
    {
        bool isDefault = false;
        std::string name;
        if (varElement->Attribute("val"))
        {
            name = varElement->Attribute("val");
            if (name == "default")
            {
                isDefault = true;
            }
        }
        else
        {
            LOG_ERROR("CRenderPass: Render Target have not name");
            varElement = varElement->NextSiblingElement("var");
            continue;
        }

        if (isDefault)
        {
            LOG_INFO("CRenderTarget: Set default setting for render target");
            CRenderPass::addRenderTarget(RENDERER->getDefaultRenderTarget());
        }
        else
        {
            const RenderTargetPtr target = CRenderTargetManager::getInstance()->get(name);
            if (!target)
            {
                LOG_WARNING("CRenderTarget: Render target not found with name '%s', set default", name.c_str());
                CRenderPass::addRenderTarget(RENDERER->getDefaultRenderTarget());
            }
            else
            {
                LOG_INFO("CRenderTarget: Set target with name '%s'", name.c_str());
                CRenderPass::addRenderTarget(target);
            }
        }

        varElement = varElement->NextSiblingElement("var");
    }

    if (CRenderPass::getRenderTargetCount() == 0)
    {
        LOG_WARNING("CRenderTarget: Render target not found set default");
        CRenderPass::addRenderTarget(RENDERER->getDefaultRenderTarget());
    }

    return true;
}

bool CRenderPass::parseRenderState(const tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("CRenderPass: Not exist xml renderstate element");
        return false;
    }

    if (!m_renderState->parse(root))
    {
        LOG_ERROR("CRenderPass: Renderstate parse error");
        return false;
    }

    return true;
}

bool CRenderPass::parseRenderLOD(const tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("CRenderPass: Not exist xml lod element");
        return false;
    }

    f32 geometryLod = root->FloatAttribute("geometry");
    if (geometryLod > 0)
    {
        m_lods->setGeometryDistance(geometryLod);
    }

    return true;
}


void CRenderPass::bind(u32 target)
{
    ASSERT(m_renderTargets.size() > target && "Invalid target index");
    m_renderTargets[target]->bind();

    m_renderState->bind();

    if (!m_enable || !m_program->isEnable())
    {
        m_program->unbind();
        return;
    }

    m_program->bind();

    const UniformList& list = m_userShaderData->m_uniformList;
    for (UniformList::const_iterator uniform = list.begin(); uniform != list.end(); ++uniform)
    {
        CShaderUniform::EUniformData data = uniform->second->getData();
        if (data != CShaderUniform::eUserUniform)
        {
            continue;
        }

        m_program->setUniform(uniform->second);
    }
}

void CRenderPass::unbind(u32 target)
{
    //m_program->unbind();

    ASSERT(m_renderTargets.size() > target && "Invalid target index");
    m_renderTargets[target]->unbind();
}

const std::string CRenderPass::attachIndexToUniform(const std::string& name, s32 idx)
{
    size_t pos = name.find_first_of(".");
    if (pos == std::string::npos)
    {
        std::string idxName(name + "[" + std::to_string(idx) + "]");

        return idxName;
    }

    std::string idxName(name.substr(0, pos) + "[" + std::to_string(idx) + "]" + name.substr(pos, name.size()));

    return idxName;
}

const RenderLODPtr& CRenderPass::getRenderLOD() const
{
    return m_lods;
}

void CRenderPass::setRenderLOD(const RenderLODPtr& lod)
{
    m_lods = lod;
}

const RenderTargetPtr& CRenderPass::getRenderTarget(u32 index) const
{
    ASSERT(m_renderTargets.size() > index && "Invalid Render Target index");
    return m_renderTargets[index];
}

void CRenderPass::addRenderTarget(const RenderTargetPtr& target)
{
    if (target)
    {
        RenderTargetList ::const_iterator iter = std::find(m_renderTargets.begin(), m_renderTargets.end(), target);
        if (iter != m_renderTargets.end())
        {
            LOG_WARNING("CRenderPass: Added target alredy exist '%s'", (*iter)->getName().c_str());
            return;
        }

        m_renderTargets.push_back(target);
    }
}

void CRenderPass::setRenderTarget(u32 index, const RenderTargetPtr& target)
{
    ASSERT(m_renderTargets.size() > index && "Invalid Render Target index");
    m_renderTargets[index] = target;
}

u32 CRenderPass::getRenderTargetCount() const
{
    return (u32)m_renderTargets.size();
}