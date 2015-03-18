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

    const std::string varName = root->Attribute("name");
    if (varName.empty())
    {
        LOG_ERROR("CRenderPass: Cannot find sampler name");
        return false;
    }
    CShaderSampler::setAttribute(varName);

    if (root->Attribute("val"))
    {
        const std::string varVal = root->Attribute("val");
        const RenderTargetPtr target = CRenderTargetManager::getInstance()->get(varVal);
        if (target)
        {
            m_type = eRenderTargetSampler;
            m_target = target;
            return true;
        }

        const renderer::TexturePtr texture = CTextureManager::getInstance()->get(varVal);
        if (texture)
        {
            m_type = eTextureSampler;
            m_texture = texture;
            return true;
        }

        m_type = eUserSampler;
        return true;
    }

    return true;
}