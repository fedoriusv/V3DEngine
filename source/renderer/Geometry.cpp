#include "Geometry.h"

using namespace v3d;
using namespace v3d::renderer;

CGeometry::CGeometry(const RenderTechiquePtr& techique)
	: m_drawMode(EDrawMode::eTriangleStrip)
    , m_geometyType(EGeomertyType::eGeomertyStatic)
    , m_techique(techique)
{
}

CGeometry::~CGeometry()
{
}

SVertexData& CGeometry::getData()
{
    return m_data;
}

EDrawMode CGeometry::getDrawMode() const
{
    return m_drawMode;
}

void CGeometry::setDrawMode(EDrawMode mode)
{
    m_drawMode = mode;
}

void CGeometry::copyVertices(const f32 vertices[][3], u32 size)
{
    for (u32 i = 0; i < size; ++i)
    {
        m_data.m_vertices.vertex[i] = vertices[i];
    }
}

void CGeometry::copyNormals(const f32 normals[][3], u32 size)
{
    for (u32 i = 0; i < size; ++i)
    {
        m_data.m_normals.vertex[i] = normals[i];
    }
}

void CGeometry::copyTexCoords(const f32 texCoords[][2], u32 laye, u32 size)
{
    for (u32 i = 0; i < size; ++i)
    {
        m_data.m_texCoords[laye].vertex[i] = texCoords[i];
    }
}

void CGeometry::copyIndices(const u32* indices, u32 size)
{
    std::copy(indices, indices + size, m_data.m_indices.vertex.begin());
}

