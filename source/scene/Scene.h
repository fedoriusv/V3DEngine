#ifndef _V3D_SCENE_H_
#define _V3D_SCENE_H_

#include "common.h"
#include "renderer/RenderTarget.h"

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

        void                        init();

        void                        update(f64 time);
        void                        renderer();

        void                        clear();

        bool                        drop(CNode* node);
        void                        add(CNode* node);

        void                        setActiveCamera(CCamera* camera);
        CCamera*                    getActiveCamera() const;
        bool                        isActiveCamera(const CCamera* camera);

        CNode*                      getNodeByID(s32 id);
        CNode*                      getNodeByName(const std::string& name);

    private:

        void                        updateNodes(u32 delta);
        bool                        checkDistance(const CNode* node, const f32 distance);

        struct SFramebuffer
        {
            bool                        _active;
            renderer::RenderTargetPtr   _target;
            std::vector<CNode*>         _list;
            CCamera*                    _camera;
        };

        std::vector<CNode*>         m_objects;
        std::vector<CNode*>         m_drawObjects;

        std::vector<SFramebuffer>   m_renderList;

        CCamera*                    m_camera;

        const f32                   k_maxPriority = 1000000.0f;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CScene> ScenePtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_SCENE_H_
