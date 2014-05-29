#ifndef _F3D_INPUT_EVENTS_H_
#define _F3D_INPUT_EVENTS_H_

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
		eMouseWheelUp,
		eMouseWheelDown,

		eMouseCount
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	struct SInputEvent
	{
		SInputEvent();
		virtual ~SInputEvent();

		bool			operator<(const SInputEvent& event);

		EInputEventType	m_eventType;
		EPriority		m_priority;
		v3d::u64		m_timeStamp;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	typedef std::shared_ptr<SInputEvent> SInputEventPtr;

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	struct SKeyboardInputEvent : public SInputEvent
	{
		SKeyboardInputEvent();
		virtual ~SKeyboardInputEvent();

		v3d::c8              m_character;
		EKeyCode             m_key;
		EKeyboardInputEvent  m_event;
		v3d::u16             m_modifers;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	typedef std::shared_ptr<SKeyboardInputEvent> SKeyboardInputEventPtr;

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	struct SMouseInputEvent : public SInputEvent
	{
		SMouseInputEvent();
		virtual ~SMouseInputEvent();

		core::Dimension2D m_position; //WARN: need class Point
		v3d::f32          m_wheel;
		EMouseInputEvent  m_event;
		v3d::u16          m_modifers;

	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	
	typedef std::shared_ptr<SMouseInputEvent> SMouseInputEventPtr;

	/////////////////////////////////////////////////////////////////////////////////////////////////////
}
}


#endif //_F3D_INPUT_EVENTS_H_