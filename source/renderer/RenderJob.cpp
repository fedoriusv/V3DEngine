#include "RenderJob.h"
#include "Material.h"
#include "Engine.h"

namespace v3d
{
namespace renderer
{

using namespace core;

CRenderJob::CRenderJob(const MaterialPtr& material, const GeometryPtr& geometry, const Matrix4D& transform)
    : m_material(material)
    , m_geometry(geometry)
    , m_transform(transform)
    , m_targetIndex(0U)
{
}

CRenderJob::~CRenderJob()
{
    CRenderJob::clearRenderPassIndexList();
}

void CRenderJob::setMaterial(const MaterialPtr& material)
{
   m_material = material;
}

const MaterialPtr& CRenderJob::getMaterial() const
{
    return m_material;
}

MaterialPtr& CRenderJob::getMaterial()
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
