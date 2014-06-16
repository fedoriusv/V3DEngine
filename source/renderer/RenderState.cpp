#include "RenderState.h"

using namespace v3d;
using namespace v3d::renderer;

const std::string CRenderState::s_renderPolygonMode[ERenderPolygonMode::ePolygonModeCount] =
{
    "fill",
    "line",
    "point"
};

ERenderPolygonMode CRenderState::getPolygonModeByName(const std::string& name)
{
    for (int i = 0; i < ERenderPolygonMode::ePolygonModeCount; ++i)
    {
        if (s_renderPolygonMode[i].compare(name) == 0)
        {
            return (ERenderPolygonMode)i;
        }
    }

    return ERenderPolygonMode::ePolyModeFill;
}

CRenderState::CRenderState()
    : m_polygonMode(ERenderPolygonMode::ePolyModeFill)
    , m_winding(ERenderWinding::eWindingCW)
    , m_cullFace(true)
{
}

CRenderState::~CRenderState()
{
}

bool CRenderState::getCullFace() const
{
    return m_cullFace;
}

ERenderWinding CRenderState::getWinding() const
{
    return m_winding;
}

ERenderPolygonMode CRenderState::getPolygonMode() const
{
    return m_polygonMode;
}

void CRenderState::setCullFace(bool type)
{
    m_cullFace = type;
}

void CRenderState::setWinding(ERenderWinding type)
{
    m_winding = type;
}

void CRenderState::setPolygonMode(ERenderPolygonMode type)
{
    m_polygonMode = type;
}