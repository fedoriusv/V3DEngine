#include "RenderTechnique.h"

#include "utils/Logger.h"

using namespace v3d;
using namespace v3d::renderer;

CRenderTechnique::CRenderTechnique()
{
}

CRenderTechnique::~CRenderTechnique()
{
    m_renderPassList.clear();
}

const RenderPassPtr& CRenderTechnique::getRenderPass(u32 id) const
{
    ASSERT(id <= m_renderPassList.size() || "RenderPass id error");

    return m_renderPassList[id];
}

u32 CRenderTechnique::getRenderPassCount() const
{
    return m_renderPassList.size();
}

void CRenderTechnique::addRenderPass(const RenderPassPtr& pass)
{
    if (pass)
    {
       m_renderPassList.push_back(pass);
    }
}

const RenderPassList& CRenderTechnique::getRenderPassList() const
{
    return m_renderPassList;
}

bool CRenderTechnique::parse(tinyxml2::XMLElement* root)
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

    LOG_INFO("Info parse. Parse render technique [%s]", m_name.c_str());

    tinyxml2::XMLElement* passElement = root->FirstChildElement("pass");
    if (!passElement)
    {
        LOG_ERROR("Error parse. Have no pass section");
        return false;
    }

    while (passElement)
    {
        RenderPassPtr pass = std::make_shared<CRenderPass>();
        if (!pass->parse(passElement))
        {
            LOG_ERROR("Error parse. Pass section");
            return false;
        }
        CRenderTechnique::addRenderPass(pass);

        passElement = passElement->NextSiblingElement("pass");
    }

    return true;

}

void CRenderTechnique::init(stream::IStream* stream)
{
    CResource::setStream(stream);
}

bool CRenderTechnique::load()
{
    stream::IStream* stream = CResource::getStream();
    if (!stream)
    {
        LOG_ERROR("Empty Stream with name [%s] form RenderTechique", CResource::getResourseName().c_str());
        return false;
    }

    m_renderPassList.clear();

    tinyxml2::XMLDocument doc;
    std::string data;

    u32 size = stream->size();
    if (size > 0)
    {
        stream->seekBeg(0);
        stream->read(data);
    }
    stream->close();

    tinyxml2::XMLError success = doc.Parse(data.c_str());
    if (success)
    {
        LOG_ERROR("Error Parse Stream name [%s] form RenderTechique", CResource::getResourseName().c_str());
        return false;
    }

    tinyxml2::XMLElement* rootElement = doc.FirstChildElement("technique");
    if (!rootElement)
    {
        LOG_ERROR("Can not find technique in Stream name [%s] form RenderTechique", CResource::getResourseName().c_str());
        return false;
    }

    return CRenderTechnique::parse(rootElement);

}

void CRenderTechnique::refresh()
{
    //TODO: refresh
}