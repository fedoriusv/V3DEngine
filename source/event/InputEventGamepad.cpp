#include "InputEventGamepad.h"

using namespace v3d;
using namespace event;

SGamepadInputEvent::SGamepadInputEvent()
: _event(eGamepadUnknown)
, _pov(0)
, _buttons(0)
, _gamepad(-1)
{
    _eventType = eGamepadInputEvent;
}

SGamepadInputEvent::~SGamepadInputEvent()
{
}

bool SGamepadInputEvent::operator == (const SGamepadInputEvent& event) const
{
    return (_event == event._event &&
        _buttons == event._buttons &&
        _gamepad == event._gamepad);
}