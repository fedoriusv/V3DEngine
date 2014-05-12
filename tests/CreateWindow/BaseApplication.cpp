#include "BaseApplication.h"

using namespace f3d;
using namespace f3d::platform;

BaseApplication::BaseApplication(int& argc, char** argv)
{
	m_engine = CEngine::getInstance();
}

BaseApplication::~BaseApplication()
{
	CEngine::freeInstance();
}

int BaseApplication::exec()
{
	if (!m_engine->init())
	{
		delete this;
		return 0;
	}
	this->init();
	//m_engine->getSceneManager()->init();

	while (m_engine->begin())
	{
		this->run();

		m_engine->end();
	}

	delete this;
	return 0;
}

platform::CPlatformPtr BaseApplication::getPlatform() const
{
	return m_engine->getPlatform();
}

event::CInputEventHandlerPtr BaseApplication::getInputEventHandler() const
{
	return m_engine->getInputEventHandler();
}

f3d::scene::CSceneManagerPtr BaseApplication::getSceneManager() const
{
	return m_engine->getSceneManager();
}
