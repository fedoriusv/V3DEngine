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

RenderTechnique::RenderTechnique()
    : m_name("")
    , m_currentPass(0U)
{
}

RenderTechnique::RenderTechnique(const RenderTechnique& technique)
    : m_name(technique.m_name + k_diff)
    , m_currentPass(technique.m_currentPass)
{
    for (RenderPassList::const_iterator iter = technique.m_renderPassList.cbegin(); iter < technique.m_renderPassList.cend(); ++iter)
    {
        m_renderPassList.push_back((*iter)->clone());
    }
}

RenderTechnique& RenderTechnique::operator=(const RenderTechnique& technique)
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
        if (!RenderTechniqueManager::getInstance()->get(generatedName + std::to_string(index)))
        {
            finding = false;
        }

        ++index;
    }

    m_name = generatedName + std::to_string(index);

    return *this;
}

RenderTechnique::~RenderTechnique()
{
    m_renderPassList.clear();
}

const std::string& RenderTechnique::getName() const
{
    return m_name;
}

const RenderPassPtr RenderTechnique::getRenderPass(u32 id) const
{
    ASSERT(id <= m_renderPassList.size(), "RenderPass id error");

    return m_renderPassList[id];
}

RenderPassPtr RenderTechnique::getRenderPass(u32 id)
{
    ASSERT(id <= m_renderPassList.size(), "RenderPass id error");

    return m_renderPassList[id];
}

u32 RenderTechnique::getRenderPassCount() const
{
    return (u32)m_renderPassList.size();
}

void RenderTechnique::addRenderPass(const RenderPassPtr pass)
{
    if (pass)
    {
       m_renderPassList.push_back(pass);
    }
}

const RenderPassList& RenderTechnique::getRenderPassList() const
{
    return m_renderPassList;
}

u32 RenderTechnique::getCurrentPass() const
{
    return m_currentPass;
}

void RenderTechnique::setCurrentPass(u32 pass)
{
    m_currentPass = pass;
}

bool RenderTechnique::parse(tinyxml2::XMLElement* root)
{
    const int techniqueVersion = root->IntAttribute("version");
    if (techniqueVersion != SHADER_PARSER )
    {
        LOG_ERROR("RenderTechnique: Need version %d", SHADER_PARSER);
        return false;
    }

    const std::string techniqueName = root->Attribute("name");
    if (techniqueName.empty())
    {
        LOG_ERROR("RenderTechnique: Technique have't name");
        return false;
    }
    m_name = techniqueName;

    LOG_INFO("RenderTechnique: Parse render technique [%s]", m_name.c_str());

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
                LOG_ERROR("RenderTechnique: Rendertarget have't name");
                targetElement = targetElement->NextSiblingElement("target");
                continue;
            }
            std::string targetName = targetElement->Attribute("name");
            if (CTargetManager::getInstance()->get(targetName))
            {
                targetElement = targetElement->NextSiblingElement("target");
                continue;
            }

            RenderTargetPtr target = ENGINE_RENDERER->makeSharedRenderTarget();
            if (!target->parse(targetElement))
            {
                LOG_ERROR("RenderTechnique: Rendertarget parse error");
                targetElement = targetElement->NextSiblingElement("target");
                continue;
            }

            if (!target->create())
            {
                LOG_ERROR("RenderPass: Can't create render target");
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
                LOG_ERROR("RenderTechnique: Transformfeedback target have't name");
                transformfeedbackElement = transformfeedbackElement->NextSiblingElement("transformfeedback");
                continue;
            }
            std::string targetName = transformfeedbackElement->Attribute("name");
            if (CTargetManager::getInstance()->get(targetName))
            {
                transformfeedbackElement = transformfeedbackElement->NextSiblingElement("transformfeedback");
                continue;
            }

            GeometryTargetPtr target = ENGINE_RENDERER->makeSharedGeometryTarget();
            if (!target->parse(transformfeedbackElement))
            {
                LOG_ERROR("RenderTechnique: Transformfeedback target parse error");
                transformfeedbackElement = transformfeedbackElement->NextSiblingElement("target");
                continue;
            }

            if (!target->create())
            {
                LOG_ERROR("RenderPass: Can't create render transformfeedback target");
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
                LOG_ERROR("RenderTechnique: Texture have't name");
                textureElement = textureElement->NextSiblingElement("texture");
                continue;
            }

            if (!textureElement->Attribute("file"))
            {
                LOG_ERROR("RenderTechnique: Texture have't path to file");
                textureElement = textureElement->NextSiblingElement("texture");
                continue;
            }
            
            std::string nameStr = textureElement->Attribute("name");
            std::string fileStr = textureElement->Attribute("file");
            const TexturePtr texture = TextureManager::getInstance()->load(fileStr, nameStr);
            if (!texture)
            {
                LOG_WARNING("RenderTechnique: File [%s] not found or not support format", fileStr.c_str());
            }

            textureElement = textureElement->NextSiblingElement("texture");
        }
    }

    //Parse Passes
    tinyxml2::XMLElement* passElement = root->FirstChildElement("pass");
    if (!passElement)
    {
        LOG_ERROR("RenderTechnique: Pass section hasn't exist");
        return false;
    }

    while (passElement)
    {
        RenderPassPtr pass = std::make_shared<RenderPass>();
        if (!pass->parse(passElement))
        {
            LOG_ERROR("RenderTechnique: Error parse pass section");
            return false;
        }
        RenderTechnique::addRenderPass(pass);

        passElement = passElement->NextSiblingElement("pass");
    }

    return true;

}

void RenderTechnique::init(const stream::IStreamPtr stream)
{
    IResource::setStream(stream);
}

bool RenderTechnique::load()
{
    const stream::IStreamPtr& stream = IResource::getStream();
    if (!stream)
    {
        LOG_ERROR("RenderTechnique: Empty Stream with name [%s] form RenderTechique", IResource::getResourseName().c_str());
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
        LOG_ERROR("RenderTechnique: Stream parse error name [%s]", IResource::getResourseName().c_str());
        ASSERT(false, "Stream parse error");
        return false;
    }

    tinyxml2::XMLElement* rootElement = doc.FirstChildElement("technique");
    if (!rootElement)
    {
        LOG_ERROR("RenderTechnique: Can't find technique in Stream name [%s]", IResource::getResourseName().c_str());
        ASSERT(false, "Can't find technique in Stream");
        return false;
    }

    return RenderTechnique::parse(rootElement);

}

RenderTechnique* RenderTechnique::clone() const
{
    RenderTechnique* technique = new RenderTechnique();
    technique->operator=(*this);

    RenderTechniqueManager::getInstance()->add(technique);

    return technique;
}

} //namespace renderer
} //namespace v3d
