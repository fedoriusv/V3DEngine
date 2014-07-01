#include "RenderLOD.h"

using namespace v3d;
using namespace v3d::renderer;

CRenderLOD::CRenderLOD()
    : m_geometryLod(0)
{
}

CRenderLOD::~CRenderLOD()
{
}

void CRenderLOD::setGeometryDistance(const f32 lod)
{
    if (lod > 0)
    {
        m_geometryLod = lod;
    }
}

f32 CRenderLOD::getGeometryDistance() const
{
    return m_geometryLod;
}