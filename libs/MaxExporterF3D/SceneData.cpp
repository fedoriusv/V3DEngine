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

bool CSceneData::save(const std::string& file)
{
    if (!m_model)
    {
        return false;
    }

    LOG_INFO("CSceneData::save: Serialize model to memory stream");

    MemoryStreamPtr stream = CStreamManager::createMemoryStream();
    stream->seekBeg(0);

    s32 id = m_model->getID();
    stream->write(id);

    std::string name = m_model->getName();
    stream->write(name);

    u32 meshes = m_model->getMeshCount();
    stream->write(meshes);

    for (u32 i = 0; i < meshes; ++i)
    {
        //TODO:
    }
    
    LOG_INFO("CSceneData::save: Save memory stream to file stream [%s]", file.c_str());

    FileStreamPtr fileStream = CStreamManager::createFileStream(file, FileStream::e_out);
    fileStream->write(stream->getData(), stream->size());
    fileStream->close();

    return true;
}