#include "FedoriaEngine.h"
#include "platform/Platform.h"
#include "event/InputEventHandler.h"

using namespace f3d;
using namespace f3d::platform;
using namespace f3d::event;

CFedoriaEngine::CFedoriaEngine()
: m_platform( nullptr )
, m_inputEventHandler(nullptr)
{
	CFedoriaEngine::init();
}

CFedoriaEngine::~CFedoriaEngine()
{
	m_platform = nullptr;
}

void CFedoriaEngine::init()
{
	m_platform = std::make_shared<CPlatform>(CPlatform());
	m_inputEventHandler = std::make_shared<CInputEventHandler>(CInputEventHandler());
}

CPlatformPtr CFedoriaEngine::getPlatform() const
{
	return m_platform;
}

event::CInputEventHandlerPtr CFedoriaEngine::getInputEventHandler() const
{
	return m_inputEventHandler;
}

bool CFedoriaEngine::begin()
{
	m_inputEventHandler->update();

	return m_platform->begin();
}

bool CFedoriaEngine::end()
{
	return m_platform->end();
}
