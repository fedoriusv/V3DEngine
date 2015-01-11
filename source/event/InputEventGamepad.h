#ifndef _V3D_INPUT_EVENT_GAMEPAD_H_
#define _V3D_INPUT_EVENT_GAMEPAD_H_

#include "common.h"
#include "InputEvents.h"

namespace v3d
{
namespace event
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EGamepadInputEvent
    {
        eGamepadUnknown = 0,
        eGamepadButtonDown,
        eGamepadButtonUp,
        eGamepadDpadMove,
        eGamepadLeftStickMove,
        eGamepadRightStickMove,

        eGamepadCount
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EGamepadAxis
    {
        eAxis_X = 0,
        eAxis_Y,
        eAxis_Z,
        eAxis_R,
        eAxis_U,
        eAxis_V,

        eAxesCount
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EGamepadButton
    {
        eButtonNone = -1,

        eButtonA,
        eButtonB,

        eButtonCount = 32
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    struct SGamepadInputEvent : public SInputEvent
    {
        SGamepadInputEvent();
        virtual ~SGamepadInputEvent();

        bool    operator == (const SGamepadInputEvent& event) const;

        EGamepadInputEvent  _event;
        u32                 _buttons;
        u16                 _pov;
        s16                 _axis[eAxesCount];
        s32                 _gamepad;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<SGamepadInputEvent> GamepadInputEventPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_INPUT_EVENT_KEYBOARD_H_
