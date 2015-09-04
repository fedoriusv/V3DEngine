#ifndef _V3D_SCENE_MANAGER_H_
#define _V3D_SCENE_MANAGER_H_

#include "Scene.h"

namespace v3d
{
namespace scene
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CNode;

    class CCamera;
    class CFPSCamera;

    class CSampleShape;
    class CCubeShape;
    class CSphereShape;
    class CCylinderShape;
    class CTorusShape;
    class CDiskShape;
    class CPlaneShape;
    class CRectangleShape;

    class CModel;
    class CSkybox;
    class CText;
    class CBillboard;
    class CLight;

    class CSceneManager
    {
    public:

        CSceneManager();
        ~CSceneManager();

        void                    init();
        void                    draw();
        void                    clear();

        bool                    dropNode(CNode* node);
        void                    addNode(CNode* node);

        CSampleShape*           addSample(CNode* parent = nullptr, const core::Vector3D& pos = core::Vector3D(0.f));
        CCubeShape*             addCube(CNode* parent = nullptr, const core::Vector3D& pos = core::Vector3D(0.f), f32 size = 1.f);
        CSphereShape*           addSphere(CNode* parent = nullptr, const core::Vector3D& pos = core::Vector3D(0.f), f32 radius = 1.f);
        CCylinderShape*         addCylinder(CNode* parent = nullptr, const core::Vector3D& pos = core::Vector3D(0.f), f32 radius = 0.5f, f32 height = 1.f);
        CTorusShape*            addTorus(CNode* parent = nullptr, const core::Vector3D& pos = core::Vector3D(0.f), f32 minorRadius = 0.5f, f32 majorRadius = 1.f);
        CDiskShape*             addDisk(CNode* parent = nullptr, const core::Vector3D& pos = core::Vector3D(0.f), f32 minorRadius = 1.f, f32 majorRadius = 2.f);
        CPlaneShape*            addPlane(CNode* parent = nullptr, const core::Vector3D& pos = core::Vector3D(0.f), f32 extent = 2.f);
        CRectangleShape*        addRectangle(CNode* parent = nullptr, const core::Rect32& rect = core::Rect32(0, 0, 1024, 768));
        CSkybox*                addSkyBox(const std::string& front, const std::string& back, const std::string& left, const std::string& right, const std::string& up, const std::string& down);
        CModel*                 addModel(const std::string& file, CNode* parent = nullptr, const core::Vector3D& pos = core::Vector3D(0.f));
        CBillboard*             addBillboard(const std::string& texture, CNode* parent = nullptr, const core::Vector3D& pos = core::Vector3D(0.f));
        CText*                  addText(CNode* parent = nullptr, const std::string& text = "text", const std::string& font = "arial.ttf", const core::Vector3D& pos = core::Vector3D(0.f), u32 size = 24U);

        CLight*                 addLight(CNode* parent = nullptr, const core::Vector3D& pos = core::Vector3D(0.f, 0.f, 1.f), const core::Vector4D& diffuse = core::Vector4D(1.f), f32 radius = 100.f);

        CCamera*                addCamera(CNode* parent = nullptr, const core::Vector3D& pos = core::Vector3D(0.f), const core::Vector3D& target = core::Vector3D(0.f, 0.f, -1.f), const core::Vector3D& up = core::Vector3D(0.f, 1.f, 0.f));
        CFPSCamera*             addFPSCamera(CNode* parent = nullptr, const core::Vector3D& pos = core::Vector3D(0.f), const core::Vector3D& target = core::Vector3D(0.f, 0.f, -1.f), f32 speed = 0.001f);

        CNode*                  getObjectByID(s32 id);
        CNode*                  getObjectByName(const std::string& name);

#ifdef _DEBUG
        void                    setDebugMode(bool active);
#endif

        void                    setActiveCamera(CCamera* camera);
        CCamera*                getActiveCamera() const;
        bool                    isActiveCamera(const CCamera* camera);

        s32                     getFPS() const;

    private:

        ScenePtr                m_scene;

        s32                     m_frameTime;
        u64                     m_lastTime;
        const f32               m_timeFactor = 1.f;

        s32                     m_framesCounted;
        u32                     m_fpsStartTime;
        s32                     m_fps;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CSceneManager>  SceneManagerPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_SCENE_MANAGER_H_
