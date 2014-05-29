#include "RenderState.h"

using namespace v3d;
using namespace v3d::renderer;


CRenderState::CRenderState()
{
}

CRenderState::~CRenderState()
{
}

ERenderCullFace CRenderState::getCullFace() const
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

void CRenderState::setCullFace(ERenderCullFace type)
{
    m_cullFace = type;
}

void CRenderState::getWinding(ERenderWinding type)
{
    m_winding = type;
}

void CRenderState::getPolygonMode(ERenderPolygonMode type)
{
    m_polygonMode = type;
}