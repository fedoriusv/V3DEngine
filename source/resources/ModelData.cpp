#include "ModelData.h"
#include "utils/Logger.h"
#include "stream/StreamManager.h"

using namespace v3d;
using namespace core;
using namespace resources;
using namespace renderer;

CModelData::CModelData()
    : m_countMeshes(0U)
    , m_id(-1)
    , m_name("")
{
}

CModelData::~CModelData()
{
}

void CModelData::init(const stream::IStreamPtr& stream)
{
    CResource::setStream(stream);
}

u32 CModelData::getCountMeshes() const
{
    return m_countMeshes;
}

const std::string& CModelData::getName() const
{
    return m_name;
}

s32 CModelData::getID() const
{
    return m_id;
}

const SVertexData& CModelData::getVertices() const
{
    return m_vertices;
}

bool CModelData::load()
{
    const stream::IStreamPtr& stream = CResource::getStream();
    if (!stream)
    {
        LOG_ERROR("CModelData::load: Empty Stream with name [%s]", CResource::getResourseName().c_str());
        return false;
    }

    if (stream->size() > 0)
    {

        stream->seekBeg(0);

        stream->read(m_id);
        stream->read(m_name);

        //stream->read(m_countMeshes);

        CModelData::loadGeometry(stream);
        CModelData::loadMaterial(stream);

        return true;
    }

    return false;
}

void CModelData::loadGeometry(const stream::IStreamPtr& stream)
{
    u32 countVertices = 0;
    stream->read(countVertices);
    if (countVertices > 0)
    {
        m_vertices._vertices.resize(countVertices);
        stream->read(&m_vertices._vertices[0], sizeof(Vector3D), countVertices);
    }

    u32 countNormals = 0;
    stream->read(countNormals);
    if (countNormals > 0)
    {
        m_vertices._normals.resize(countNormals);
        stream->read(&m_vertices._normals[0], sizeof(Vector3D), countNormals);
    }

    u32 countBinormals = 0;
    stream->read(countBinormals);
    if (countBinormals > 0)
    {
        m_vertices._binormals.resize(countBinormals);
        stream->read(&m_vertices._binormals[0], sizeof(Vector3D), countBinormals);
    }

    u32 countTangents = 0;
    stream->read(countTangents);
    if (countTangents > 0)
    {
        m_vertices._tangents.resize(countTangents);
        stream->read(&m_vertices._tangents[0], sizeof(Vector3D), countTangents);
    }

    u32 countTexCoordLayers = 0;
    stream->read(countTexCoordLayers);
    m_vertices._texCoords.resize(countTexCoordLayers);
    for (u32 layer = 0; layer < countTexCoordLayers; ++layer)
    {
        u32 countTexCoords = 0;
        stream->read(countTexCoords);
        if (countTexCoords > 0)
        {
            m_vertices._texCoords[layer].resize(countTexCoords);
            stream->read(&m_vertices._texCoords[layer][0], sizeof(Vector2D), countTexCoords);
        }
    }

    u32 countIndices = 0;
    stream->read(countIndices);
    if (countIndices > 0)
    {
        m_vertices._indices.resize(countIndices);
        stream->read(&m_vertices._indices[0], sizeof(u32), countIndices);
    }
}

void CModelData::loadMaterial(const stream::IStreamPtr& stream)
{

}

stream::ResourcePtr CModelData::readMeshResource(const renderer::RenderTechniquePtr& technique) const
{
    const stream::IStreamPtr& stream = CResource::getStream();
    if (!stream)
    {
        LOG_ERROR("CModelData::getMeshResource: Empty Stream with name [%s]", CResource::getResourseName().c_str());
        return nullptr;
    }

    if (stream->size() > 0)
    {
        u32 size = 0;
        stream->read(size);
        u8* data = new u8[size];
        stream->read(data, sizeof(u8), size);

        //MeshPtr mesh = std::make_shared<CMesh>(technique);

        //stream::IStreamPtr mem = stream::CStreamManager::createMemoryStream(data, size);
        //mesh->init(mem);

        //return mesh;
    }

    return nullptr;
}