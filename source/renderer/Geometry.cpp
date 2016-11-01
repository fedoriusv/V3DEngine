#include "Geometry.h"
#include "Buffer.h"
#include "renderer/RenderTechnique.h"

namespace v3d
{
namespace renderer
{

Geometry::Geometry(const CRenderTechnique* technique)
    : m_drawMode(EPrimitivesTopology::eTriangles)
    , m_geometyType(EDataUsageType::eWriteStatic)
    , m_technique(technique)

    , m_vertexBuffer(nullptr)
    , m_indexBuffer(nullptr)

    , m_currentVertexMask(0U)
{
}

Geometry::~Geometry()
{
    m_data.clear();
}

SVertexData& Geometry::getData()
{
    return m_data;
}

EPrimitivesTopology Geometry::getDrawMode() const
{
    return m_drawMode;
}

void Geometry::setDrawMode(EPrimitivesTopology mode)
{
    m_drawMode = mode;
}

void Geometry::addVertex(const core::Vector3D& vertex)
{
    m_data._vertices.push_back(vertex);
}
void Geometry::addNormal(const core::Vector3D& normal)
{
    m_data._normals.push_back(normal);
}

void Geometry::addBinormal(const core::Vector3D& binormal)
{
    m_data._binormals.push_back(binormal);
}

void Geometry::addTangent(const core::Vector3D& tangent)
{
    m_data._tangents.push_back(tangent);
}

void Geometry::addColor(const core::Vector3D& color)
{
    m_data._colors.push_back(color);
}

void Geometry::addTexCoord(u32 layer, const core::Vector2D& texCoord)
{
    if (layer >= m_data._texCoords.size())
    {
        m_data._texCoords.resize(layer + 1);
    }
    m_data._texCoords[layer].push_back(texCoord);
}

void Geometry::addIndex(u32 index)
{
    m_data._indices.push_back(index);
}

void Geometry::setVertexMask(u32 mask)
{
    m_currentVertexMask = mask;
}

void Geometry::draw()
{
    u32 passIndex = m_technique->getCurrentPass();
    const RenderPassPtr pass = m_technique->getRenderPass(passIndex);

    Geometry::setVertexMask(pass->getDefaultShaderData()->getVertexFormatMask());
}

bool Geometry::updated() const
{
    u32 passIndex = m_technique->getCurrentPass();
    const RenderPassPtr pass = m_technique->getRenderPass(passIndex);

    return m_currentVertexMask != pass->getDefaultShaderData()->getVertexFormatMask();
}

} //namespace renderer
} //namespace v3d
