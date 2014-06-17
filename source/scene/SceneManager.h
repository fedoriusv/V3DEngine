#ifndef _V3D_SCENE_MANAGER_H_
#define _V3D_SCENE_MANAGER_H_

#include "scene/Node.h"
#include "renderer/Renderer.h"

namespace v3d
{
namespace scene
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CCamera;

    class CSceneManager
    {
    public:

        CSceneManager();
        ~CSceneManager();

        void                    init();

        void                    draw();

        void                    clear();
        bool                    drop(CNode* node);

        void                    addNode(CNode* node);

        CNode*                  addSample(CNode* parent = nullptr, const core::Vector3D& pos = core::Vector3D(0.0f));
        CNode*                  addCube(CNode* parent = nullptr, const core::Vector3D& pos = core::Vector3D(0.0f), const float size = 1.0f);

        CNode*                  addCamera(CNode* parent = nullptr, const core::Vector3D& pos = core::Vector3D(0.0f), const core::Vector3D& target = core::Vector3D(0.0f, 0.0f, -1.0f), 
                                    const core::Vector3D& up = core::Vector3D(0.0f, 1.0f, 0.0f));

        CNode*                  addFPSCamera(CNode* parent = nullptr, const core::Vector3D& pos = core::Vector3D(0.0f), const core::Vector3D& target = core::Vector3D(0.0f, 0.0f, -1.0f),
                                    const core::Vector3D& up = core::Vector3D(0.0f, 1.0f, 0.0f));

        CNode*                  getObjectByID(const s32 id);
        CNode*                  getObjectByName(const std::string& name);

        void                    setBackgroundColor(const core::Vector3D& color);

        const core::Vector3D&   getBackgroundColor() const;

    private:

        double                  m_currentTime;
        double                  m_deltaTime;
        double                  m_lastTime;

        void                    updateDeltaTime();
        void                    update(f64 time);

        std::vector<CNode*>     m_objects;
        CCamera*                m_camera;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CSceneManager>	SceneManagerPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_SCENE_MANAGER_H_
