#include "RenderState.h"
#include "utils/Logger.h"

#include "tinyxml2.h"

namespace v3d
{
namespace renderer
{

const std::string RenderState::s_renderPolygonMode[eModeCount] =
{
    "fill",
    "line",
    "point"
};

EPolygonMode RenderState::getPolygonModeByName(const std::string& name)
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

const std::string RenderState::s_blendFactor[eBlendCount] =
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

EBlendFactor RenderState::getBlendFactorByName(const std::string& name)
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

const std::string RenderState::s_cullface[eCullfaceCount] =
{
    "none",
    "front",
    "back",
    "frontAndBack"
};

ECullMode RenderState::getCullFaceByName(const std::string& name)
{
    for (u32 i = 0; i < eCullfaceCount; ++i)
    {
        if (s_cullface[i].compare(name) == 0)
        {
            return (ECullMode)i;
        }
    }

    return eFaceBack;
}

const std::string RenderState::s_comparefunc[eCompareCount] =
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

ECompareFunc RenderState::getCompareFuncByName(const std::string& name)
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

void RenderState::updateHash()
{
    if (m_isChanged)
    {

    }
}

RenderState::RenderState()
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

    , m_isRasterizerEnable(true)

    , m_isChanged(true)
{
}

RenderState::RenderState(const RenderState& state)
    : m_culling(state.m_culling)
    , m_cullface(state.m_cullface)
    , m_winding(state.m_winding)
    , m_polygonMode(state.m_polygonMode)

    , m_blendSrc(state.m_blendSrc)
    , m_blendDst(state.m_blendDst)
    , m_blend(state.m_blend)

    , m_depthWrite(state.m_depthWrite)
    , m_depthTest(state.m_depthTest)
    , m_depthFunc(state.m_depthFunc)

    , m_isRasterizerEnable(state.m_isRasterizerEnable)

    , m_isChanged(true)
{
}

RenderState& RenderState::operator=(const RenderState& state)
{
    if (&state == this)
    {
        return *this;
    }

    m_culling = state.m_culling;
    m_cullface = state.m_cullface;
    m_winding = state.m_winding;
    m_polygonMode = state.m_polygonMode;

    m_blendSrc = state.m_blendSrc;
    m_blendDst = state.m_blendDst;
    m_blend = state.m_blend;

    m_depthWrite = state.m_depthWrite;
    m_depthTest = state.m_depthTest;
    m_depthFunc = state.m_depthFunc;

    m_isRasterizerEnable = state.m_isRasterizerEnable;

    m_isChanged = true;

    return *this;
}

RenderState::~RenderState()
{
}

bool RenderState::isCulling() const
{
    return m_culling;
}

EWinding RenderState::getWinding() const
{
    return m_winding;
}

EPolygonMode RenderState::getPolygonMode() const
{
    return m_polygonMode;
}

void RenderState::setCulling(bool enable)
{
    if (m_culling != enable)
    {
        m_culling = enable;
        m_isChanged = true;
    }
}

void RenderState::setWinding(EWinding type)
{
    if (m_winding != type)
    {
        m_winding = type;
        m_isChanged = true;
    }
}

void RenderState::setPolygonMode(EPolygonMode type)
{
    if (m_polygonMode != type)
    {
        m_polygonMode = type;
        m_isChanged = true;
    }
}

void RenderState::setBlend(bool enable)
{
    if (m_blend != enable)
    {
        m_blend = enable;
        m_isChanged = true;
    }
}

bool RenderState::isBlend() const
{
    return m_blend;
}

ECullMode RenderState::getCullface() const
{
    return m_cullface;
}

bool RenderState::getDepthWrite() const
{
    return m_depthWrite;
}

bool RenderState::getDepthTest() const
{
    return m_depthTest;
}

ECompareFunc RenderState::getDepthfunc() const
{
    return m_depthFunc;
}

bool RenderState::isRasterizerEnable() const
{
    return m_isRasterizerEnable;
}

void RenderState::setCullface(ECullMode mode)
{
    if (m_cullface != mode)
    {
        m_cullface = mode;
        m_isChanged = true;
    }
}

void RenderState::setBlendFactors(EBlendFactor dst, EBlendFactor src)
{
    if (m_blendDst != dst || m_blendSrc != src)
    {
        m_blendDst = dst;
        m_blendSrc = src;
        m_isChanged = true;
    }
}

void RenderState::setDepthWrite(bool enable)
{
    if (m_depthWrite != enable)
    {
        m_depthWrite = enable;
        m_isChanged = true;
    }
}

void RenderState::setDepthTest(bool enable)
{
    if (m_depthTest != enable)
    {
        m_depthTest = enable;
        m_isChanged = true;
    }
}

void RenderState::setDepthFunc(ECompareFunc func)
{
    if (m_depthFunc != func)
    {
        m_depthFunc = func;
        m_isChanged = true;
    }
}

void RenderState::setRasterizerEnable(bool enable)
{
    if (m_isRasterizerEnable != enable)
    {
        m_isRasterizerEnable = enable;
        m_isChanged = true;
    }
}

EBlendFactor RenderState::getBlendFactorSrc() const
{
    return m_blendSrc;
}

EBlendFactor RenderState::getBlendFactorDst() const
{
    return m_blendDst;
}

bool RenderState::parse(const tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("CRenderState: Not exist xml renderstate element");
        return false;
    }

    const tinyxml2::XMLElement* polygonmodeElement = root->FirstChildElement("polygonmode");
    if (polygonmodeElement)
    {
        if (polygonmodeElement->Attribute("val"))
        {
            const std::string polygonModeStr = polygonmodeElement->Attribute("val");
            EPolygonMode polygonMode = RenderState::getPolygonModeByName(polygonModeStr);
            RenderState::setPolygonMode(polygonMode);
        }
    }


    const tinyxml2::XMLElement* windingElement = root->FirstChildElement("winding");
    if (windingElement)
    {
        if (windingElement->Attribute("val"))
        {
            const std::string windingStr = windingElement->Attribute("val");
            EWinding  winding = (windingStr == "ccw") ? eWindingCCW : eWindingCW;
            RenderState::setWinding(winding);
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
                RenderState::setCulling(false);
            }
            ECullMode cullface = RenderState::getCullFaceByName(cullfaceStr);
            RenderState::setCullface(cullface);
        }
    }

    const tinyxml2::XMLElement* blendElement = root->FirstChildElement("blending");
    if (blendElement)
    {
        bool blend = blendElement->BoolAttribute("val");
        RenderState::setBlend(blend);

        EBlendFactor blendSrc = eBlendInvSrcAlpha;
        if (blendElement->Attribute("src"))
        {
            const std::string blendSrcStr = blendElement->Attribute("src");
            blendSrc = RenderState::getBlendFactorByName(blendSrcStr);
        }

        EBlendFactor blendDst = eBlendDstAlpha;
        if (blendElement->Attribute("dst"))
        {
            const std::string blendDstStr = blendElement->Attribute("dst");
            blendDst = RenderState::getBlendFactorByName(blendDstStr);
        }

        RenderState::setBlendFactors(blendDst, blendSrc);
    }

    const tinyxml2::XMLElement* depthElement = root->FirstChildElement("depthWrite");
    if (depthElement)
    {
        bool depth = depthElement->BoolAttribute("val");
        RenderState::setDepthWrite(depth);
    }

    const tinyxml2::XMLElement* depthTestElement = root->FirstChildElement("depthTest");
    if (depthTestElement)
    {
        if (depthTestElement->Attribute("val"))
        {
            const std::string depthTestStr = depthTestElement->Attribute("val");
            if (depthTestStr == "false")
            {
                RenderState::setDepthTest(false);
            }

            ECompareFunc depthFunc = RenderState::getCompareFuncByName(depthTestStr);
            RenderState::setDepthFunc(depthFunc);
        }
    }

    return true;
}

} //namespace renderer
} //namespace v3d
