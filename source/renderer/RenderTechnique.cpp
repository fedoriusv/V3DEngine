#include "RenderTechnique.h"
#include "scene/TargetManager.h"
#include "scene/TextureManager.h"
#include "scene/RenderTechniqueManager.h"
#include "utils/Logger.h"
#include "Engine.h"

#include "tinyxml2.h"

namespace v3d
{
namespace renderer
{

using namespace scene;

static const std::string k_diff = "_copy";

CRenderTechnique::CRenderTechnique()
    : m_name("")
    , m_currentPass(0U)
{
}

CRenderTechnique::CRenderTechnique(const CRenderTechnique& technique)
    : m_name(technique.m_name + k_diff)
    , m_currentPass(technique.m_currentPass)
{
    for (RenderPassList::const_iterator iter = technique.m_renderPassList.cbegin(); iter < technique.m_renderPassList.cend(); ++iter)
    {
        m_renderPassList.push_back((*iter)->clone());
    }
}

CRenderTechnique& CRenderTechnique::operator=(const CRenderTechnique& technique)
{
    if (&technique == this)
    {
        return *this;
    }

    IResource::operator=(technique);

    for (RenderPassList::const_iterator iter = technique.m_renderPassList.cbegin(); iter < technique.m_renderPassList.cend(); ++iter)
    {
        m_renderPassList.push_back((*iter)->clone());
    }
    m_currentPass = technique.m_currentPass;

    bool finding = true;
    u32 index = 0;
    std::string generatedName = technique.m_name + k_diff;
    while (finding)
    {
        if (!CRenderTechniqueManager::getInstance()->get(generatedName + std::to_string(index)))
        {
            finding = false;
        }

        ++index;
    }

    m_name = generatedName + std::to_string(index);

    return *this;
}

CRenderTechnique::~CRenderTechnique()
{
    m_renderPassList.clear();
}

const std::string& CRenderTechnique::getName() const
{
    return m_name;
}

const RenderPassPtr CRenderTechnique::getRenderPass(u32 id) const
{
    ASSERT(id <= m_renderPassList.size(), "RenderPass id error");

    return m_renderPassList[id];
}

RenderPassPtr CRenderTechnique::getRenderPass(u32 id)
{
    ASSERT(id <= m_renderPassList.size(), "RenderPass id error");

    return m_renderPassList[id];
}

u32 CRenderTechnique::getRenderPassCount() const
{
    return (u32)m_renderPassList.size();
}

void CRenderTechnique::addRenderPass(const RenderPassPtr pass)
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

u32 CRenderTechnique::getCurrentPass() const
{
    return m_currentPass;
}

void CRenderTechnique::setCurrentPass(u32 pass)
{
    m_currentPass = pass;
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
        //Targets
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
            if (CTargetManager::getInstance()->get(targetName))
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
            CTargetManager::getInstance()->add(target);

            targetElement = targetElement->NextSiblingElement("target");
        }

        //Transformfeedbacks
        tinyxml2::XMLElement* transformfeedbackElement = rendertargetsElement->FirstChildElement("transformfeedback");
        while (transformfeedbackElement)
        {
            if (!transformfeedbackElement->Attribute("name"))
            {
                LOG_ERROR("CRenderTechnique: Transformfeedback target have't name");
                transformfeedbackElement = transformfeedbackElement->NextSiblingElement("transformfeedback");
                continue;
            }
            std::string targetName = transformfeedbackElement->Attribute("name");
            if (CTargetManager::getInstance()->get(targetName))
            {
                transformfeedbackElement = transformfeedbackElement->NextSiblingElement("transformfeedback");
                continue;
            }

            GeometryTargetPtr target = RENDERER->makeSharedGeometryTarget();
            if (!target->parse(transformfeedbackElement))
            {
                LOG_ERROR("CRenderTechnique: Transformfeedback target parse error");
                transformfeedbackElement = transformfeedbackElement->NextSiblingElement("target");
                continue;
            }

            if (!target->create())
            {
                LOG_ERROR("CRenderPass: Can't create render transformfeedback target");
                transformfeedbackElement = transformfeedbackElement->NextSiblingElement("target");
                continue;
            }
            CTargetManager::getInstance()->add(target);

            transformfeedbackElement = transformfeedbackElement->NextSiblingElement("target");
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
    IResource::setStream(stream);
}

bool CRenderTechnique::load()
{
    const stream::IStreamPtr& stream = IResource::getStream();
    if (!stream)
    {
        LOG_ERROR("CRenderTechnique: Empty Stream with name [%s] form RenderTechique", IResource::getResourseName().c_str());
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
        LOG_ERROR("CRenderTechnique: Stream parse error name [%s]", IResource::getResourseName().c_str());
        ASSERT(false, "Stream parse error");
        return false;
    }

    tinyxml2::XMLElement* rootElement = doc.FirstChildElement("technique");
    if (!rootElement)
    {
        LOG_ERROR("CRenderTechnique: Can't find technique in Stream name [%s]", IResource::getResourseName().c_str());
        ASSERT(false, "Can't find technique in Stream");
        return false;
    }

    return CRenderTechnique::parse(rootElement);

}

CRenderTechnique* CRenderTechnique::clone() const
{
    CRenderTechnique* technique = new CRenderTechnique();
    technique->operator=(*this);

    CRenderTechniqueManager::getInstance()->add(technique);

    return technique;
}

} //namespace renderer
} //namespace v3d
