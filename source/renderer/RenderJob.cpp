#include "RenderJob.h"
#include "scene/Light.h"
#include "Engine.h"

using namespace v3d;
using namespace core;
using namespace renderer;

CRenderJob::CRenderJob(const MaterialPtr& material, const GeometryPtr& geometry, const Matrix4D& transform)
    : m_material(material)
    , m_geometry(geometry)
    , m_transform(transform)
    , m_targetIndex(0U)
{
}

CRenderJob::~CRenderJob()
{
}

void CRenderJob::setMaterial(const MaterialPtr& material)
{
   m_material = material;
}

const MaterialPtr& CRenderJob::getMaterial() const
{
    return m_material;
}

void CRenderJob::setGeometry(const GeometryPtr& geometry)
{
    m_geometry = geometry;
}

const GeometryPtr& CRenderJob::getGeometry() const
{
    return m_geometry;
}

void CRenderJob::setTransform(const Matrix4D& transform)
{
    m_transform = transform;
}

const Matrix4D& CRenderJob::getTransform() const
{
    return m_transform;
}

void CRenderJob::setRenderTarget(u32 target)
{
    m_targetIndex = target;
}
u32 CRenderJob::getRenderTarget() const
{
    return m_targetIndex;
}
