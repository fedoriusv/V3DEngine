#include "scene/SceneManager.h"
#ifdef _PLATFORM_WIN_
#	include <windows.h>
#endif

using namespace f3d;
using namespace f3d::scene;

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
	//
}

void CSceneManager::draw()
{
	CSceneManager::updateDeltaTime();
	CSceneManager::update(m_deltaTime);

	//CRender::getInstance()->beginRender();

	for (std::vector<CNode*>::iterator iter = m_objects.begin(); iter < m_objects.end(); ++iter)
	{
		CNode* item = (*iter);

		//item->update(m_dDeltaTime);
		//item->render();
	}

	//CRender::getInstance()->endRender();
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

void CSceneManager::update(double time)
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

CObject* CSceneManager::getObjectByID(const s32 id)
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

CObject* CSceneManager::getObjectByName(const std::string& name)
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