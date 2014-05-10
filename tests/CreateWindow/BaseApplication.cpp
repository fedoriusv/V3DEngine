#include "BaseApplication.h"

using namespace f3d;
using namespace f3d::platform;

BaseApplication::BaseApplication(int& argc, char** argv)
{
	m_engine = CFedoriaEngine::getInstance();
	m_engine->getPlatform()->createWindowWithContext(core::Dimension2D(800, 600));
}

BaseApplication::~BaseApplication()
{
	CFedoriaEngine::freeInstance();
}

int BaseApplication::exec()
{
	this->init();

	while (m_engine->begin())
	{
		this->run();

		m_engine->end();
	}

	return 0;
}