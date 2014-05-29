#include "RenderTechnique.h"

#include "utils/Logger.h"

using namespace v3d;
using namespace v3d::renderer;

CRenderTechique::CRenderTechique()
{
}

CRenderTechique::~CRenderTechique()
{
    m_renderPassList.clear();
}

RenderPassPtr CRenderTechique::getRenderPass(u32 id) const
{
    ASSERT(id <= m_renderPassList.size() || "RenderPass id error");

    return m_renderPassList[id];
}

u32 CRenderTechique::getRenderPassCount() const
{
    return m_renderPassList.size();
}

void CRenderTechique::addRenderPass(const RenderPassPtr& pass)
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
        RenderPassPtr pass = std::make_shared<CRenderPass>();
        if (pass->parse(passElement))
        {
            LOG_ERROR("Error parse. Pass section");
            return false;
        }
        CRenderTechique::addRenderPass(pass);

        passElement = passElement->NextSiblingElement("pass");
    }

    return true;

}

void CRenderTechique::init(stream::IStream* stream)
{
    CResource::setStream(stream);
}

bool CRenderTechique::load()
{
    stream::IStream* stream = CResource::getStream();
    if (!stream)
    {
        LOG_ERROR("Empty Stream with name [%s] form RenderTechique", CResource::getResourseName());
        return false;
    }

    m_renderPassList.clear();

    tinyxml2::XMLDocument doc;
    
    stream->seekBeg(0);
    s32 size = stream->size();
    std::string data;
    stream->read(data);

    tinyxml2::XMLError success = doc.Parse(data.c_str());
    if (!success)
    {
        LOG_ERROR("Error Parse Stream name [%s] form RenderTechique", CResource::getResourseName());
        return false;
    }

    tinyxml2::XMLElement* rootElement = doc.FirstChildElement("technique");
    if (!rootElement)
    {
        LOG_ERROR("Can not find technique in Stream name [%s] form RenderTechique", CResource::getResourseName());
        return false;
    }

    return CRenderTechique::parse(rootElement);

}

void CRenderTechique::refresh()
{
}