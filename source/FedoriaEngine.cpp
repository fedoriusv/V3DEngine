#include "FedoriaEngine.h"
#include "platform/Platform.h"
#include "event/InputEventHandler.h"

using namespace f3d;
using namespace f3d::platform;
using namespace f3d::event;
using namespace f3d::scene;

CFedoriaEngine::CFedoriaEngine()
: m_platform(nullptr)
, m_inputEventHandler(nullptr)
, m_scene(nullptr)
{
	CFedoriaEngine::init();
}

CFedoriaEngine::~CFedoriaEngine()
{
}

void CFedoriaEngine::init()
{
	m_platform = std::make_shared<CPlatform>(CPlatform());
	m_inputEventHandler = std::make_shared<CInputEventHandler>(CInputEventHandler());
	m_scene = std::make_shared<CSceneManager>(CSceneManager());
}

CPlatformPtr CFedoriaEngine::getPlatform() const
{
	return m_platform;
}

event::CInputEventHandlerPtr CFedoriaEngine::getInputEventHandler() const
{
	return m_inputEventHandler;
}

scene::CSceneManagerPtr CFedoriaEngine::getSceneManager() const
{
	return m_scene;
}

bool CFedoriaEngine::begin()
{
	m_inputEventHandler->update();
	m_scene->draw();

	return m_platform->begin();
}

bool CFedoriaEngine::end()
{
	return m_platform->end();
}
