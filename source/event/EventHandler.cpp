#include "event/EventHandler.h"

using namespace v3d;
using namespace v3d::event;

CEventHandler::CEventHandler()
{
}

CEventHandler::~CEventHandler()
{
}

void CEventHandler::update()
{
	while (!m_events.empty())
	{
		const SInputEventPtr event = m_events.front();
		
		if (onEvent(event))
		{
			int a = 0;
			//TODO:
		}

		m_events.pop();
	}
}

void CEventHandler::pushEvent(const SInputEventPtr event)
{
	m_events.push(event);
}
