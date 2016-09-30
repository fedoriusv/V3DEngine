#pragma once

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
        eGamepadButtonInput,
        eGamepadDpadDirection,
        eGamepadPOV,

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
        s32                 _pov;
        s32                 _axis[eAxesCount];
        s32                 _gamepad;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<SGamepadInputEvent> GamepadInputEventPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace event
} //namespace v3d
