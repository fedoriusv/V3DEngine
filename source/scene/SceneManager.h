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

        CNode*                  addSample(CNode* parent = nullptr, const Vector3D& pos = Vector3D(0.0f));
        CNode*                  addCube(CNode* parent = nullptr, const Vector3D& pos = Vector3D(0.0f), const float size = 1.0f);
        CNode*                  addSphere(CNode* parent = nullptr, const Vector3D& pos = Vector3D(0.0f, 0.0f, 0.0f), const float radius = 1.0f);
        CNode*                  addCylinder(CNode* parent = nullptr, const Vector3D& pos = Vector3D(0.0f, 0.0f, 0.0f), const float radius = 0.5f, const float height = 1.0f);
        CNode*                  addTorus(CNode* parent = nullptr, const Vector3D& pos = Vector3D(0.0f, 0.0f, 0.0f), const float minorRadius = 0.5f, const float majorRadius = 1.0f);
        CNode*                  addDisk(CNode* parent = nullptr, const Vector3D& pos = Vector3D(0.0f, 0.0f, 0.0f), const float minorRadius = 1.0f, const float majorRadius = 2.0f);

        CNode*                  addCamera(CNode* parent = nullptr, const Vector3D& pos = Vector3D(0.0f), const Vector3D& target = Vector3D(0.0f, 0.0f, -1.0f), const Vector3D& up = Vector3D(0.0f, 1.0f, 0.0f));
        CNode*                  addFPSCamera(CNode* parent = nullptr, const Vector3D& pos = Vector3D(0.0f), const Vector3D& target = Vector3D(0.0f, 0.0f, -1.0f), const f32 speed = 0.001f);

        CNode*                  getObjectByID(const s32 id);
        CNode*                  getObjectByName(const std::string& name);

        void                    setBackgroundColor(const Vector3D& color);

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
