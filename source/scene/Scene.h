#ifndef _V3D_SCENE_H_
#define _V3D_SCENE_H_

#include "common.h"
#include "renderer/RenderList.h"

namespace v3d
{
namespace scene
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CNode;
    class CCamera;

    class CScene
    {
    public:

        CScene();
        ~CScene();

        void                                init();

        void                                draw(u32 delta);

        void                                add(CNode* node);
        bool                                drop(CNode* node);
        void                                clear();

        void                                setActiveCamera(CCamera* camera);
        CCamera*                            getActiveCamera() const;
        bool                                isActiveCamera(const CCamera* camera);

        CNode*                              getNodeByID(s32 id);
        CNode*                              getNodeByName(const std::string& name);

    private:

        void                                initRenderLists();
        void                                attachToRenderList(CNode* node);
        void                                updateRenderLists(u32 delta);

        void                                needRefresh();

        std::vector<CNode*>                 m_objects;
        std::vector<renderer::CRenderList>  m_renderList;

        CCamera*                            m_camera;
        bool                                m_refresh;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CScene> ScenePtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_SCENE_H_
