#include "RenderState.h"

using namespace v3d;
using namespace v3d::renderer;

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

CRenderState::CRenderState()
    : m_polygonMode(EPolygonMode::ePolyModeFill)
    , m_winding(EWinding::eWindingCW)
    , m_cullFace(true)
    , m_blendDst(EBlendFactor::eBlendDstAlpha)
    , m_blendSrc(EBlendFactor::eBlendInvSrcAlpha)
    , m_blend(false)
{
}

CRenderState::~CRenderState()
{
}

bool CRenderState::getCullFace() const
{
    return m_cullFace;
}

EWinding CRenderState::getWinding() const
{
    return m_winding;
}

EPolygonMode CRenderState::getPolygonMode() const
{
    return m_polygonMode;
}

void CRenderState::setCullFace(bool use)
{
    m_cullFace = use;
}

void CRenderState::setWinding(EWinding type)
{
    m_winding = type;
}

void CRenderState::setPolygonMode(EPolygonMode type)
{
    m_polygonMode = type;
}

void CRenderState::setBlend(bool use)
{
    m_blend = use;
}

bool CRenderState::getBlend() const
{
    return m_blend;
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
