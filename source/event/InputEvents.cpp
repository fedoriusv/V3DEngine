#include "InputEvents.h"

using namespace v3d;
using namespace v3d::event;

SInputEvent::SInputEvent()
    : _eventType(eUnknownInputEvent)
    , _priority(eNormal)
    , _timeStamp(0) //TODO: get current time, need CTimer
{
}

SInputEvent::~SInputEvent()
{
}

bool SInputEvent::operator<(const SInputEvent& event)
{
	return _priority < event._priority;
}

SKeyboardInputEvent::SKeyboardInputEvent()
    : _character(0)
    , _key(eKeyUknown)
    , _event(eKeyboardUnknown)
    , _modifers(0)
{
	_eventType = eKeyboardInputEvent;
}

SKeyboardInputEvent::~SKeyboardInputEvent()
{
}

SMouseInputEvent::SMouseInputEvent()
    : _position(0,0)
    , _wheel(0.0f)
    , _event(eMouseUnknown)
    , _modifers(0)
{
	_eventType = eMouseInputEvent;
}

SMouseInputEvent::~SMouseInputEvent()
{
}


void SKeyCodes::add(EKeyCode code, u32 systemCode)
{
    _key.insert(std::map<EKeyCode, u32>::value_type(code, systemCode));
}

u32 SKeyCodes::get(EKeyCode code)
{
    return _key[code];
}
