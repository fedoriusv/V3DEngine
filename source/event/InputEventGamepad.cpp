#include "InputEventGamepad.h"

namespace v3d
{
namespace event
{

SGamepadInputEvent::SGamepadInputEvent()
    : _event(eGamepadUnknown)
    , _buttons(0)
    , _pov(0)
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

} //namespace event
} //namespace v3d
