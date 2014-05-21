#include "RenderPass.h"
#include "Fedoria3D.h"
#include "renderer/GL/ShaderProgramGL.h"

using namespace f3d;
using namespace f3d::renderer;

CRenderPass::CRenderPass()
    : m_program(nullptr)
{
    CRenderPass::init();
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

    m_program->parse(root);

    //rendertarget
    tinyxml2::XMLElement*  rendertargetElement = root->FirstChildElement("rendertarget");
    if (rendertargetElement)
    {
        if (!parseRenderTarget(rendertargetElement))
        {
            return false;
        }
    }

    //renderstate
    tinyxml2::XMLElement*  renderstateElement = root->FirstChildElement("renderstate");
    if (renderstateElement)
    {
        if (!parseRenderState(renderstateElement))
        {
            return false;
        }
    }

    return true;
}

void CRenderPass::init()
{
    platform::EDriverType type = f3d::CEngine::getInstance()->getPlatform()->getDriverType();

    switch (type)
    {
        case platform::EDriverType::eDriverOpenGL:
        {
            m_program = std::make_shared<CShaderProgramGL>(CShaderProgramGL());
        }
            break;

        case platform::EDriverType::eDriverDirect3D:
        {
            //m_geometry = std::make_shared<CGeometry>(CGeometryGL());
        }
            break;

        default:
        {
            LOG_ERROR("Can`t Choose driver type");
        }
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

bool CRenderPass::parseRenderState(tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("Error parse. Not exist xml element");
        return false;
    }
}