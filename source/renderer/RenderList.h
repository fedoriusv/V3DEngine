#ifndef _V3D_RENDER_LIST_H_
#define _V3D_RENDER_LIST_H_

#include "common.h"
#include "RenderTarget.h"

namespace v3d
{
namespace scene
{
    class CNode;
    class CCamera;
}
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CRenderList
    {
    public:

        explicit CRenderList(const RenderTargetPtr& target);
        ~CRenderList();

        void                            refresh();
        void                            update(u32 delta);
        void                            render();

        void                            setCamera(scene::CCamera* camera);
        scene::CCamera*                 getCamera() const;

        bool                            isEnable() const;
        void                            setEnable(bool enable);

        void                            add(scene::CNode* node, u32 target);
        void                            clear();

        const RenderTargetPtr&          getRenderTarget() const;
        const std::string&              getTargetName() const;

    private:

        struct SNodeList
        {
            SNodeList(scene::CNode* node, u32 index)
            : _node(node)
            , _targetIndex(index)
            {}

            scene::CNode* _node;
            u32           _targetIndex;
        };

        bool                            checkDistance(const scene::CNode* node, const f32 distance);

        bool                            m_enable;

        RenderTargetPtr                 m_target;

        std::vector<SNodeList>          m_list;
        std::vector<SNodeList>          m_draw;

        scene::CCamera*                 m_camera;

        const f32                       k_maxPriority = 1000000.0f;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_RENDER_LIST_H_