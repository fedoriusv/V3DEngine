#ifndef _V3D_SCENE_DATA_H_
#define _V3D_SCENE_DATA_H_

#include "../source/Object.h"
#include "scene/Model.h"
#include "scene/Light.h"
#include "scene/Camera.h"

#include "IGameObject.h"

namespace v3d
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::pair<IGameObject::ObjectTypes, v3d::ObjectPtr> Obj;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CSceneData
    {
    public:

        CSceneData();
        ~CSceneData();

        const v3d::scene::ModelPtr& getModel() const;
        const std::vector<Obj>&     getObjectList() const;

        void                        addObject(Obj& object);

        void                        setName(const std::string& name);

        v3d::ObjectPtr              createObject(IGameObject::ObjectTypes type);

        bool                        save(const std::string& file);
        bool                        convert();

    private:

        v3d::scene::ModelPtr        m_model;
        std::vector<Obj>            m_objectList;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif //_V3D_MODEL_METADATA_H_
