#include "Mesh.h"
#include "utils/Logger.h"
#include "Engine.h"

using namespace v3d;
using namespace v3d::renderer;

CMesh::CMesh(const RenderTechniquePtr& technique)
: m_initialiazed(false)
, m_material(nullptr)
, m_geometry(nullptr)
, m_name("")
, m_id(-1)
#ifdef _DEBUG
//, m_debug(nullptr)
#endif
{
    m_material = std::make_shared<CMaterial>();

    if (technique)
    {
        m_material->setRenderTechnique(technique);
    }
}

CMesh::~CMesh()
{
    m_geometry->free();
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

const std::string& CMesh::getName() const
{
    return m_name;
}

s32 CMesh::getID() const
{
    return m_id;
}

void CMesh::init(const stream::IStreamPtr& stream)
{
    CResource::setStream(stream);

    RenderTechniquePtr technique = m_material->getRenderTechique();
    if (!technique)
    {
        LOG_ERROR("CMesh::init: Do not exist RenderTechique");
        ASSERT(false && "CMesh: Do not exist RenderTechique");
        return;
    }

    m_geometry = RENDERER->makeSharedGeometry(technique);
}

bool CMesh::load()
{
    const stream::IStreamPtr& stream = CResource::getStream();
    if (!stream)
    {
        LOG_ERROR("CMesh: Empty Stream with name [%s]", CResource::getResourseName().c_str());
        return false;
    }

    if (stream->size() > 0)
    {

        stream->seekBeg(0);

        stream->read(m_id);
        stream->read(m_name);

        SVertexData& data = m_geometry->getData();

        u32 indices = 0;
        stream->read(indices);
        data._indices.resize(indices);
        stream->read(&data._indices[0], sizeof(u32), indices);

        u32 vertices = 0;
        stream->read(vertices);
        data._vertices.resize(vertices);
        stream->read(&data._vertices[0], sizeof(f32), vertices * 3);

        u32 normals = 0;
        stream->read(normals);
        data._normals.resize(normals);
        stream->read(&data._normals[0], sizeof(f32), normals * 3);

        u32 texCoord = 0;
        stream->read(texCoord);
        data._texCoords.resize(1);
        data._texCoords[0].resize(texCoord);
        stream->read(&data._texCoords[0][0], sizeof(f32), texCoord * 2);
        
        return true;
    }

    return false;
}

void CMesh::init()
{
    m_geometry->setDrawMode(CGeometry::eTriangles);

    m_geometry->init();
#ifdef _DEBUG
//    m_debug->init();
#endif

    m_initialiazed = true;
}

void CMesh::refresh()
{
    if (!m_initialiazed)
    {
        return;
    }

    m_geometry->refresh();
#ifdef _DEBUG
//    m_debug->refresh();
#endif
}

void CMesh::update(f64 time)
{
    /*if (!m_visible || !m_initialiazed)
    {
        return;
    }

    m_renderJob->setTransform(CNode::getAbsTransform());*/
}

void CMesh::render()
{
//    if (!m_visible || !m_initialiazed)
//    {
//        return;
//    }
//
//    RENDERER->draw(m_renderJob);
//    //m_needUpdate = false;
//
//#ifdef _DEBUG
//    if (RENDERER->isDebugMode())
//    {
//        m_debug->draw();
//    }
//#endif
}