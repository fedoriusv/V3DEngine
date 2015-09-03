#include "Mesh.h"
#include "utils/Logger.h"
#include "Engine.h"

using namespace v3d;
using namespace scene;
using namespace renderer;

CMesh::CMesh()
{
    m_nodeType = ENodeType::eMesh;
    LOG_INFO("CMesh: Create node type: %s", getNodeNameByType(m_nodeType).c_str());
}

CMesh::~CMesh()
{
}

void CMesh::render()
{
    if (!CNode::isVisible() || !m_initialiazed)
    {
        return;
    }

    CRenderable::render();
}

void CMesh::update(s32 dt)
{
    if (!CNode::isVisible() || !m_initialiazed)
    {
        return;
    }

    CNode::update(dt);
    CRenderable::getRenderJob()->setTransform(CNode::getAbsTransform());
}

void CMesh::init()
{
    if (m_initialiazed)
    {
        return;
    }

    const CRenderTechnique* technique = CRenderable::getMaterial()->getRenderTechique();
    if (!technique)
    {
        LOG_ERROR("CMesh: Do not exist RenderTechique");
        ASSERT(false && "CMesh: Do not exist RenderTechique");
        return;
    }

    CRenderable::setGeometry(RENDERER->makeSharedGeometry(technique));
    CRenderable::setRenderJob(std::make_shared<CRenderJob>(CRenderable::getMaterial(), CRenderable::getGeometry(), CNode::getAbsTransform()));

    CRenderable::getGeometry()->setDrawMode(CGeometry::eTriangles);

    if (!CMesh::load())
    {
        LOG_ERROR("CMesh: Can't load mesh stream");
        ASSERT(false && "CMesh: Can't load mesh stream");
        return;
    }

    CRenderable::getGeometry()->init();
    m_initialiazed = true;
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
        LOG_ERROR("CMesh: Empty Stream with name [%s] form Texture", CResource::getResourseName().c_str());
        return false;
    }

    CMesh::loadGeometry(stream);
    CMesh::loadMaterial(stream);
}


void CMesh::loadGeometry(const stream::IStreamPtr& stream)
{
    SVertexData& data = CRenderable::getGeometry()->getData();

    u32 countVertices = 0;
    stream->read(countVertices);
    if (countVertices > 0)
    {
        data._vertices.resize(countVertices);
        stream->read(&data._vertices[0], sizeof(Vector3D), countVertices);
    }

    u32 countNormals = 0;
    stream->read(countNormals);
    if (countNormals > 0)
    {
        data._normals.resize(countNormals);
        stream->read(&data._normals[0], sizeof(Vector3D), countNormals);
    }

    u32 countBinormals = 0;
    stream->read(countBinormals);
    if (countBinormals > 0)
    {
        data._binormals.resize(countBinormals);
        stream->read(&data._binormals[0], sizeof(Vector3D), countBinormals);
    }

    u32 countTangents = 0;
    stream->read(countTangents);
    if (countTangents > 0)
    {
        data._tangents.resize(countTangents);
        stream->read(&data._tangents[0], sizeof(Vector3D), countTangents);
    }

    u32 countTexCoordLayers = 0;
    stream->read(countTexCoordLayers);
    data._texCoords.resize(countTexCoordLayers);
    for (u32 layer = 0; layer < countTexCoordLayers; ++layer)
    {
        u32 countTexCoords = 0;
        stream->read(countTexCoords);
        if (countTexCoords > 0)
        {
            data._texCoords[layer].resize(countTexCoords);
            stream->read(&data._texCoords[layer][0], sizeof(Vector2D), countTexCoords);
        }
    }

    u32 countIndices = 0;
    stream->read(countIndices);
    if (countIndices > 0)
    {
        data._indices.resize(countIndices);
        stream->read(&data._indices[0], sizeof(u32), countIndices);
    }
}

void CMesh::loadMaterial(const stream::IStreamPtr& stream)
{
    //TODO:
}
