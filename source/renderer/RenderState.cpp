#include "RenderState.h"
#include "utils/Logger.h"

#include "tinyxml2.h"

using namespace v3d;
using namespace renderer;

const std::string CRenderState::s_renderPolygonMode[eModeCount] =
{
    "fill",
    "line",
    "point"
};

EPolygonMode CRenderState::getPolygonModeByName(const std::string& name)
{
    for (u32 i = 0; i < eModeCount; ++i)
    {
        if (s_renderPolygonMode[i].compare(name) == 0)
        {
            return (EPolygonMode)i;
        }
    }

    return ePolyModeFill;
}

const std::string CRenderState::s_blendFactor[eBlendCount] =
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
    for (u32 i = 0; i < eBlendCount; ++i)
    {
        if (s_blendFactor[i].compare(name) == 0)
        {
            return (EBlendFactor)i;
        }
    }

    return eBlendZero;
}

const std::string CRenderState::s_cullface[eCullfaceCount] =
{
    "front",
    "back",
    "frontAndBack"
};

ECullFace CRenderState::getCullFaceByName(const std::string& name)
{
    for (u32 i = 0; i < eCullfaceCount; ++i)
    {
        if (s_cullface[i].compare(name) == 0)
        {
            return (ECullFace)i;
        }
    }

    return eFaceBack;
}

const std::string CRenderState::s_comparefunc[eCompareCount] =
{
    "less",
    "lequal",
    "equal",
    "gequal",
    "greater",
    "notequal",
    "always",
    "never"
};

ECompareFunc CRenderState::getCompareFuncByName(const std::string& name)
{
    for (u32 i = 0; i < eCompareCount; ++i)
    {
        if (s_comparefunc[i].compare(name) == 0)
        {
            return (ECompareFunc)i;
        }
    }

    return eCmpLequal;
}

CRenderState::CRenderState()
    : m_culling(true)
    , m_cullface(eFaceBack)
    , m_winding(eWindingCCW)
    , m_polygonMode(ePolyModeFill)

    , m_blendSrc(eBlendInvSrcAlpha)
    , m_blendDst(eBlendDstAlpha)
    , m_blend(false)

    , m_depthWrite(true)
    , m_depthTest(true)
    , m_depthFunc(eCmpLequal)
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

bool CRenderState::getDepthWrite() const
{
    return m_depthWrite;
}

bool CRenderState::getDepthTest() const
{
    return m_depthTest;
}

ECompareFunc CRenderState::getDepthfunc() const
{
    return m_depthFunc;
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

void CRenderState::setDepthWrite(bool enable)
{
    m_depthWrite = enable;
}

void CRenderState::setDepthTest(bool enable)
{
    m_depthTest = enable;
}

void CRenderState::setDepthFunc(ECompareFunc func)
{
    m_depthFunc = func;
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

    const tinyxml2::XMLElement* cullfaceElement = root->FirstChildElement("culling");
    if (cullfaceElement)
    {
        if (cullfaceElement->Attribute("val"))
        {
            const std::string cullfaceStr = cullfaceElement->Attribute("val");
            if (cullfaceStr == "false")
            {
                CRenderState::setCulling(false);
            }
            ECullFace cullface = CRenderState::getCullFaceByName(cullfaceStr);
            CRenderState::setCullface(cullface);
        }
    }

    const tinyxml2::XMLElement* blendElement = root->FirstChildElement("blending");
    if (blendElement)
    {
        bool blend = blendElement->BoolAttribute("val");
        CRenderState::setBlend(blend);

        EBlendFactor blendSrc = eBlendInvSrcAlpha;
        if (blendElement->Attribute("src"))
        {
            const std::string blendSrcStr = blendElement->Attribute("src");
            blendSrc = CRenderState::getBlendFactorByName(blendSrcStr);
        }

        EBlendFactor blendDst = eBlendDstAlpha;
        if (blendElement->Attribute("dst"))
        {
            const std::string blendDstStr = blendElement->Attribute("dst");
            blendDst = CRenderState::getBlendFactorByName(blendDstStr);
        }

        CRenderState::setBlendFactors(blendDst, blendSrc);
    }

    const tinyxml2::XMLElement* depthElement = root->FirstChildElement("depthWrite");
    if (depthElement)
    {
        bool depth = depthElement->BoolAttribute("val");
        CRenderState::setDepthWrite(depth);
    }

    const tinyxml2::XMLElement* depthTestElement = root->FirstChildElement("depthTest");
    if (depthTestElement)
    {
        if (depthTestElement->Attribute("val"))
        {
            const std::string depthTestStr = depthTestElement->Attribute("val");
            if (depthTestStr == "false")
            {
                CRenderState::setDepthTest(false);
            }

            ECompareFunc depthFunc = CRenderState::getCompareFuncByName(depthTestStr);
            CRenderState::setDepthFunc(depthFunc);
        }
    }

    return true;
}
