#include "RenderLOD.h"

namespace v3d
{
namespace renderer
{

CRenderLOD::CRenderLOD()
    : m_geometryLod(0)
{
}

v3d::renderer::CRenderLOD::CRenderLOD(const CRenderLOD& lod)
    : m_geometryLod(lod.m_geometryLod)
{
}

CRenderLOD& CRenderLOD::operator=(const CRenderLOD& lod)
{
    if (&lod == this)
    {
        return *this;
    }

    m_geometryLod = lod.m_geometryLod;

    return *this;
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

} //namespace renderer
} //namespace v3d
