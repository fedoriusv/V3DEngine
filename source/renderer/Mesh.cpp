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

    if (stream->size() > 0)
    {

        stream->seekBeg(0);

        stream->read(m_id);
        stream->read(m_name);

        //TODO:

        return true;
    }

    return false;
}

void CMesh::update(f64 time)
{

}

void CMesh::render()
{

}