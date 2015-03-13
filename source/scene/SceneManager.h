#ifndef _V3D_SCENE_MANAGER_H_
#define _V3D_SCENE_MANAGER_H_

#include "Scene.h"

namespace v3d
{
namespace scene
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CCamera;
    class CNode;
    class CSampleShape;
    class CCubeShape;
    class CSphereShape;
    class CCylinderShape;
    class CTorusShape;
    class CDiskShape;
    class CPlaneShape;
    class CRectangleShape;
    class CSkybox;
    class CModel;
    class CText;
    class CLight;
    class CCamera;
    class CFPSCamera;

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

        CSampleShape*           addSample(CNode* parent = nullptr, const Vector3D& pos = Vector3D(0.0f));
        CCubeShape*             addCube(CNode* parent = nullptr, const Vector3D& pos = Vector3D(0.0f), f32 size = 1.0f);
        CSphereShape*           addSphere(CNode* parent = nullptr, const Vector3D& pos = Vector3D(0.0f, 0.0f, 0.0f), f32 radius = 1.0f);
        CCylinderShape*         addCylinder(CNode* parent = nullptr, const Vector3D& pos = Vector3D(0.0f, 0.0f, 0.0f), f32 radius = 0.5f, f32 height = 1.0f);
        CTorusShape*            addTorus(CNode* parent = nullptr, const Vector3D& pos = Vector3D(0.0f, 0.0f, 0.0f), f32 minorRadius = 0.5f, f32 majorRadius = 1.0f);
        CDiskShape*             addDisk(CNode* parent = nullptr, const Vector3D& pos = Vector3D(0.0f, 0.0f, 0.0f), f32 minorRadius = 1.0f, f32 majorRadius = 2.0f);
        CPlaneShape*            addPlane(CNode* parent = nullptr, const Vector3D& pos = Vector3D(0.0f, 0.0f, 0.0f), f32 extent = 2.0f);
        CRectangleShape*        addRectangle(CNode* parent = nullptr, const Rect& rect = Rect(0, 0, 1024, 768));
        CSkybox*                addSkyBox(const std::string& front, const std::string& back, const std::string& left, const std::string& right, const std::string& up, const std::string& down);
        CModel*                 addModel(const std::string& file, const std::string& techique, CNode* parent = nullptr, const Vector3D& pos = Vector3D(0.0f, 0.0f, 0.0f));

        CText*                  addText(CNode* parent = nullptr, const std::string& text = "text", const std::string& font = "arial.ttf", const Vector3D& pos = Vector3D(0.0f), u32 size = 24U);

        CLight*                 addLight(CNode* parent = nullptr, const Vector3D& pos = Vector3D(0.0f, 0.0f, 1.0f), const Vector4D& diffuse = Vector4D(1.0f), f32 radius = 100.0f);

        CCamera*                addCamera(CNode* parent = nullptr, const Vector3D& pos = Vector3D(0.0f), const Vector3D& target = Vector3D(0.0f, 0.0f, -1.0f), const Vector3D& up = Vector3D(0.0f, 1.0f, 0.0f));
        CFPSCamera*             addFPSCamera(CNode* parent = nullptr, const Vector3D& pos = Vector3D(0.0f), const Vector3D& target = Vector3D(0.0f, 0.0f, -1.0f), f32 speed = 0.001f);

        CNode*                  getObjectByID(s32 id);
        CNode*                  getObjectByName(const std::string& name);

#ifdef _DEBUG
        void                    setDebugMode(bool active);
#endif

        void                    setActiveCamera(CCamera* camera);
        CCamera*                getActiveCamera() const;
        bool                    isActiveCamera(const CCamera* camera);

    private:

        ScenePtr                m_scene;

        f64                     m_currentTime;
        f64                     m_deltaTime;
        f64                     m_lastTime;

        void                    updateDeltaTime();
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CSceneManager>  SceneManagerPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_SCENE_MANAGER_H_
