#include "RenderState.h"

using namespace v3d;
using namespace v3d::renderer;

const std::string CRenderState::s_renderPolygonMode[EPolygonMode::eModeCount] =
{
    "fill",
    "line",
    "point"
};

CRenderState::EPolygonMode CRenderState::getPolygonModeByName(const std::string& name)
{
    for (int i = 0; i < CRenderState::eModeCount; ++i)
    {
        if (s_renderPolygonMode[i].compare(name) == 0)
        {
            return (CRenderState::EPolygonMode)i;
        }
    }

    return CRenderState::ePolyModeFill;
}

CRenderState::CRenderState()
    : m_polygonMode(EPolygonMode::ePolyModeFill)
    , m_winding(EWinding::eWindingCW)
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

CRenderState::EWinding CRenderState::getWinding() const
{
    return m_winding;
}

CRenderState::EPolygonMode CRenderState::getPolygonMode() const
{
    return m_polygonMode;
}

void CRenderState::setCullFace(bool type)
{
    m_cullFace = type;
}

void CRenderState::setWinding(EWinding type)
{
    m_winding = type;
}

void CRenderState::setPolygonMode(EPolygonMode type)
{
    m_polygonMode = type;
}
