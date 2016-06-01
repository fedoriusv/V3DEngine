#include "SceneManager.h"
#include "SampleShape.h"
#include "CubeShape.h"
#include "SphereShape.h"
#include "CylinderShape.h"
#include "TorusShape.h"
#include "DiskShape.h"
#include "PlaneShape.h"
#include "RectangleShape.h"
#include "Billboard.h"
#include "ParticleSystem.h"
#include "Camera.h"
#include "Skybox.h"
#include "Light.h"
#include "Text.h"
#include "Model.h"
#include "FPSCamera.h"
#include "Engine.h"
#include "utils/Logger.h"
#include "utils/Timer.h"

#ifdef _PLATFORM_WIN_
#	include <windows.h>
#endif

#include "ModelManager.h"

using namespace v3d;
using namespace core;
using namespace scene;
using namespace renderer;
using namespace utils;

CSceneManager::CSceneManager()
    : m_frameTime(0)
    , m_lastTime(0U)

    , m_framesCounted(0)
    , m_fpsStartTime(0U)
    , m_fps(0)
{
    m_scene = std::make_shared<CScene>();
    m_lastTime = CTimer::getCurrentTime();
}

CSceneManager::~CSceneManager()
{
    CSceneManager::clear();
}

#ifdef _DEBUG
void CSceneManager::setDebugMode(bool active)
{
    RENDERER->setDebugMode(active);
}
#endif

s32 CSceneManager::getFPS() const
{
    return m_fps;
}

s32 CSceneManager::getDeltaTime() const
{
    return m_frameTime;
}

void CSceneManager::setActiveCamera(CCamera* camera)
{
    m_scene->setActiveCamera(camera);
}

CCamera* CSceneManager::getActiveCamera() const
{
    return m_scene->getActiveCamera();
}

bool CSceneManager::isActiveCamera(const CCamera* camera)
{
    return m_scene->isActiveCamera(camera);
}

void CSceneManager::init()
{
    m_scene->init();
}

void CSceneManager::draw()
{
    const u64 ticks = CTimer::getCurrentTime();
    m_frameTime = (s32)((ticks - m_lastTime) * m_timeFactor);
    m_lastTime = ticks;

    if (m_frameTime <= 0)
    {
        m_frameTime = 1;
    }

    //FPS
    ++m_framesCounted;
    const u32 fpsDeltaTime = static_cast<u32>(ticks - m_fpsStartTime);
    if (fpsDeltaTime > 1000) //1 sec
    {
        m_fps = m_framesCounted;
        m_framesCounted = 0;
        m_fpsStartTime = static_cast<u32>(ticks);
    }

    m_scene->draw(m_frameTime);
}

void CSceneManager::clear()
{
    m_scene->clear();
}

bool CSceneManager::dropNode(CNode* node)
{
    return m_scene->drop(node);
}

void CSceneManager::addNode(CNode* node)
{
    if (node)
    {
        m_scene->add(node);
    }
}

CNode* CSceneManager::getObjectByID(s32 id)
{
    return m_scene->getNodeByID(id);
}

CNode* CSceneManager::getObjectByName(const std::string& name)
{
    return m_scene->getNodeByName(name);
}

CSampleShape* CSceneManager::addSample(CNode* parent, const core::Vector3D& pos)
{
    CSampleShape* node = new CSampleShape();
    node->setParent(parent);
    node->setPosition(pos);

    CSceneManager::addNode(node);

    return node;
}

CCubeShape* CSceneManager::addCube(CNode* parent, const core::Vector3D& pos, f32 size)
{
    CCubeShape* node = new CCubeShape();
    node->setParent(parent);
    node->setPosition(pos);
    node->setScale(size);

    CSceneManager::addNode(node);

    return node;
}

CSphereShape* CSceneManager::addSphere(CNode* parent, const Vector3D& pos, f32 radius)
{
    CSphereShape* node = new CSphereShape(radius);
    node->setParent(parent);
    node->setPosition(pos);

    CSceneManager::addNode(node);

    return node;
}

CCylinderShape* CSceneManager::addCylinder(CNode* parent, const Vector3D& pos, f32 radius, f32 height)
{
    CCylinderShape* node = new CCylinderShape(radius, height);
    node->setParent(parent);
    node->setPosition(pos);

    CSceneManager::addNode(node);

    return node;
}

CTorusShape* CSceneManager::addTorus(CNode* parent, const Vector3D& pos, f32 minorRadius, f32 majorRadius)
{
    CTorusShape* node = new CTorusShape(minorRadius, majorRadius);
    node->setParent(parent);
    node->setPosition(pos);

    CSceneManager::addNode(node);

    return node;
}

CDiskShape* CSceneManager::addDisk(CNode* parent, const Vector3D& pos, f32 minorRadius, f32 majorRadius)
{
    CDiskShape* node = new CDiskShape(minorRadius, majorRadius);
    node->setParent(parent);
    node->setPosition(pos);

    CSceneManager::addNode(node);

    return node;
}

CPlaneShape* CSceneManager::addPlane(CNode* parent, const Vector3D& pos, f32 extent)
{
    CPlaneShape* node = new CPlaneShape(extent);
    node->setParent(parent);
    node->setPosition(pos);

    CSceneManager::addNode(node);

    return node;
}

CRectangleShape* CSceneManager::addRectangle(CNode* parent, const Rect32& rect)
{
    CRectangleShape* node = new CRectangleShape(rect);
    node->setParent(parent);
    node->setPosition(Vector3D(0.f, 0.f, 0.f));

    CSceneManager::addNode(node);

    return node;
}

CSkybox* CSceneManager::addSkyBox(const std::string& front, const std::string& back, const std::string& left, const std::string& right, const std::string& up, const std::string& down)
{
    CSkybox* node = new CSkybox();
    node->load(front, back, left, right, up, down);

    CSceneManager::addNode(node);

    return node;

}

CCamera* CSceneManager::addCamera(CNode* parent, const core::Vector3D& pos, const core::Vector3D& target, const core::Vector3D& up)
{
    CCamera* node = new CCamera();
    node->setParent(parent);
    node->setPosition(pos);
    node->setTarget(target);
    node->setUpVector(up);
    
    m_scene->setActiveCamera(node);

    CSceneManager::addNode(node);

    return node;
}

CFPSCamera* CSceneManager::addFPSCamera(CNode* parent, const core::Vector3D& pos, const core::Vector3D& target, f32 speed)
{
    CFPSCamera* node = new CFPSCamera();
    node->setParent(parent);
    node->setPosition(pos);
    node->setPosition(target);
    node->setUpVector(Vector3D(0.0f, 1.0f, 0.0f));
    node->setSpeed(speed);

    m_scene->setActiveCamera(node);

    CSceneManager::addNode(node);

    return node;
}

CParticleSystem* CSceneManager::addParticleSystem(const std::string& texture, CNode* parent, Vector3D& pos, u32 amout)
{
    CParticleSystem* node = new CParticleSystem(texture);
    node->setParent(parent);
    node->setPosition(pos);
    node->setAmountOfParticles(amout);

    CSceneManager::addNode(node);

    return node;
}

CLight* CSceneManager::addLight(CNode* parent, const Vector3D& pos, const Vector4D& diffuse, f32 radius)
{
    CLight* node = new CLight();
    node->setParent(parent);
    node->setPosition(pos);
    node->setDiffuse(diffuse);
    node->setRadius(radius);

    CSceneManager::addNode(node);
    RENDERER->addLight(node);

    return node;
}

CText* CSceneManager::addText(CNode* parent, const std::string& text, const std::string& font, const Vector3D& pos, u32 size)
{
    CText*  node = new CText(font);
    node->setParent(parent);
    node->setPosition(pos);
    node->setText(text);
    node->setSize(size);

    CSceneManager::addNode(node);

    return node;
}

CModel* CSceneManager::addModel(const std::string& file, CNode* parent, const Vector3D& pos)
{
    CModel* node = CModelManager::getInstance()->load(file);
    if (node)
    {
        node->setParent(parent);
        node->setPosition(pos);

        CSceneManager::addNode(node);
    }
    return node;
}

CBillboard* CSceneManager::addBillboard(const std::string& texture, CNode* parent, const Vector3D& pos)
{
    CBillboard* node = new CBillboard(texture);
    node->setParent(parent);
    node->setPosition(pos);

    CSceneManager::addNode(node);

    return node;
}
