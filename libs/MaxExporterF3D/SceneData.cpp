#include "SceneData.h"
#include "stream/StreamManager.h"
#include "scene/TextureManager.h"
#include "utils/Logger.h"

#include "scene/Node.h"
#include "scene/Mesh.h"
#include "scene/Light.h"
#include "scene/Camera.h"
#include "resources/Image.h"

using namespace v3d;
using namespace scene;
using namespace stream;
using namespace renderer;

CSceneData::CSceneData()
{
}

CSceneData::~CSceneData()
{
    for (Obj& iter : m_objectList)
    {
        delete iter.second;
        iter.second = nullptr;

    }
    m_objectList.clear();
    m_materialList.clear();

    CTextureManager::freeInstance();
}

const std::vector<Obj>& CSceneData::getNodesList() const
{
    return m_objectList;
}

void CSceneData::addNode(const Obj& node)
{
    m_objectList.push_back(node);
}

void CSceneData::addMaterial(const MaterialPtr& material)
{
    m_materialList.push_back(material);
}

void v3d::CSceneData::addTexture(const renderer::MaterialPtr& material, const resources::CImage* image)
{
    if (material && image)
    {
        m_textureList.insert(decltype(m_textureList)::value_type(material, image));
    }
}

const std::vector<MaterialPtr>& CSceneData::getMaterialList() const
{
    return m_materialList;
}

void CSceneData::setName(const std::string& name)
{
    m_name = name;
}

void CSceneData::setId(s32 id)
{
    m_id = id;
}

CNode* CSceneData::createNode(IGameObject::ObjectTypes type)
{
    switch (type)
    {
    case IGameObject::IGAME_MESH:
        return new CMesh();

    case IGameObject::IGAME_LIGHT:
        return new CLight();

    case IGameObject::IGAME_CAMERA:
        return new CCamera();

    default:
        return nullptr;
    }

    return nullptr;
}

bool CSceneData::save(const std::string& file, f32 version)
{
    if (m_objectList.empty())
    {
        LOG_ERROR("CSceneData::save: Object list empty, nothing save");
        return false;
    }
    LOG_DEBUG("CSceneData::save: Serialize model to memory stream");

    MemoryStreamPtr stream = CStreamManager::createMemoryStream();
    stream->seekBeg(0);
    stream->write(version);

    stream->write(m_id);
    stream->write(m_name);

    if (!CSceneData::saveMaterial(stream))
    {
        LOG_ERROR("CSceneData::save: Material serialize failed");
        return false;
    }

    if (!CSceneData::saveGeometry(stream))
    {
        LOG_ERROR("CSceneData::save: Geometry serialize failed");
        return false;
    }

    LOG_DEBUG("CSceneData::save: Save memory stream to file stream [%s]. Size [%d]", file.c_str(), stream->size());

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
    u32 materialsCount = (u32)m_materialList.size();
    stream->write(materialsCount);
    LOG_INFO("Materials count %d\n", materialsCount);

    bool success = false;
    for (auto& material : m_materialList)
    {
        MemoryStreamPtr subStream = CStreamManager::createMemoryStream();
        subStream->seekBeg(0);

        std::string name = material->getResourseName();
        subStream->write(name);
        LOG_INFO("Material name %s", name.c_str());

        //Texture

        decltype(m_textureList) textures;
        std::copy_if(m_textureList.cbegin(), m_textureList.cend(), std::inserter(textures, textures.begin()), [&material](auto& item) -> bool
        {
            return item.first == material;
        });

        u32 countTextures = (u32)textures.size();
        subStream->write(countTextures);
        LOG_INFO("Num Texutes %d", countTextures);

        for (auto& item : textures)
        {
            const std::string& name = item.second->getResourseName();
            LOG_INFO("Texute name %d", name.c_str());
            subStream->write(name);
        }
        /*for (u32 i = 0; i < countTextures; ++i)
        {
            const TexturePtr texure = material->getTexture(i);
            subStream->write(texure->getResourseName());
        }*/

        //Colors
        core::Vector4D diffuse = material->getDiffuseColor();
        subStream->write(&diffuse.x, sizeof(core::Vector4D), 1);

        core::Vector4D ambient = material->getAmbientColor();
        subStream->write(&ambient.x, sizeof(core::Vector4D), 1);

        core::Vector4D specular = material->getSpecularColor();
        subStream->write(&specular.x, sizeof(core::Vector4D), 1);

        core::Vector4D emission = material->getEmissionColor();
        subStream->write(&emission.x, sizeof(core::Vector4D), 1);

        subStream->write(material->getTransparency());
        subStream->write(material->getShininess());
        subStream->write(material->getGlossiness());

        LOG_DEBUG("Material stream size %d\n", subStream->size());
        stream->write(subStream->size());
        stream->write(subStream->getData(), sizeof(u8), subStream->size());
    }

    return true;
}

bool CSceneData::serializeMesh(const CMesh* mesh, MemoryStreamPtr& stream)
{
    stream->write((s32)mesh->getNodeType());
    s32 parent = mesh->getParent() ? mesh->getParent()->getID() : -1;
    stream->write(parent);

    const std::string& material = mesh->getMaterial()->getResourseName();
    stream->write(material);

    MemoryStreamPtr subStream = CStreamManager::createMemoryStream();
    subStream->seekBeg(0);

    s32 id = mesh->getID();
    subStream->write(id);
    LOG_INFO("Mesh id %d", id);

    std::string name = mesh->getName();
    subStream->write(name);
    LOG_INFO("Mesh name %s", name.c_str());
    LOG_INFO("Mesh material %s", material.c_str());

    const renderer::GeometryPtr& geomerty = mesh->getGeometry();
    SVertexData& data = geomerty->getData();

    subStream->write((u32)data._indices.size());
    if (!data._indices.empty())
    {
        LOG_INFO("Mesh indices size %d", (u32)data._indices.size());
        subStream->write(data._indices.data(), sizeof(u32), (u32)data._indices.size());
    }

    LOG_INFO("Mesh vertices size %d", (u32)data._vertices.size() * 3);
    subStream->write((u32)data._vertices.size());
    subStream->write(data._vertices.data(), sizeof(f32), (u32)data._vertices.size() * 3);

    subStream->write((u32)data._normals.size());
    if (!data._normals.empty())
    {
        LOG_INFO("Mesh normals size %d", (u32)data._normals.size() * 3);
        subStream->write(data._normals.data(), sizeof(f32), (u32)data._normals.size() * 3);
    }

    subStream->write((u32)data._binormals.size());
    if (!data._binormals.empty())
    {
        LOG_INFO("Mesh binormals size %d", (u32)data._binormals.size() * 3);
        subStream->write(data._binormals.data(), sizeof(f32), (u32)data._binormals.size() * 3);
    }

    subStream->write((u32)data._tangents.size());
    if (!data._tangents.empty())
    {
        LOG_INFO("Mesh tangents size %d", (u32)data._tangents.size() * 3);
        subStream->write(data._tangents.data(), sizeof(f32), (u32)data._tangents.size() * 3);
    }

    subStream->write((u32)data._colors.size());
    if (!data._colors.empty())
    {
        LOG_INFO("Mesh colors size %d", (u32)data._colors.size() * 3);
        subStream->write(data._colors.data(), sizeof(f32), (u32)data._colors.size() * 3);
    }


    subStream->write((u32)data._texCoords.size());
    if (!data._texCoords.empty())
    {
        LOG_INFO("Mesh texCoords layer count %d", (u32)data._texCoords.size());
        for (u32 layer = 0; layer < data._texCoords.size(); ++layer)
        {
            if (!data._texCoords[layer].empty())
            {
                LOG_INFO("Mesh texCoords[%d] size %d", layer, (u32)data._texCoords[layer].size() * 2);
                subStream->write((u32)data._texCoords[layer].size());
                subStream->write(data._texCoords[layer].data(), sizeof(f32), (u32)data._texCoords[layer].size() * 2);
            }
        }
    }

    LOG_DEBUG("Mesh stream size %d\n", subStream->size());
    stream->write(subStream->size());
    stream->write(subStream->getData(), sizeof(u8), subStream->size());

    return true;
}

bool CSceneData::serializeLight(const scene::CLight* light, MemoryStreamPtr& stream)
{
    stream->write((s32)light->getNodeType());
    s32 parent = light->getParent() ? light->getParent()->getID() : -1;
    stream->write(parent);

    const std::string material = "";
    stream->write(material);

    return true;
}

bool CSceneData::serializeCamera(const scene::CCamera* camera, MemoryStreamPtr& stream)
{
    stream->write((s32)camera->getNodeType());
    s32 parent = camera->getParent() ? camera->getParent()->getID() : -1;
    stream->write(parent);

    const std::string material = "";
    stream->write(material);

    return true;
}