#include "ModelMetadata.h"

using namespace v3d;

CModelMetadata::CModelMetadata()
{
    m_model = std::make_shared<scene::CModel>();
}

CModelMetadata::~CModelMetadata()
{
}

const v3d::scene::ModelPtr& CModelMetadata::getModel() const
{
    return m_model;
}

const std::vector<Obj>& CModelMetadata::getObjectList() const
{
    return m_objectList;
}

void CModelMetadata::addObject(Obj& object)
{
    m_objectList.push_back(object);
}

v3d::ObjectPtr CModelMetadata::createObject(IGameObject::ObjectTypes type)
{
    switch (type)
    {
    case IGameObject::IGAME_MESH:
        return std::make_shared<renderer::CMesh>();

    /*case IGameObject::IGAME_LIGHT:
        return std::make_shared<scene::CLight>();*/

   /* case IGameObject::IGAME_CAMERA:
        return std::make_shared<scene::CCamera>();*/

    default:
        return nullptr;
    }

    return nullptr;
}