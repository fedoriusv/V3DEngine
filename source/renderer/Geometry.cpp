#include "Geometry.h"

using namespace v3d;
using namespace v3d::renderer;

CGeometry::CGeometry(const RenderTechniquePtr& technique)
    : m_drawMode(EDrawMode::eTriangleStrip)
    , m_geometyType(EGeomertyType::eGeomertyStatic)
    , m_technique(technique)
    , m_currentVertexMask(0U)
{
}

CGeometry::~CGeometry()
{
    m_data.clear();
}

SVertexData& CGeometry::getData()
{
    return m_data;
}

CGeometry::EDrawMode CGeometry::getDrawMode() const
{
    return m_drawMode;
}

void CGeometry::setDrawMode(EDrawMode mode)
{
    m_drawMode = mode;
}

void CGeometry::setInterval(u32 begin, u32 count)
{
    m_interval._begin = begin;
    m_interval._count = count;
}

void CGeometry::addVertex(const core::Vector3D& vertex)
{
    m_data._vertices.push_back(vertex);
}
void CGeometry::addNormal(const core::Vector3D& normal)
{
    m_data._normals.push_back(normal);
}

void CGeometry::addBinormal(const core::Vector3D& binormal)
{
    m_data._binormals.push_back(binormal);
}

void CGeometry::addTangent(const core::Vector3D& tangent)
{
    m_data._tangents.push_back(tangent);
}

void CGeometry::addColor(const core::Vector3D& color)
{
    m_data._colors.push_back(color);
}

void CGeometry::addTexCoord(u32 layer, const core::Vector2D& texCoord)
{
    if (layer >= m_data._texCoords.size())
    {
        m_data._texCoords.resize(layer + 1);
    }
    m_data._texCoords[layer].push_back(texCoord);
}

void CGeometry::addIndex(u32 index)
{
    m_data._indices.push_back(index);
}

void CGeometry::setVertexMask(u32 mask)
{
    m_currentVertexMask = mask;
}

const CGeometry::SInterval& CGeometry::getInterval() const
{
    return m_interval;
}

void CGeometry::draw()
{
    u32 passIdx = m_technique->getCurrentPass();
    const RenderPassPtr& pass = m_technique->getRenderPass(passIdx);
    CGeometry::setVertexMask(pass->getDefaultShaderData()->getVertexFormatMask());
}

bool CGeometry::updated() const
{
    u32 passIdx = m_technique->getCurrentPass();
    const RenderPassPtr& pass = m_technique->getRenderPass(passIdx);
    
    return m_currentVertexMask != pass->getDefaultShaderData()->getVertexFormatMask();
}

