#include "ShaderSampler.h"
#include "scene/TargetManager.h"
#include "scene/TextureManager.h"
#include "utils/Logger.h"

#include "tinyxml2.h"

namespace v3d
{
namespace renderer
{

using namespace scene;

CShaderSampler::CShaderSampler()
    : m_type(ESamplerType::eUserSampler)
    , m_name("")

    //, m_target()
    , m_texture(nullptr)

    , m_id(-1)
{
}

CShaderSampler::CShaderSampler(const CShaderSampler& sampler)
    : m_type(ESamplerType::eUserSampler)
    , m_name("")

    //, m_target()
    , m_texture(nullptr)

    , m_id(-1)
{
    m_name = sampler.m_name;
    m_type = sampler.m_type;

    m_target = sampler.m_target;
    m_texture = sampler.m_texture;
}

CShaderSampler& CShaderSampler::operator=(const CShaderSampler& sampler)
{
    if (this == &sampler)
    {
        return *this;
    }

    m_name = sampler.m_name;
    m_type = sampler.m_type;

    m_target = sampler.m_target;
    m_texture = sampler.m_texture;

    m_id = -1;

    return *this;
}

CShaderSampler::~CShaderSampler()
{
}

void CShaderSampler::setName(const std::string& name)
{
    m_name = name;
}

const std::string& CShaderSampler::getName() const
{
    return m_name;
}

CShaderSampler::ESamplerType CShaderSampler::getType() const
{
    return m_type;
}

void CShaderSampler::setID(s32 id)
{
    m_id = id;
}

s32 CShaderSampler::getID() const
{
    return m_id;
}

bool CShaderSampler::parse(const tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("CShaderSampler: Empty xml samplers element");
        return false;
    }

    if (!root->Attribute("name"))
    {
        LOG_ERROR("CRenderPass: Cannot find sampler name");
        return false;
    }
    const std::string varName = root->Attribute("name");
    CShaderSampler::setName(varName);

    if (root->Attribute("val"))
    {
        const std::string varVal = root->Attribute("val");
        const TargetPtr target = CTargetManager::getInstance()->get(varVal);
        if (target)
        {
            if (target->getTagetType() != ITarget::ETagetType::eRenderTarget)
            {
                LOG_ERROR("CRenderPass: Target not supported");
                return false;
            }
            const RenderTargetPtr& rendertarget = std::static_pointer_cast<CRenderTarget>(target);

            m_type = ESamplerType::eRenderTargetSampler;
            m_target = rendertarget;
            if (!root->Attribute("attachment") || std::string(root->Attribute("attachment")).empty())
            {
                m_texture = m_target.lock()->getColorTexture(0);
                return true;

            }

            std::string attachVal = root->Attribute("attachment");
            if (attachVal[0] == 'c' && attachVal.size() > 1)
            {
                std::string indexStr = attachVal.substr(1);
                s32 index = std::stoi(indexStr);
                if (index >= 0)
                {
                    m_texture = m_target.lock()->getColorTexture(index);
                    return true;
                }

                LOG_ERROR("CRenderPass: Color Texture %d not found", index);
                return false;
            }
            else if (attachVal[0] == 'd')
            {
                m_texture = m_target.lock()->getDepthTexture();
                return true;
            }
            else if (attachVal[0] == 's')
            {
                m_texture = m_target.lock()->getStencilTexture();
                return true;
            }
            else
            {
                LOG_ERROR("CRenderPass: Unknown key '%c'", attachVal[0]);
                return false;
            }

            return true;
        }

        const TexturePtr texture = CTextureManager::getInstance()->get(varVal);
        if (texture)
        {
            m_type = ESamplerType::eTextureSampler;
            m_texture = texture;
            return true;
        }

        LOG_ERROR("CRenderPass: Texture with val '%s' not found", varVal.c_str());
        m_type = ESamplerType::eUserSampler;
        return true;
    }

    m_type = ESamplerType::eUserSampler;
    return true;
}

const TexturePtr CShaderSampler::getTexture() const
{
    return m_texture;
}

TexturePtr CShaderSampler::getTexture()
{
    return utils::const_pointer_cast<CTexture>(m_texture);
}

const RenderTargetWPtr CShaderSampler::getTarget() const
{
    return m_target;
}

RenderTargetWPtr CShaderSampler::getTarget()
{
    return m_target;
}

} //namespace renderer
} //namespace v3d
