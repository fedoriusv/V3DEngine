#pragma once

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

        bool operator == (const SMouseInputEvent& event) const;

        core::Point2D     _position;
        v3d::f32          _wheel;
        EMouseInputEvent  _event;
        v3d::u16          _modifers;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<SMouseInputEvent> MouseInputEventPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace event
} //namespace v3d
