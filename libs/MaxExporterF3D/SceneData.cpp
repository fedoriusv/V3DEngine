#include "SceneData.h"
#include "stream/StreamManager.h"
#include "utils/Logger.h"

#include "scene/Node.h"
#include "scene/Mesh.h"
#include "scene/Light.h"
#include "scene/Camera.h"

using namespace v3d;
using namespace scene;
using namespace stream;

CSceneData::CSceneData()
{
}

CSceneData::~CSceneData()
{
}

const std::vector<Obj>& CSceneData::getNodesList() const
{
    return m_objectList;
}

void CSceneData::addNode(Obj& node)
{
    m_objectList.push_back(node);
}

CNode* CSceneData::createNode(IGameObject::ObjectTypes type)
{
    switch (type)
    {
    case IGameObject::IGAME_MESH:
        return new CMesh("");

    case IGameObject::IGAME_LIGHT:
        return new CLight();

    case IGameObject::IGAME_CAMERA:
        return new CCamera();

    default:
        return nullptr;
    }

    return nullptr;
}

bool CSceneData::save(const std::string& file, float version)
{
    if (m_objectList.empty())
    {
        LOG_ERROR("CSceneData::save: Object list empty, nothing save");
        return false;
    }
    LOG_INFO("CSceneData::save: Serialize model to memory stream");

    MemoryStreamPtr stream = CStreamManager::createMemoryStream();
    stream->seekBeg(0);
    stream->write(version);

    if (!CSceneData::saveGeometry(stream))
    {
        LOG_ERROR("CSceneData::save: Geometry serialize failed");
        return false;
    }

    if (!CSceneData::saveMaterial(stream))
    {
        LOG_ERROR("CSceneData::save: Material serialize failed");
        return false;
    }

    LOG_INFO("CSceneData::save: Save memory stream to file stream [%s]. Size [%d]", file.c_str(), stream->size());

    FileStreamPtr fileStream = CStreamManager::createFileStream(file, FileStream::e_create);
    fileStream->write(stream->getData(), sizeof(u8), stream->size());
    fileStream->close();

    return true;
}

bool CSceneData::saveGeometry(MemoryStreamPtr& stream)
{
    u32 nodesCount = (u32)m_objectList.size();
    stream->write(nodesCount);
    LOG_INFO("Nodes count %d", nodesCount);

    bool success = false;
    for (auto& iter : m_objectList)
    {
        const CNode* node = iter.second;
        switch (node->getNodeType())
        {
        case eMesh:
            success = CSceneData::serializeMesh(static_cast<const CMesh*>(node), stream);
            break;

        case eCamera:
            success = CSceneData::serializeCamera(static_cast<const CCamera*>(node), stream);
            break;

        case eLight:
            success = CSceneData::serializeLight(static_cast<const CLight*>(node), stream);
            break;

        default:
            LOG_INFO(" CSceneData::saveGeometry: Node type not found %s", CNode::getNodeNameByType(node->getNodeType()));
            success = false;
            break;
        }

        if (!success)
        {
            return false;
        }
    }

    return true;
}

bool CSceneData::saveMaterial(MemoryStreamPtr& stream)
{
    return true;
}

bool CSceneData::serializeMesh(const CMesh* mesh, MemoryStreamPtr& stream)
{
    MemoryStreamPtr subStream = CStreamManager::createMemoryStream();
    subStream->seekBeg(0);

    s32 id = mesh->getID();
    subStream->write(id);
    LOG_INFO("Mesh id %d", id);

    std::string name = mesh->getName();
    subStream->write(name);
    LOG_INFO("Mesh name %s", name.c_str());

    const renderer::GeometryPtr& geomerty = mesh->getGeometry();
    SVertexData& data = geomerty->getData();

    LOG_INFO("Mesh indices size %d", (u32)data._indices.size());
    subStream->write((u32)data._indices.size());
    subStream->write(data._indices.data(), sizeof(u32), (u32)data._indices.size());

    LOG_INFO("Mesh vertices size %d", (u32)data._vertices.size() * 3);
    subStream->write((u32)data._vertices.size());
    subStream->write(data._vertices.data(), sizeof(f32), (u32)data._vertices.size() * 3);

    if (data._normals.size() > 0)
    {
        LOG_INFO("Mesh normals size %d", (u32)data._normals.size() * 3);
        subStream->write((u32)data._normals.size());
        subStream->write(data._normals.data(), sizeof(f32), (u32)data._normals.size() * 3);
    }

    if (data._binormals.size() > 0)
    {
        LOG_INFO("Mesh binormals size %d", (u32)data._binormals.size() * 3);
        subStream->write((u32)data._binormals.size());
        subStream->write(data._binormals.data(), sizeof(f32), (u32)data._binormals.size() * 3);
    }

    if (data._tangents.size() > 0)
    {
        LOG_INFO("Mesh tangents size %d", (u32)data._tangents.size() * 3);
        subStream->write((u32)data._tangents.size());
        subStream->write(data._tangents.data(), sizeof(f32), (u32)data._tangents.size() * 3);
    }

    if (data._colors.size() > 0)
    {
        LOG_INFO("Mesh colors size %d", (u32)data._colors.size() * 3);
        subStream->write((u32)data._colors.size());
        subStream->write(data._colors.data(), sizeof(f32), (u32)data._colors.size() * 3);
    }

    LOG_INFO("Mesh texCoords layer count %d", (u32)data._texCoords.size());
    for (u32 layer = 0; layer < data._texCoords.size(); ++layer)
    {
        if (data._texCoords[layer].size() > 0)
        {
            LOG_INFO("Mesh texCoords[%d] size %d", layer, (u32)data._texCoords[layer].size() * 2);
            subStream->write((u32)data._texCoords[layer].size());
            subStream->write(data._texCoords[layer].data(), sizeof(f32), (u32)data._texCoords[layer].size() * 2);
        }
    }

    LOG_INFO("Mesh stream size %d", subStream->size());
    stream->write(subStream->size());
    stream->write(subStream->getData(), sizeof(u8), subStream->size());

    return true;
}

bool CSceneData::serializeLight(const scene::CLight* node, MemoryStreamPtr& stream)
{
    return true;
}

bool CSceneData::serializeCamera(const scene::CCamera* node, MemoryStreamPtr& stream)
{
    return true;
}