#include "Geometry.h"

using namespace v3d;
using namespace v3d::renderer;

CGeometry::CGeometry(const RenderTechniquePtr& technique)
    : m_drawMode(EDrawMode::eTriangleStrip)
    , m_geometyType(EGeomertyType::eGeomertyStatic)
    , m_technique(technique)
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

void CGeometry::copyToVertices(const f32 vertices[][3], u32 size)
{
    for (u32 i = 0; i < size; ++i)
    {
        m_data._vertices[i] = vertices[i];
    }
}

void CGeometry::copyToNormals(const f32 normals[][3], u32 size)
{
    for (u32 i = 0; i < size; ++i)
    {
        m_data._normals[i] = normals[i];
    }
}

void CGeometry::copyToTexCoords(const f32 texCoords[][2], u32 layer, u32 size)
{
    for (u32 i = 0; i < size; ++i)
    {
        m_data._texCoords[layer][i] = texCoords[i];
    }
}

void CGeometry::copyToIndices(const u32* indices, u32 size)
{
    std::copy(indices, indices + size, m_data._indices.begin());
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
