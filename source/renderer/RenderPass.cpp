#include "RenderPass.h"
#include "utils/Logger.h"

using namespace f3d;
using namespace f3d::renderer;

CRenderPass::CRenderPass()
{
}

CRenderPass::~CRenderPass()
{
}

bool CRenderPass::parse(tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("Error parse.  Cannot read render pass element");
        return false;
    }

    const std::string passName = root->Attribute("name");
    if (passName.empty())
    {
        LOG_ERROR("Error parse. Cannot read render pass name");
        return false;
    }

    m_name = passName;

    //uniforms
    tinyxml2::XMLElement*  uniformsElement = root->FirstChildElement("uniforms");
    if (uniformsElement)
    {
        if (!parseUniforms(uniformsElement))
        {
            return false;
        }
    }

    //attributes
    tinyxml2::XMLElement*  attributesElement = root->FirstChildElement("attributes");
    if (attributesElement)
    {
        if (!parseAttributes(attributesElement))
        {
            return false;
        }
    }

    //samplers
    tinyxml2::XMLElement*  samplersElement = root->FirstChildElement("samplers");
    if (samplersElement)
    {
        if (!parseSamplers(samplersElement))
        {
            return false;
        }
    }

    //rendertarget
    tinyxml2::XMLElement*  rendertargetElement = root->FirstChildElement("rendertarget");
    if (rendertargetElement)
    {
        if (!parseRenderTarget(rendertargetElement))
        {
            return false;
        }
    }


    //shaders
    tinyxml2::XMLElement*  shadersElement = root->FirstChildElement("shaders");
    if (shadersElement)
    {
        if (!parseShaders(shadersElement))
        {
            return false;
        }
    }

    return true;
}

bool CRenderPass::parseUniforms(tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("Error parse. Not exist xml element");
        return false;
    }

    tinyxml2::XMLElement* varElement = root->FirstChildElement("uniforms");
    while (varElement)
    {
        const std::string varName = varElement->Attribute("name");
        if (varName.empty())
        {
            LOG_ERROR("Cannot find uniform name from pass '%s'", m_name);
            return false;
        }

        const std::string varType = varElement->Attribute("type");
        if (varType.empty())
        {
            LOG_ERROR("Cannot find uniform type from pass '%s' in '%s'", m_name, varName);
            return false;
        }

        const std::string varVal = varElement->Attribute("val");
        if (varVal.empty())
        {
            LOG_ERROR("Cannot find uniform val from pass '%s' in '%s'", m_name, varName);
            return false;
        }



        varElement = varElement->NextSiblingElement("uniforms");
    }



    return true;
}

bool CRenderPass::parseAttributes(tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("Error parse. Not exist xml element");
        return false;
    }
}

bool CRenderPass::parseSamplers(tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("Error parse. Not exist xml element");
        return false;
    }
}

bool CRenderPass::parseRenderTarget(tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("Error parse. Not exist xml element");
        return false;
    }
}

bool CRenderPass::parseShaders(tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("Error parse. Not exist xml element");
        return false;
    }
}