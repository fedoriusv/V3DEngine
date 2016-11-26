#include "RenderPass.h"

#include "Engine.h"
#include "utils/Logger.h"
#include "scene/TargetManager.h"
#include "scene/ShaderManager.h"
#include "GeometryTarget.h"
#include "renderer/ShaderAttribute.h"
#include "renderer/ShaderSampler.h"

#include "tinyxml2.h"

namespace v3d
{
namespace renderer
{

using namespace scene;
using namespace resources;

RenderPass::RenderPass()
    : m_renderState(nullptr)
    , m_lods(nullptr)
    , m_program(new ShaderProgram())

    , m_enable(true)
    , m_name("")
{
    RenderPass::init();
}

RenderPass::RenderPass(const RenderPass& pass)
    : m_renderState(nullptr)
    , m_lods(nullptr)
    , m_program(nullptr)

    , m_enable(pass.m_enable)
    , m_name(pass.m_name)
{
    RenderPass::init();

    m_targetList = pass.m_targetList;

    m_lods->operator=(*pass.m_lods);
    m_renderState->operator=(*pass.m_renderState);

    m_program = pass.m_program->clone();
    ASSERT(m_program, "RenderPass: Copy program is failed");
    if (m_program)
    {
       /* m_program->addShaderData(m_defaultShaderData);
        m_program->addShaderData(m_userShaderData);

        if (!m_program->create())
        {
            ASSERT(false, "RenderPass::clone fail");
        }*/
    }
}

RenderPass& RenderPass::operator=(const RenderPass& pass)
{
    if (&pass == this)
    {
        return *this;
    }

    m_targetList = pass.m_targetList;

    m_lods->operator=(*pass.m_lods);
    m_renderState->operator=(*pass.m_renderState);

    m_program = pass.m_program->clone();
    ASSERT(m_program, "Copy program is failed");
    if (m_program)
    {
       /* m_program->addShaderData(m_defaultShaderData);
        m_program->addShaderData(m_userShaderData);

        if (!m_program->create())
        {
            ASSERT(false, "CShaderProgramGL::clone fail");
        }*/
    }

    m_enable = pass.m_enable;
    m_name = pass.m_name;

    return *this;
}

RenderPass::~RenderPass()
{
    m_targetList.clear();
}

const ShaderProgramPtr RenderPass::getShaderProgram() const
{
    return m_program;
}

void RenderPass::setShaderProgram(const ShaderProgramPtr& program)
{
    m_program = program;
}

const RenderStatePtr RenderPass::getRenderState() const
{
    return m_renderState;
}

void RenderPass::setRenderState(const RenderStatePtr& state)
{
    m_renderState = state;
}

bool RenderPass::parse(const tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("RenderPass: Cannot read render pass element");
        return false;
    }

    const std::string passName = root->Attribute("name");
    if (passName.empty())
    {
        LOG_ERROR("RenderPass: Cannot read render pass name");
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

    //fragData
    const tinyxml2::XMLElement* fragdataElement = root->FirstChildElement("fragdata");
    if (fragdataElement)
    {
        if (!parseFragdata(fragdataElement))
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

bool RenderPass::parseUniforms(const tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("RenderPass: Not exist xml uniforms element");
        return false;
    }

    const tinyxml2::XMLElement* varElement = root->FirstChildElement("var");
    while (varElement)
    {
        ShaderUniform* uniform = new ShaderUniform();

        if (!uniform->parse(varElement))
        {
            delete uniform;
            uniform = nullptr;

            LOG_ERROR("RenderPass: Cannot parse uniform in pass '%s'", m_name.c_str());
            varElement = varElement->NextSiblingElement("var");
            continue;
        }
        m_program->addUniform(uniform);

        varElement = varElement->NextSiblingElement("var");
    }

    return true;
}

bool RenderPass::parseAttributes(const tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("RenderPass: Not exist xml attributes element");
        return false;
    }

    const tinyxml2::XMLElement* varElement = root->FirstChildElement("var");
    while (varElement)
    {
        ShaderAttribute* attribute = new ShaderAttribute();
        if (!attribute->parse(varElement))
        {
            delete attribute;
            attribute = nullptr;

            LOG_ERROR("RenderPass: Parse error attribute element");
            varElement = varElement->NextSiblingElement("var");
            continue;
        }

        /*bool isDefault = (attribute->getData() != ShaderAttribute::eAttribUser);
        if (isDefault)
        {
            m_defaultShaderData->addAttribute(attribute);
        }
        else
        {
            m_userShaderData->addAttribute(attribute);
        }*/

        varElement = varElement->NextSiblingElement("var");
    }

    return true;
}

bool RenderPass::parseFragdata(const tinyxml2::XMLElement* root)
{
    if (!root)
    {
        return true;
    }

    const tinyxml2::XMLElement* varElement = root->FirstChildElement("var");
    while (varElement)
    {
        ShaderAttribute* fragData = new ShaderAttribute();
        if (!fragData->parse(varElement))
        {
            delete fragData;
            fragData = nullptr;

            LOG_ERROR("RenderPass: Parse error fragData element");
            varElement = varElement->NextSiblingElement("var");
            continue;
        }

        /*bool isDefault = (fragData->getData() != ShaderAttribute::eAttribUser);
        if (isDefault)
        {
            m_defaultShaderData->addFragData(fragData);
        }
        else
        {
            m_userShaderData->addFragData(fragData);
        }*/

        varElement = varElement->NextSiblingElement("var");
    }

    return true;
}

bool RenderPass::parseSamplers(const tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("RenderPass: Not exist xml samplers element");
        return false;
    }

    const tinyxml2::XMLElement* varElement = root->FirstChildElement("var");
    while (varElement)
    {
        CShaderSampler* sampler = new CShaderSampler();
        if (!sampler->parse(varElement))
        {
            delete sampler;
            sampler = nullptr;

            LOG_ERROR("RenderPass: Parse error samplers element");
            varElement = varElement->NextSiblingElement("var");
            continue;
        }

        /*bool isDefault = sampler->getType() != CShaderSampler::ESamplerType::eUserSampler;
        if (isDefault)
        {
            m_defaultShaderData->addSampler(sampler);
        }
        else
        {
            m_userShaderData->addSampler(sampler);
        }*/

        varElement = varElement->NextSiblingElement("var");
    }

    return true;
}

bool RenderPass::parseShaders(const tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("RenderPass: Not exist xml shader element");
        return false;
    }

    ShaderDefinesList definesList;
    const tinyxml2::XMLElement*  definesElement = root->FirstChildElement("defines");
    if (definesElement)
    {
        const tinyxml2::XMLElement*  defineElement = definesElement->FirstChildElement("var");
        while (defineElement)
        {
            if (defineElement->Attribute("name"))
            {
                std::string name = defineElement->Attribute("name");
                std::string value = (defineElement->Attribute("val")) ? defineElement->Attribute("val") : "";

                definesList.insert(std::map<std::string, std::string>::value_type(name, value));
            }

            defineElement = defineElement->NextSiblingElement("var");
        }

        if (!definesList.empty())
        {
            m_program->setMacroDefinitions(definesList);
        }
    }

    const tinyxml2::XMLElement* shaderElement = root->FirstChildElement("var");
    while (shaderElement)
    {
        /*ShaderResource shaderData;
        if (!IShader::parse(shaderElement, shaderData))
        {
            LOG_ERROR("RenderPass: Shader parse error");
            ASSERT(false, "Shader parse error");

            shaderElement = shaderElement->NextSiblingElement("var");
            continue;

        if (!definesList.empty())
        {
            shaderData.setDefines(definesList);
        }*/

        /*ShaderWPtr shader = ShaderManager::getInstance()->get(shaderData.getHash());
        if (shader.expired())
        {
            ShaderPtr newShader = ENGINE_RENDERER->makeSharedShader();
            newShader->setFlag(IShader::eLoaded);

            if (!newShader->create(std::move(shaderData)))
            {
                LOG_ERROR("RenderPass: Error create shader");
                shaderElement = shaderElement->NextSiblingElement("var");
                continue;
            }

            ShaderManager::getInstance()->add(newShader);
            shader = newShader;
        }

        m_program->attachShader(shader);*/

        shaderElement = shaderElement->NextSiblingElement("var");
    }

    /*m_program->addShaderData(m_defaultShaderData);
    m_program->addShaderData(m_userShaderData);*/

   /* if (!m_program->create())
    {
        LOG_ERROR("RenderPass: Error Create Shader Program");
        return false;
    }*/

    return true;
}

void RenderPass::init()
{
    m_lods = std::make_shared<CRenderLOD>();
    //m_renderState = ENGINE_RENDERER->makeSharedRenderState();
}

bool RenderPass::parseRenderTarget(const tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("RenderPass: Not exist xml element");
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
            LOG_ERROR("RenderPass: Render Target have not name");
            varElement = varElement->NextSiblingElement("var");
            continue;
        }

        if (isDefault)
        {
            LOG_INFO("CRenderTarget: Set default setting for render target");
            RenderPass::addTarget(ENGINE_RENDERER->getDefaultRenderTarget());
        }
        else
        {
            const TargetPtr target = CTargetManager::getInstance()->get(name);
            if (!target)
            {
                LOG_WARNING("CRenderTarget: Render target not found with name '%s', skip", name.c_str());
                varElement = varElement->NextSiblingElement("var");

                continue;
            }
            else
            {
                LOG_INFO("CRenderTarget: Set target with name '%s'", name.c_str());
                RenderPass::addTarget(target);

                if (target->getTagetType() == ITarget::ETagetType::eGeometryTarget)
                {
                    const GeometryTargetPtr& geomtarget = std::static_pointer_cast<CGeometryTarget>(target);

                    if (geomtarget->getBufferStrings().empty())
                    {
                        LOG_WARNING("CRenderTarget: Varyings list is empty in target '%s'", name.c_str());
                    }
                    else
                    {
                        //m_program->addVaryingsAttibutes(geomtarget->getBufferStrings());
                    }
                }
            }
        }

        varElement = varElement->NextSiblingElement("var");
    }

    if (RenderPass::getTargetCount() == 0)
    {
        LOG_WARNING("CRenderTarget: Current pass hasn't no one of Target");
    }

    return true;
}

bool RenderPass::parseRenderState(const tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("RenderPass: Not exist xml renderstate element");
        return false;
    }

   /* if (!m_renderState->parse(root))
    {
        LOG_ERROR("RenderPass: Renderstate parse error");
        return false;
    }*/

    return true;
}

bool RenderPass::parseRenderLOD(const tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("RenderPass: Not exist xml lod element");
        return false;
    }

    f32 geometryLod = root->FloatAttribute("geometry");
    if (geometryLod > 0)
    {
        m_lods->setGeometryDistance(geometryLod);
    }

    return true;
}

void RenderPass::bind(u32 target)
{
    m_renderState->bind();

    /*if (!m_enable || !m_program->isEnable())
    {
        m_program->unbind();
        return;
    }*/

   /* if (!m_program->isFlagPresent(ShaderProgram::eLinked))
    {
        if (!m_program->create())
        {
            LOG_ERROR("RenderPass: Error Create Shader Program");
            return;
        }
    }

    m_program->bind();*/

    /*const UniformList& list = m_userShaderData->m_uniformList;
    for (UniformList::const_iterator uniform = list.begin(); uniform != list.end(); ++uniform)
    {
        ShaderUniform::EUniformData data = uniform->second->getData();
        if (data != ShaderUniform::eUserUniform || uniform->second->getID() < 0)
        {
            continue;
        }
        m_program->applyUniform(uniform->second);
    }*/

    //Replaced to render lists
    /*ASSERT(m_targetList.size() > target, "Invalid target index");
    m_targetList[target]->bind();*/
}

void RenderPass::unbind(u32 target)
{

    //m_program->unbind();

    //Replaced to render lists
    /*ASSERT(m_targetList.size() > target, "Invalid target index");
    m_targetList[target]->unbind();*/

}

RenderPassPtr RenderPass::clone() const
{
    RenderPassPtr pass = std::make_shared<RenderPass>(*this);
    return pass;
}

const std::string RenderPass::attachIndexToUniform(const std::string& name, s32 idx)
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

const RenderLODPtr RenderPass::getRenderLOD() const
{
    return m_lods;
}

void RenderPass::setRenderLOD(const RenderLODPtr& lod)
{
    m_lods = lod;
}

const TargetPtr RenderPass::getTarget(u32 index) const
{
    ASSERT(m_targetList.size() > index, "Invalid Render Target index");
    if (m_targetList.size() <= index)
    {
        return nullptr;
    }

    if (m_targetList[index].expired())
    {
        return nullptr;
    }

    return m_targetList[index].lock();
}

const TargetPtr RenderPass::getTarget(const std::string& target) const
{
    TargetList::const_iterator iter = std::find_if(m_targetList.begin(), m_targetList.end(), [&target](const TargetWPtr& item) -> bool
    {
        if (!item.expired() && item.lock()->getName() == target)
        {
            return true;
        }

        return false;
    });

    if (iter == m_targetList.end())
    {
        return nullptr;
    }

    if ((*iter).expired())
    {
        return nullptr;
    }

    return (*iter).lock();
}

bool RenderPass::addTarget(const TargetPtr& target)
{
    if (target)
    {
        TargetList::const_iterator iter = std::find_if(m_targetList.cbegin(), m_targetList.cend(), [&target](const TargetWPtr& item) -> bool
        {
            return (!item.expired() && item.lock() == target);
        });

        if (iter != m_targetList.cend())
        {
            LOG_WARNING("RenderPass: Added target alredy exist '%s'", (*iter).lock()->getName().c_str());
            return false;
        }

        m_targetList.push_back(target);

        return true;
    }

    return false;
}

bool RenderPass::removeTarget(const TargetPtr& target)
{
    if (target)
    {
        TargetList::const_iterator iter = std::remove_if(m_targetList.begin(), m_targetList.end(), [&target](const TargetWPtr& item) -> bool
        {
            return (!item.expired() && item.lock() == target);
        });

        if (iter == m_targetList.end())
        {
            LOG_WARNING("RenderPass: Romoved target not found '%s'", target->getName().c_str());
            return false;
        }
        m_targetList.erase(iter, m_targetList.end());

        return true;
    }

    return false;
}

bool RenderPass::addTarget(const std::string& target)
{
    if (!target.empty())
    {
        if (RenderPass::getTarget(target))
        {
            LOG_WARNING("RenderPass: Added target alredy exist '%s'", target.c_str());
            return false;
        }

        const TargetPtr newTarget = CTargetManager::getInstance()->get(target);
        if (!newTarget)
        {
            LOG_WARNING("RenderPass: Can't find target '%s' in manager", target.c_str());
            return false;
        }

        m_targetList.push_back(newTarget);

        return true;
    }

    return false;
}

bool RenderPass::removeTarget(const std::string& target)
{
    if (!target.empty())
    {
        TargetList::const_iterator iter = std::remove_if(m_targetList.begin(), m_targetList.end(), [&target](const TargetWPtr& item) -> bool
        {
            return (!item.expired() && item.lock()->getName() == target);
        });

        if (iter == m_targetList.end())
        {
            LOG_WARNING("RenderPass: Romoved target not found '%s'", target.c_str());
            return false;
        }
        m_targetList.erase(iter, m_targetList.end());

        return true;
    }

    return false;
}

u32 RenderPass::getTargetCount() const
{
    return (u32)m_targetList.size();
}

} //namespace renderer
} //namespace v3d
