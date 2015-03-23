#include "ShaderSampler.h"
#include "scene/RenderTargetManager.h"
#include "scene/TextureManager.h"
#include "utils/Logger.h"

#include "tinyxml2.h"

using namespace v3d;
using namespace renderer;
using namespace scene;

CShaderSampler::CShaderSampler()
    : m_attribute("")
    , m_type(eUserSampler)
    , m_id(-1)
{
}

CShaderSampler::~CShaderSampler()
{
}

CShaderSampler& CShaderSampler::operator=(const CShaderSampler& other)
{
    if (this == &other)
    {
        return *this;
    }

    m_attribute = other.m_attribute;
    m_type = other.m_type;
    m_target = other.m_target;
    m_texture = other.m_texture;

    return *this;
}

void CShaderSampler::setAttribute(const std::string& attribute)
{
    m_attribute = attribute;
}

const std::string& CShaderSampler::getAttribute() const
{
    return m_attribute;
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
    CShaderSampler::setAttribute(varName);

    if (root->Attribute("val"))
    {
        const std::string varVal = root->Attribute("val");
        const RenderTargetPtr target = CRenderTargetManager::getInstance()->get(varVal);
        if (target)
        {
            m_type = eRenderTargetSampler;
            m_target = target;
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

        const renderer::TexturePtr texture = CTextureManager::getInstance()->get(varVal);
        if (texture)
        {
            m_type = eTextureSampler;
            m_texture = texture;
            return true;
        }

        LOG_ERROR("CRenderPass: Texture with val '%s' not found", varVal.c_str());
        m_type = eUserSampler;
        return true;
    }

    m_type = eUserSampler;
    return true;
}

const TextureWPtr& CShaderSampler::getTexture() const
{
    return m_texture;
}

const RenderTargetWPtr& CShaderSampler::getTarget() const
{
    return m_target;
}