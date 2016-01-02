#include "Renderable.h"
#include "Material.h"
#include "Engine.h"

using namespace v3d;
using namespace renderer;

CRenderable::CRenderable()
: m_material(nullptr)
, m_renderJob(nullptr)
{
}

CRenderable::~CRenderable()
{
}

void CRenderable::setMaterial(const MaterialPtr& material)
{
    m_material = material;
}

void CRenderable::setRenderJob(const RenderJobPtr& job)
{
    m_renderJob = job;
}

void CRenderable::setGeometry(const GeometryPtr& geometry)
{
    m_geometry = geometry;
}


const MaterialPtr& CRenderable::getMaterial() const
{
    return m_material;
}

const RenderJobPtr& CRenderable::getRenderJob() const
{
    return m_renderJob;
}

const GeometryPtr& CRenderable::getGeometry() const
{
    return m_geometry;
}

void CRenderable::render()
{
    ASSERT(CRenderable::getRenderJob(), "CRenderable: Render job nullptr");
    RENDERER->draw(CRenderable::getRenderJob());
}