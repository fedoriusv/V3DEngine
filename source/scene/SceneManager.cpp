#include "SceneManager.h"
#include "SampleShape.h"
#include "CubeShape.h"
#include "SphereShape.h"
#include "CylinderShape.h"
#include "TorusShape.h"
#include "DiskShape.h"
#include "PlaneShape.h"
#include "Camera.h"
#include "Light.h"
#include "FreeTypeFont.h"
#include "FPSCamera.h"
#include "Engine.h"
#include "utils/Logger.h"

#ifdef _PLATFORM_WIN_
#	include <windows.h>
#endif

using namespace v3d;
using namespace v3d::scene;
using namespace v3d::renderer;

CSceneManager::CSceneManager()
    : m_currentTime(0.0)
    , m_deltaTime(0.0)
    , m_lastTime(0.0)
    , m_camera(nullptr)
{
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

void CSceneManager::setActiveCamera(CCamera* camera)
{
    if (m_camera)
    {
        m_camera->m_active = false;
    }
    m_camera = camera;
    m_camera->m_active = true;
}

const CCamera* CSceneManager::getActiveCamera(const CCamera* camera)
{
    return m_camera;
}

bool CSceneManager::isActiveCamera(const CCamera* camera)
{
    return m_camera == camera;
}

void CSceneManager::init()
{
    for (std::vector<CNode*>::iterator iter = m_objects.begin(); iter < m_objects.end(); ++iter)
    {
        CNode* item = (*iter);
        item->init();
    }

    LOG_INFO("Scene: Init completed");
}

void CSceneManager::draw()
{
    CSceneManager::updateDeltaTime();
    CSceneManager::update(m_deltaTime);

    RENDERER->preRender();

    for (std::vector<CNode*>::iterator iter = m_drawObjects.begin(); iter < m_drawObjects.end(); ++iter)
    {
        CNode* item = (*iter);

        item->update(m_deltaTime);
        item->render();
    }

    RENDERER->postRender();
}

void CSceneManager::clear()
{
    m_drawObjects.clear();

	for (std::vector<CNode*>::iterator iter = m_objects.begin(); iter < m_objects.end(); ++iter)
	{
		delete (*iter);
		(*iter) = nullptr;
	}

	m_objects.clear();
}

bool CSceneManager::drop(CNode* node)
{
	std::vector<CNode*>::iterator iter = std::find(m_objects.begin(), m_objects.end(), node);

	if (iter != m_objects.end())
	{
		delete (*iter);
		(*iter) = nullptr;
		m_objects.erase(iter);

		return true;
	}

	return false;
}

void CSceneManager::addNode(CNode* node)
{
	if (node)
	{
		m_objects.push_back(node);
	}
}

void CSceneManager::update(v3d::f64 time)
{
    m_drawObjects.clear();

    for (std::vector<CNode*>::iterator iter = m_objects.begin(); iter < m_objects.end(); ++iter)
    {
        CNode* node = (*iter);

        switch (node->getNodeType())
        {
            case ENodeType::eShape:
            case ENodeType::eModel:
            {
                f32 priority = 0.0f;
                if (static_cast<CShape*>(node)->getMaterial()->getTransparency() > 0.0f)
                {
                    if (m_camera)
                    {
                        priority = (node->getPosition() - m_camera->getPosition()).length();
                    }
                    else
                    {
                        priority = node->getPosition().z;
                    }
                    node->m_priority = priority;

                    if (checkDistance(node, priority))
                    {
                        m_drawObjects.push_back(node);
                    }
                }
            }
            break;

            case ENodeType::eCamera:
            {
                node->m_priority = 1000000.0f;

                if (static_cast<CCamera*>(node)->isActive())
                {
                    m_drawObjects.push_back(node);
                }
            }
            break;

            case ENodeType::eLight:
            case ENodeType::eFog:
            {
                node->m_priority = -1000000.0f;
            }
            break;

            case ENodeType::eFont:
            {
                node->m_priority = 0.0f;

                m_drawObjects.push_back(node);
            }
            break;

            default:
            break;
        }
    }

    std::sort(m_drawObjects.begin(), m_drawObjects.end(), [](CNode* node0, CNode* node1)
        {
            return  (node0->getPriority() > node1->getPriority());
        });
}

void CSceneManager::updateDeltaTime()
{
#ifdef _PLATFORM_WIN_
	LARGE_INTEGER nTime;
	QueryPerformanceCounter(&nTime);
	m_currentTime = static_cast<double>(nTime.QuadPart);
#endif
	m_deltaTime = (m_currentTime - m_lastTime) * 0.001;
	m_lastTime = m_currentTime;
}

CNode* CSceneManager::getObjectByID(const s32 id)
{
	for (std::vector<CNode*>::iterator iter = m_objects.begin(); iter < m_objects.end(); ++iter)
	{
		if ((*iter)->getID() == id)
		{
			return (*iter);
		}
	}

	return nullptr;
}

CNode* CSceneManager::getObjectByName(const std::string& name)
{
	for (std::vector<CNode*>::const_iterator iter = m_objects.begin(); iter < m_objects.end(); ++iter)
	{
		if ((*iter)->getName().compare(name) == 0)
		{
			return (*iter);
		}
	}

	return nullptr;
}

void CSceneManager::setBackgroundColor(const core::Vector3D& color)
{
    RENDERER->setBackColor(color);
}

const core::Vector3D& CSceneManager::getBackgroundColor() const
{
    return RENDERER->getBackColor();
}

bool CSceneManager::checkDistance(const CNode* node, const f32 distance)
{
    if (node->getNodeType() == ENodeType::eShape || node->getNodeType() == ENodeType::eModel)
    {
        const RenderTechniquePtr& technique = static_cast<const CShape*>(node)->getMaterial()->getRenderTechique();
        for (u32 pass = 0; pass < technique->getRenderPassCount(); ++pass)
        {
            const RenderLODPtr& lod = technique->getRenderPass(pass)->getRenderLOD();
            if (lod->getGeometryDistance() < distance && lod->getGeometryDistance() > 0)
            {
                return false;
            }
        }
    }

    return true;
}

CNode* CSceneManager::addSample(CNode* parent, const core::Vector3D& pos)
{
    CSampleShape* node = new CSampleShape();
    node->setParent(parent);
    node->setPosition(pos);

    CSceneManager::addNode(node);

    return node;
}

CNode* CSceneManager::addCube(CNode* parent, const core::Vector3D& pos, const f32 size)
{
    CCubeShape* node = new CCubeShape();
    node->setParent(parent);
    node->setPosition(pos);
    node->setScale(size);

    CSceneManager::addNode(node);

    return node;
}

CNode* CSceneManager::addSphere(CNode* parent, const Vector3D& pos, const f32 radius)
{
    CSphereShape* node = new CSphereShape(radius);
    node->setParent(parent);
    node->setPosition(pos);

    CSceneManager::addNode(node);

    return node;
}

CNode* CSceneManager::addCylinder(CNode* parent, const Vector3D& pos, const f32 radius, const f32 height)
{
    CCylinderShape* node = new CCylinderShape(radius, height);
    node->setParent(parent);
    node->setPosition(pos);

    CSceneManager::addNode(node);

    return node;
}

CNode* CSceneManager::addTorus(CNode* parent, const Vector3D& pos, const f32 minorRadius, const f32 majorRadius)
{
    CTorusShape* node = new CTorusShape(minorRadius, majorRadius);
    node->setParent(parent);
    node->setPosition(pos);

    CSceneManager::addNode(node);

    return node;
}

CNode* CSceneManager::addDisk(CNode* parent, const Vector3D& pos, const f32 minorRadius, const f32 majorRadius)
{
    CDiskShape* node = new CDiskShape(minorRadius, majorRadius);
    node->setParent(parent);
    node->setPosition(pos);

    CSceneManager::addNode(node);

    return node;
}

CNode* CSceneManager::addPlane(CNode* parent, const Vector3D& pos, const f32 extent)
{
    CPlaneShape* node = new CPlaneShape(extent);
    node->setParent(parent);
    node->setPosition(pos);

    CSceneManager::addNode(node);

    return node;
}

CNode* CSceneManager::addCamera(CNode* parent, const core::Vector3D& pos, const core::Vector3D& target, const core::Vector3D& up)
{
    CCamera* node = new CCamera();
    node->setParent(parent);
    node->setPosition(pos);
    node->setTarget(target);
    node->setUpVector(up);
    if (m_camera)
    {
        m_camera->m_active = false;
    }
    m_camera = node;
    m_camera->m_active = true;

    CSceneManager::addNode(node);

    return node;
}

CNode* CSceneManager::addFPSCamera(CNode* parent, const core::Vector3D& pos, const core::Vector3D& target, const f32 speed)
{
    CFPSCamera* node = new CFPSCamera();
    node->setParent(parent);
    node->setPosition(pos);
    node->setPosition(target);
    node->setUpVector(Vector3D(0.0f, 1.0f, 0.0f));
    node->setSpeed(speed);

    if (m_camera)
    {
        m_camera->m_active = false;
    }
    m_camera = node;
    m_camera->m_active = true;

    CSceneManager::addNode(node);

    return node;
}

CNode* CSceneManager::addLight(CNode* parent, const Vector3D& pos, const Vector4D& diffuse, const f32 radius)
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

CNode* CSceneManager::addFreeTypeFont(CNode* parent, const std::string& text, const std::string& font, const Vector3D& pos, const u32 size)
{
    CFont*  node = new CFreeTypeFont(font);
    node->setParent(parent);
    node->setPosition(pos);
    node->setText(text);
    node->setSize(size);

    CSceneManager::addNode(node);

    return node;
}
