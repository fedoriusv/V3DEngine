#include "RenderTechnique.h"
#include "scene/RenderTargetManager.h"
#include "scene/TextureManager.h"
#include "utils/Logger.h"
#include "Engine.h"

#include "tinyxml2.h"

using namespace v3d;
using namespace renderer;
using namespace scene;

CRenderTechnique::CRenderTechnique()
    : m_name("")
{
}

CRenderTechnique::~CRenderTechnique()
{
    m_renderPassList.clear();
}

const std::string& CRenderTechnique::getName() const
{
    return m_name;
}

const RenderPassPtr& CRenderTechnique::getRenderPass(u32 id) const
{
    ASSERT(id <= m_renderPassList.size() || "RenderPass id error");

    return m_renderPassList[id];
}

u32 CRenderTechnique::getRenderPassCount() const
{
    return (u32)m_renderPassList.size();
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
        LOG_ERROR("CRenderTechnique: Need version %d", SHADER_PARSER);
        return false;
    }

    const std::string techniqueName = root->Attribute("name");
    if (techniqueName.empty())
    {
        LOG_ERROR("CRenderTechnique: Technique have't name");
        return false;
    }
    m_name = techniqueName;

    LOG_INFO("CRenderTechnique: Parse render technique [%s]", m_name.c_str());

    //Parse RenderTargets
    tinyxml2::XMLElement* rendertargetsElement = root->FirstChildElement("rendertargets");
    if (rendertargetsElement)
    {
        tinyxml2::XMLElement* targetElement = rendertargetsElement->FirstChildElement("target");
        while (targetElement)
        {
            if (!targetElement->Attribute("name"))
            {
                LOG_ERROR("CRenderTechnique: Rendertarget have't name");
                targetElement = targetElement->NextSiblingElement("target");
                continue;
            }
            std::string targetName = targetElement->Attribute("name");
            if (CRenderTargetManager::getInstance()->get(targetName))
            {
                targetElement = targetElement->NextSiblingElement("target");
                continue;
            }

            RenderTargetPtr target = RENDERER->makeSharedRenderTarget();
            if (!target->parse(targetElement))
            {
                LOG_ERROR("CRenderTechnique: Rendertarget parse error");
                targetElement = targetElement->NextSiblingElement("target");
                continue;
            }

            if (!target->create())
            {
                LOG_ERROR("CRenderPass: Can't create render target");
                targetElement = targetElement->NextSiblingElement("target");
                continue;
            }
            CRenderTargetManager::getInstance()->add(target);

            targetElement = targetElement->NextSiblingElement("pass");
        }
    }

    //Parse Textures
    tinyxml2::XMLElement* texturesElement = root->FirstChildElement("textures");
    if (texturesElement)
    {
        tinyxml2::XMLElement* textureElement = texturesElement->FirstChildElement("texture");
        while (textureElement)
        {
            if (!textureElement->Attribute("name"))
            {
                LOG_ERROR("CRenderTechnique: Texture have't name");
                textureElement = textureElement->NextSiblingElement("texture");
                continue;
            }

            if (!textureElement->Attribute("file"))
            {
                LOG_ERROR("CRenderTechnique: Texture have't path to file");
                textureElement = textureElement->NextSiblingElement("texture");
                continue;
            }
            
            std::string nameStr = textureElement->Attribute("name");
            std::string fileStr = textureElement->Attribute("file");
            const TexturePtr texture = CTextureManager::getInstance()->load(fileStr, nameStr);
            if (!texture)
            {
                LOG_WARNING("CRenderTechnique: File [%s] not found or not support format", fileStr.c_str());
            }

            textureElement = textureElement->NextSiblingElement("texture");
        }
    }

    //Parse Passes
    tinyxml2::XMLElement* passElement = root->FirstChildElement("pass");
    if (!passElement)
    {
        LOG_ERROR("CRenderTechnique: Pass section have't exist");
        return false;
    }

    while (passElement)
    {
        RenderPassPtr pass = std::make_shared<CRenderPass>();
        if (!pass->parse(passElement))
        {
            LOG_ERROR("CRenderTechnique: Error parse pass section");
            return false;
        }
        CRenderTechnique::addRenderPass(pass);

        passElement = passElement->NextSiblingElement("pass");
    }

    return true;

}

void CRenderTechnique::init(const stream::IStreamPtr& stream)
{
    CResource::setStream(stream);
}

bool CRenderTechnique::load()
{
    const stream::IStreamPtr& stream = CResource::getStream();
    if (!stream)
    {
        LOG_ERROR("CRenderTechnique: Empty Stream with name [%s] form RenderTechique", CResource::getResourseName().c_str());
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
        LOG_ERROR("CRenderTechnique: Error Parse Stream name [%s]", CResource::getResourseName().c_str());
        return false;
    }

    tinyxml2::XMLElement* rootElement = doc.FirstChildElement("technique");
    if (!rootElement)
    {
        LOG_ERROR("CRenderTechnique: Can't find technique in Stream name [%s]", CResource::getResourseName().c_str());
        return false;
    }

    return CRenderTechnique::parse(rootElement);

}

void CRenderTechnique::refresh()
{
    //TODO: refresh
}
