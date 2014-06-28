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

void CGeometry::copyToTexCoords(const f32 texCoords[][2], u32 laye, u32 size)
{
    for (u32 i = 0; i < size; ++i)
    {
        m_data._texCoords[laye][i] = texCoords[i];
    }
}

void CGeometry::copyToIndices(const u32* indices, u32 size)
{
    std::copy(indices, indices + size, m_data._indices.begin());
}

