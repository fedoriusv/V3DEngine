#ifndef _V3D_SCENE_DATA_H_
#define _V3D_SCENE_DATA_H_

#include "IGameObject.h"

#ifdef max
#   undef max
#endif //max

#ifdef min
#   undef min
#endif //min

#include "renderer/Material.h"
#include "stream/MemoryStream.h"

namespace v3d
{
namespace scene
{
    class CNode;
    class CMesh;
    class CLight;
    class CCamera;
}
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::pair<IGameObject::ObjectTypes, scene::CNode*> Obj;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CSceneData
    {
    public:

        CSceneData();
        ~CSceneData();

        const std::vector<Obj>&             getNodesList() const;
        void                                addNode(const Obj& node);

        const std::vector<renderer::MaterialPtr>& getMaterialList() const;
        void                                addMaterial(const renderer::MaterialPtr& material);

        void                                setName(const std::string& name);
        void                                setId(s32 id);

        scene::CNode*                       createNode(IGameObject::ObjectTypes type);

        bool                                save(const std::string& file, s32 version);

    private:

        bool                                saveGeometry(stream::MemoryStreamPtr& stream);
        bool                                saveMaterial(stream::MemoryStreamPtr& stream);

        bool                                serializeMesh(const scene::CMesh* node, stream::MemoryStreamPtr& stream);
        bool                                serializeLight(const scene::CLight* node, stream::MemoryStreamPtr& stream);
        bool                                serializeCamera(const scene::CCamera* node, stream::MemoryStreamPtr& stream);

        std::string                         m_name;
        s32                                 m_id;

        std::vector<Obj>                    m_objectList;
        std::vector<renderer::MaterialPtr>  m_materialList;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif //_V3D_MODEL_METADATA_H_
