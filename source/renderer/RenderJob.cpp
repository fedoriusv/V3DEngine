#include "RenderJob.h"
#include "scene/Light.h"
#include "Engine.h"

using namespace v3d;
using namespace v3d::renderer;

CRenderJob::CRenderJob(const MaterialPtr& material, const GeometryPtr& geomerty, const Matrix4D& transform)
    : m_material(material)
    , m_geomerty(geomerty)
    , m_transform(transform)
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

void CRenderJob::setGeomery(const GeometryPtr& geomerty)
{
   m_geomerty = geomerty;
}

const GeometryPtr& CRenderJob::getGeometry() const
{
    return m_geomerty;
}

void CRenderJob::setTransform(const Matrix4D& transform)
{
    m_transform = transform;
}

const Matrix4D& CRenderJob::getTransform() const
{
    return m_transform;
}
