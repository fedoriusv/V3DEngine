#include "RenderTechnique.h"

#include "utils/Logger.h"

using namespace f3d;
using namespace f3d::renderer;

CRenderTechique::CRenderTechique()
{
}

CRenderTechique::~CRenderTechique()
{
    m_renderPassList.clear();
}

CRenderPassPtr CRenderTechique::getRenderPass(u32 id) const
{
    ASSERT(id <= m_renderPassList.size() || "RenderPass id error");

    return m_renderPassList[id];
}

u32 CRenderTechique::getRenderPassCount() const
{
    return m_renderPassList.size();
}

void CRenderTechique::addRenderPass(const CRenderPassPtr& pass)
{
    if (pass)
    {
        m_renderPassList.push_back(pass);
    }
}

bool CRenderTechique::parse(tinyxml2::XMLElement* root)
{
    const int techniqueVersion = root->IntAttribute("version");
    if (techniqueVersion != SHADER_PARSER )
    {
        LOG_ERROR("Error parse. Need version %d", SHADER_PARSER);
        return false;
    }

    const std::string techniqueName = root->Attribute("name");
    if (techniqueName.empty())
    {
        LOG_ERROR("Error parse. Have no technique name");
        return false;
    }

    m_name = techniqueName;


    tinyxml2::XMLElement* passElement = root->FirstChildElement("pass");
    if (!passElement)
    {
        LOG_ERROR("Error parse. Have no pass section");
        return false;
    }

    while (passElement)
    {
        CRenderPassPtr pass = std::make_shared<CRenderPass>(CRenderPass());
        if (pass->parse(passElement))
        {
            LOG_ERROR("Error parse. Pass section");
            return false;
        }
        m_renderPassList.push_back(pass);

        passElement = passElement->NextSiblingElement("pass");
    }

    return true;

}