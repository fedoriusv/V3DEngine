#include "InputEvents.h"

using namespace v3d;
using namespace v3d::event;

SInputEvent::SInputEvent()
: m_eventType(eUnknownInputEvent)
, m_priority(eNormal)
, m_timeStamp(0) //TODO: get current time, need CTimer
{
}

SInputEvent::~SInputEvent()
{
}

bool SInputEvent::operator<(const SInputEvent& event)
{
	return m_priority < event.m_priority;
}

SKeyboardInputEvent::SKeyboardInputEvent()
: m_character(0)
, m_key(eKeyUknown)
, m_event(eKeyboardUnknown)
, m_modifers(0)
{
	m_eventType = eKeyboardInputEvent;
}

SKeyboardInputEvent::~SKeyboardInputEvent()
{
}

SMouseInputEvent::SMouseInputEvent()
: m_position(0,0)
, m_wheel(0.0f)
, m_event(eMouseUnknown)
, m_modifers(0)
{
	m_eventType = eMouseInputEvent;
}

SMouseInputEvent::~SMouseInputEvent()
{
}
