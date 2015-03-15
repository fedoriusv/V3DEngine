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
    , m_type(eInvalidSampler)
{
}

CShaderSampler::~CShaderSampler()
{
}

void CShaderSampler::setAttribute(const std::string& attribute)
{
    m_attribute = attribute;
}

const std::string& CShaderSampler::getAttribute() const
{
    return m_attribute;
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
    m_attribute = varName;

    const std::string varVal = root->Attribute("val");
    if (!varVal.empty())
    {
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

        LOG_WARNING("CShaderSampler: Target with name '%s' have't exist ", varVal.c_str());
        return false;
    }

    return false;
}