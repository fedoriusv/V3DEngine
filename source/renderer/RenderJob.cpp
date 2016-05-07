#include "RenderJob.h"
#include "Renderable.h"
#include "Engine.h"

namespace v3d
{
namespace renderer
{

using namespace core;

CRenderJob::CRenderJob(const Renderable* renderable, const Matrix4D& transform)
    : m_renderable(renderable)
    , m_transform(transform)
    , m_targetIndex(0U)
{
}

CRenderJob::~CRenderJob()
{
    CRenderJob::clearRenderPassIndexList();
}

const MaterialPtr& CRenderJob::getMaterial() const
{
    ASSERT(m_renderable, "Renderable is nullptr");
    return m_renderable->getMaterial();
}

MaterialPtr& CRenderJob::getMaterial()
{
    ASSERT(m_renderable, "Renderable is nullptr");
    return const_cast<Renderable*>(m_renderable)->getMaterial();
}

const GeometryPtr& CRenderJob::getGeometry() const
{
    ASSERT(m_renderable, "Renderable is nullptr");
    return m_renderable->getGeometry();
}

void CRenderJob::clearRenderPassIndexList()
{
    m_passIndexList.clear();
}

PassIndexIterConst CRenderJob::renderPassIndexBegin() const
{
    return m_passIndexList.cbegin();
}

PassIndexIterConst CRenderJob::renderPassIndexEnd() const
{
    return m_passIndexList.cend();
}

const CRenderTechnique* CRenderJob::getRenderTechique() const
{
    ASSERT(m_renderable, "Renderable is nullptr");
    return m_renderable->getRenderTechique();
}

CRenderTechnique* CRenderJob::getRenderTechique()
{
    ASSERT(m_renderable, "Renderable is nullptr");
    return const_cast<Renderable*>(m_renderable)->getRenderTechique();
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

void CRenderJob::addRenderPassIndex(u32 pass)
{
    if (std::find(m_passIndexList.cbegin(), m_passIndexList.cend(), pass) == m_passIndexList.cend())
    {
        m_passIndexList.push_back(pass);
    }
}

} //namespace renderer
} //namespace v3d
