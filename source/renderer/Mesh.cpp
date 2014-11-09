#include "Mesh.h"
#include "utils/Logger.h"

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

void CMesh::init(const stream::IStreamPtr& stream)
{
    CResource::setStream(stream);
}

bool CMesh::load()
{
    const stream::IStreamPtr& stream = CResource::getStream();
    if (!stream)
    {
        LOG_ERROR("CMesh::load: Empty Stream with name [%s]", CResource::getResourseName().c_str());
        return false;
    }

    //TODO:
}

void CMesh::update(f64 time)
{

}

void CMesh::render()
{

}