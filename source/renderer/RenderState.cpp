#include "RenderState.h"
#include "utils/Logger.h"

#include "tinyxml2.h"

using namespace v3d;
using namespace renderer;

const std::string CRenderState::s_renderPolygonMode[EPolygonMode::eModeCount] =
{
    "fill",
    "line",
    "point"
};

EPolygonMode CRenderState::getPolygonModeByName(const std::string& name)
{
    for (int i = 0; i < EPolygonMode::eModeCount; ++i)
    {
        if (s_renderPolygonMode[i].compare(name) == 0)
        {
            return (EPolygonMode)i;
        }
    }

    return EPolygonMode::ePolyModeFill;
}

const std::string CRenderState::s_blendFactor[EBlendFactor::eBlendCount] =
{
    "zero",
    "one",
    "srcColor",
    "invSrcColor",
    "dstColor",
    "invDstColor",
    "srcAlpha",
    "invSrcAlpha",
    "dstAlpha",
    "invDstAlpha",
    "srcAplhaSaturated"
};

EBlendFactor CRenderState::getBlendFactorByName(const std::string& name)
{
    for (int i = 0; i < EBlendFactor::eBlendCount; ++i)
    {
        if (s_blendFactor[i].compare(name) == 0)
        {
            return (EBlendFactor)i;
        }
    }

    return EBlendFactor::eBlendZero;
}

const std::string CRenderState::s_cullface[ECullFace::eCullfaceCount] =
{
    "front",
    "back",
    "frontAndBack"
};

ECullFace CRenderState::getCullFaceByName(const std::string& name)
{
    for (int i = 0; i < ECullFace::eCullfaceCount; ++i)
    {
        if (s_cullface[i].compare(name) == 0)
        {
            return (ECullFace)i;
        }
    }

    return ECullFace::eFaceFront;
}

CRenderState::CRenderState()
    : m_culling(true)
    , m_cullface(eFaceBack)
    , m_winding(eWindingCW)
    , m_polygonMode(ePolyModeFill)

    , m_blendDst(EBlendFactor::eBlendDstAlpha)
    , m_blendSrc(EBlendFactor::eBlendInvSrcAlpha)
    , m_blend(false)
{
}

CRenderState::~CRenderState()
{
}

bool CRenderState::getCulling() const
{
    return m_culling;
}

EWinding CRenderState::getWinding() const
{
    return m_winding;
}

EPolygonMode CRenderState::getPolygonMode() const
{
    return m_polygonMode;
}

void CRenderState::setCulling(bool enable)
{
    m_culling = enable;
}

void CRenderState::setWinding(EWinding type)
{
    m_winding = type;
}

void CRenderState::setPolygonMode(EPolygonMode type)
{
    m_polygonMode = type;
}

void CRenderState::setBlend(bool enable)
{
    m_blend = enable;
}

bool CRenderState::getBlend() const
{
    return m_blend;
}

ECullFace CRenderState::getCullface() const
{
    return m_cullface;
}

void CRenderState::setCullface(ECullFace mode)
{
    m_cullface = mode;
}

void CRenderState::setBlendFactors(EBlendFactor dst, EBlendFactor src)
{
    m_blendDst = dst;
    m_blendSrc = src;
}

EBlendFactor CRenderState::getBlendFactorSrc() const
{
    return m_blendSrc;
}

EBlendFactor CRenderState::getBlendFactorDst() const
{
    return m_blendDst;
}

bool CRenderState::parse(const tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("CRenderPass: Not exist xml renderstate element");
        return false;
    }

    const tinyxml2::XMLElement* polygonmodeElement = root->FirstChildElement("polygonmode");
    if (polygonmodeElement)
    {
        if (polygonmodeElement->Attribute("val"))
        {
            const std::string polygonModeStr = polygonmodeElement->Attribute("val");
            EPolygonMode polygonMode = CRenderState::getPolygonModeByName(polygonModeStr);
            CRenderState::setPolygonMode(polygonMode);
        }
    }


    const tinyxml2::XMLElement* windingElement = root->FirstChildElement("winding");
    if (windingElement)
    {
        if (windingElement->Attribute("val"))
        {
            const std::string windingStr = windingElement->Attribute("val");
            EWinding  winding = (windingStr == "ccw") ? eWindingCCW : eWindingCW;
            CRenderState::setWinding(winding);
        }
    }

    const tinyxml2::XMLElement* cullfaceElement = root->FirstChildElement("cullface");
    if (cullfaceElement)
    {
        bool culling = cullfaceElement->BoolAttribute("val");
        CRenderState::setCulling(culling);

        if (cullfaceElement->Attribute("cull"))
        {
            const std::string cullfaceStr = cullfaceElement->Attribute("cull");
            ECullFace cullface = CRenderState::getCullFaceByName(cullfaceStr);
            CRenderState::setCullface(cullface);
        }
    }

    /*bool blend = root->BoolAttribute("blend");
    m_renderState->setBlend(blend);

    if (root->Attribute("blendfactordst") && root->Attribute("blendfactorsrc"))
    {
        const std::string blendFactorDstStr = root->Attribute("blendfactordst");
        EBlendFactor blendFactorDst = CRenderState::getBlendFactorByName(blendFactorDstStr);

        const std::string blendFactorSrcStr = root->Attribute("blendfactorsrc");
        EBlendFactor blendFactorSrc = CRenderState::getBlendFactorByName(blendFactorSrcStr);

        m_renderState->setBlendFactors(blendFactorDst, blendFactorSrc);
    }*/

    return true;
}
