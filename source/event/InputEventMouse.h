#ifndef _V3D_INPUT_EVENT_MOUSE_H_
#define _V3D_INPUT_EVENT_MOUSE_H_

#include "common.h"
#include "InputEvents.h"

namespace v3d
{
namespace event
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EMouseInputEvent
    {
        eMouseUnknown = 0,
        eLeftMousePressedDown,
        eLeftMousePressedUp,
        eLeftMouseDoubleClick,
        eRightMousePressedDown,
        eRightMousePressedUp,
        eRightMouseDoubleClick,
        eMiddleMousePressedDown,
        eMiddleMousePressedUp,
        eMiddleMouseDoubleClick,
        eMouseMoved,
        eMouseWheel,

        eMouseCount
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    struct SMouseInputEvent : public SInputEvent
    {
        SMouseInputEvent();
        virtual ~SMouseInputEvent();

        core::Dimension2D _position; //WARN: need class Point
        v3d::f32          _wheel;
        EMouseInputEvent  _event;
        v3d::u16          _modifers;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<SMouseInputEvent> SMouseInputEventPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_INPUT_EVENT_KEYBOARD_H_
