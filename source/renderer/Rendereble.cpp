#include "Rendereble.h"

using namespace v3d;
using namespace renderer;

CRendereble::CRendereble()
: m_material(nullptr)
, m_geometry(nullptr)
, m_renderJob(nullptr)
{
}

CRendereble::~CRendereble()
{
}

void CRendereble::setMaterial(const renderer::MaterialPtr& material)
{
    m_material = material;
}

void CRendereble::setRenderJob(const RenderJobPtr& job)
{
    m_renderJob = job;
}

void CRendereble::setGeometry(const GeometryPtr& geometry)
{
    m_geometry = geometry;
}


const MaterialPtr& CRendereble::getMaterial() const
{
    return m_material;
}

const RenderJobPtr& CRendereble::getRenderJob() const
{
    return m_renderJob;
}

const GeometryPtr& CRendereble::getGeometry() const
{
    return m_geometry;
}