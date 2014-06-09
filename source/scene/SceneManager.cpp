#include "SceneManager.h"
#include "CubeShape.h"
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
{
}

CSceneManager::~CSceneManager()
{
	CSceneManager::clear();
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

	for (std::vector<CNode*>::iterator iter = m_objects.begin(); iter < m_objects.end(); ++iter)
	{
		CNode* item = (*iter);

        item->update(m_deltaTime);
		item->render();
	}

    RENDERER->postRender();
}

void CSceneManager::clear()
{
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
	v3d::CEngine::getInstance()->getRenderer()->setBackColor(color);
}

const core::Vector3D& CSceneManager::getBackgroundColor() const
{
	return v3d::CEngine::getInstance()->getRenderer()->getBackColor();
}

CNode* CSceneManager::addCube(CNode* parent, const core::Vector3D& pos, const float size)
{
    CCubeShape* node = new CCubeShape();
    node->setParent(parent);
    node->setPosition(pos);
    node->setScale(size);

    CSceneManager::addNode(node);

    return node;
}