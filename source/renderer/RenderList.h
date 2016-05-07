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

    class Renderable;

    /**
    * Module which controlled render lists.
    * Provides update and render for nodes from sorted lists by target entities.
    */
    class CRenderList
    {
    public:

        explicit CRenderList(const TargetPtr& target);
        ~CRenderList();

        void                            refresh();
        void                            update(u32 delta);
        void                            render();

        void                            setCamera(scene::CCamera* camera);
        scene::CCamera*                 getCamera() const;

        bool                            isEnable() const;
        void                            setEnable(bool enable);

        void                            add(scene::CNode* node, Renderable* draw, u32 target, u32 pass);
        void                            clear();

        bool                            contain(scene::CNode* node);

        const TargetPtr                 getTarget() const;
        const std::string&              getTargetName() const;

    private:

        struct SNodeList
        {
            SNodeList(scene::CNode* node, Renderable* draw, u32 target, u32 pass);

            scene::CNode*   _node;
            Renderable*     _draw;
            u32             _targetIndex;
            u32             _passIndex;
        };

        bool                            checkDistance(const scene::CNode* node, const f32 distance);

        bool                            m_enable;

        TargetPtr                       m_target;

        std::vector<SNodeList>          m_list;

        std::vector<SNodeList>          m_draw;
        std::vector<SNodeList>          m_drawStatic;
        std::vector<SNodeList>          m_drawAlpha;
        std::vector<SNodeList>          m_drawTransparency;

        scene::CCamera*                 m_camera;

        const f32                       k_maxPriority = 1000000.0f;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_V3D_RENDER_LIST_H_
