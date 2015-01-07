#ifndef _V3D_INPUT_EVENTS_H_
#define _V3D_INPUT_EVENTS_H_

#include "common.h"

namespace v3d
{
namespace event
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EInputEventType
    {
        eUnknownInputEvent = 0,
        eMouseInputEvent,
        eKeyboardInputEvent,
        eGamepadInputEvent,
        eJoystickInputEvent,
        eTouchInputEvent
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EPriority
    {
        eLow    = 10,
        eNormal = 20,
        eHigh   = 30
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    struct SInputEvent
    {
        SInputEvent();
        virtual         ~SInputEvent();

        bool            operator<(const SInputEvent& event);

        EInputEventType _eventType;
        EPriority       _priority;
        v3d::u64        _timeStamp;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<SInputEvent> SInputEventPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_INPUT_EVENTS_H_
