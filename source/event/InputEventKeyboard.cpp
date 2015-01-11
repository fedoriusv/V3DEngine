#include "InputEventKeyboard.h"

using namespace v3d;
using namespace event;

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

bool SKeyboardInputEvent::operator == (const SKeyboardInputEvent& event) const
{
    return (_character == event._character &&
        _key == event._key &&
        _event == event._event);
}

void SKeyCodes::add(EKeyCode code, u32 systemCode)
{
    _key.insert(std::map<u32, EKeyCode>::value_type(systemCode, code));
}

EKeyCode SKeyCodes::get(u32 code)
{
    if (_key.find(code) != _key.end())
    {
        return _key[code];
    }

    return eKeyUknown;
}