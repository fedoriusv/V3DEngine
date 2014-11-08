#include "SceneData.h"

using namespace v3d;

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