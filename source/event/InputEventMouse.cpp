#include "InputEventMouse.h"

using namespace v3d;
using namespace event;

SMouseInputEvent::SMouseInputEvent()
: _position(0, 0)
, _wheel(0.0f)
, _event(eMouseUnknown)
, _modifers(0)
{
    _eventType = eMouseInputEvent;
}

SMouseInputEvent::~SMouseInputEvent()
{
}

bool SMouseInputEvent::operator == (const SMouseInputEvent& event) const
{
    return (_position == event._position &&
        _wheel == event._wheel &&
        _event == event._event);
}