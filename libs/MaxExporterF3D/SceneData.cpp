#include "SceneData.h"
#include "stream/StreamManager.h"
#include "utils/Logger.h"

using namespace v3d;
using namespace stream;

CSceneData::CSceneData()
{
    m_model = std::make_shared<scene::CModel>();
}

CSceneData::~CSceneData()
{
}

const v3d::scene::ModelPtr& CSceneData::getModel() const
{
    return m_model;
}

const std::vector<Obj>& CSceneData::getObjectList() const
{
    return m_objectList;
}

void CSceneData::setName(const std::string& name)
{
    m_model->setName(name);
}

void CSceneData::addObject(Obj& object)
{
    m_objectList.push_back(object);
}

v3d::ObjectPtr CSceneData::createObject(IGameObject::ObjectTypes type)
{
    switch (type)
    {
    case IGameObject::IGAME_MESH:
        return std::make_shared<renderer::CMesh>();

    case IGameObject::IGAME_LIGHT:
        return std::make_shared<scene::CLight>();

    case IGameObject::IGAME_CAMERA:
        return std::make_shared<scene::CCamera>();

    default:
        return nullptr;
    }

    return nullptr;
}

bool CSceneData::convert()
{
    for (std::vector<Obj>::const_iterator i = m_objectList.begin(); i < m_objectList.end(); ++i)
    {
        const Obj& obj = (*i);
        if (obj.first == IGameObject::IGAME_MESH)
        {
            m_model->addMesh(std::static_pointer_cast<renderer::CMesh>(obj.second));
        }
    }

    return true;
}

bool CSceneData::save(const std::string& file)
{
    if (!m_model)
    {
        return false;
    }
    
    if (!CSceneData::convert())
    {
        return false;
    }

    LOG_INFO("CSceneData::save: Serialize model to memory stream");

    MemoryStreamPtr stream = CStreamManager::createMemoryStream();
    stream->seekBeg(0);

    s32 id = m_model->getID();
    stream->write(id);
    LOG_INFO("Model id %d", id);

    std::string name = m_model->getName();
    stream->write(name);
    LOG_INFO("Model name %s", name.c_str());

    u32 meshes = m_model->getMeshCount();
    stream->write(meshes);
    LOG_INFO("Meshes count %d", meshes);

    for (u32 i = 0; i < meshes; ++i)
    {
        const renderer::MeshPtr& mesh = m_model->getMesh(i);

        MemoryStreamPtr subStream = CStreamManager::createMemoryStream();
        subStream->seekBeg(0);

        s32 id = mesh->getID();
        subStream->write(id);
        LOG_INFO("Mesh id %d", id);

        std::string name = mesh->getName();
        subStream->write(name);
        LOG_INFO("Mesh name %s", name.c_str());

        //TODO : data

        stream->write(subStream->size());
        LOG_INFO("Mesh stream size %d", subStream->size());

        stream->write(subStream->getData(), sizeof(u8), subStream->size());
    }
    
    LOG_INFO("CSceneData::save: Save memory stream to file stream [%s]. Size [%d]", file.c_str(), stream->size());

    FileStreamPtr fileStream = CStreamManager::createFileStream(file, FileStream::e_create);
    fileStream->write(stream->getData(), sizeof(u8), stream->size());
    fileStream->close();

    return true;
}