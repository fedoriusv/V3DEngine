#ifndef _V3D_INPUT_EVENTS_H_
#define _V3D_INPUT_EVENTS_H_

#include "common.h"
#include "KeyCodes.h"

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

	enum EKeyboardInputEvent
	{
		eKeyboardUnknown = 0,
		eKeyboardPressDown,
		eKeyboardPressUp,

		eKeyboardCount
	};

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

	struct SInputEvent
	{
		SInputEvent();
		virtual ~SInputEvent();

		bool			operator<(const SInputEvent& event);

		EInputEventType	_eventType;
		EPriority		_priority;
		v3d::u64		_timeStamp;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	typedef std::shared_ptr<SInputEvent> SInputEventPtr;

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	struct SKeyboardInputEvent : public SInputEvent
	{
		SKeyboardInputEvent();
		virtual ~SKeyboardInputEvent();

		v3d::c8              _character;
		EKeyCode             _key;
		EKeyboardInputEvent  _event;
		v3d::u16             _modifers;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	typedef std::shared_ptr<SKeyboardInputEvent> SKeyboardInputEventPtr;

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

    struct SKeyCodes
    {
    public:

        void                    add(EKeyCode code, u32 systemCode);
        EKeyCode                get(u32 code);

    private:

        std::map<u32, EKeyCode> _key;
    };
}
}


#endif //_V3D_INPUT_EVENTS_H_
