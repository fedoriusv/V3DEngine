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