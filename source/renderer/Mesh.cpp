#include "Mesh.h"

using namespace v3d;
using namespace v3d::renderer;

CMesh::CMesh()
{
}

CMesh::~CMesh()
{
}

void CMesh::setMaterial(const MaterialPtr& material)
{
    m_material = material;
}

const MaterialPtr& CMesh::getMaterial() const
{
    return m_material;
}

const GeometryPtr& CMesh::getGeomerty() const
{
    return m_geometry;
}

void CMesh::update(f64 time)
{

}

void CMesh::render()
{

}