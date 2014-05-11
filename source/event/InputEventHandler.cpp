#include "InputEventHandler.h"
#include "utils/Logger.h"

using namespace f3d;
using namespace f3d::event;

CInputEventHandler::CInputEventHandler()
	: m_mousePosition(0,0)
	, m_mouseWheel(0.0f)
	, m_keyboardSignature(nullptr)
	, m_mouseSignature(nullptr)
{
	resetKeyPressed();
}

CInputEventHandler::~CInputEventHandler()
{
}

void CInputEventHandler::resetKeyPressed()
{
	for (u32 key = 0; key < eKey_Codes_Count; ++key)
	{
		m_keysPressed[key] = false;
	}
}

void CInputEventHandler::update()
{
	CEventHandler::update();
}

bool CInputEventHandler::onEvent(const SInputEventPtr event)
{
	switch (event->m_eventType)
	{
		case eKeyboardInputEvent:
		{
			const SKeyboardInputEventPtr keyEvent = std::static_pointer_cast<SKeyboardInputEvent>(event);

			switch (keyEvent->m_event)
			{
				case eKeyboardPressDown:
				{
					m_keysPressed[keyEvent->m_key] = true;
					break;
				}

				case eKeyboardPressUp:
				{
					m_keysPressed[keyEvent->m_key] = false;
					break;
				}

				default:
				{
					return false;
				}
			}

			if (m_keyboardSignature)
			{
				m_keyboardSignature(keyEvent);
			}
			return true;
		}

		case eMouseInputEvent:
		{
			const SMouseInputEventPtr mouseEvent = std::static_pointer_cast<SMouseInputEvent>(event);
			
			for (u32 state = 0; state < eMouseCount; ++state)
			{
				m_mouseStates[state] = state == mouseEvent->m_event;
			}
			
			switch (mouseEvent->m_event)
			{
				case	eMouseMoved:
				case	eLeftMousePressedDown:
				case	eLeftMousePressedUp:
				case	eLeftMouseDoubleClick:
				case	eRightMousePressedDown:
				case	eRightMousePressedUp:
				case	eRightMouseDoubleClick:
				case	eMiddleMousePressedDown:
				case	eMiddleMousePressedUp:
				case	eMiddleMouseDoubleClick:
				{
					break;
				}

				default:
				{
					 return false;
				}
			}

			m_mousePosition = mouseEvent->m_position;
			m_mouseWheel = mouseEvent->m_wheel;

			if (m_mouseSignature)
			{
				m_mouseSignature(mouseEvent);
			}
			return true;
		};

		default:
		{
			return false;
		}
	}
}

void CInputEventHandler::connectKeyboardEvent(std::function<void(const SKeyboardInputEventPtr&)> signature)
{
	if (signature)
	{
		m_keyboardSignature = signature;
	}
}

void CInputEventHandler::connectMouseEvent(std::function<void(const SMouseInputEventPtr&)> signature)
{
	if (signature)
	{
		m_mouseSignature = signature;
	}
}

bool CInputEventHandler::isKeyPressed(const EKeyCode& code)  const
{
	return m_keysPressed[code];
}

bool CInputEventHandler::isLeftMousePressed() const
{
	return m_mouseStates[eLeftMousePressedDown];
}

bool CInputEventHandler::isRightMousePressed() const
{
	return m_mouseStates[eRightMousePressedDown];
}


bool CInputEventHandler::isMiddleMousePressed() const
{
	return m_mouseStates[eMiddleMousePressedDown];
}

const core::Dimension2D& CInputEventHandler::getCursorPosition() const
{
	return m_mousePosition;
}

float CInputEventHandler::getMouseWheel() const
{
	return m_mouseWheel;
}