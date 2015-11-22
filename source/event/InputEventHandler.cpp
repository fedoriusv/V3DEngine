#include "InputEventHandler.h"
#include "EventManager.h"
#include "utils/Logger.h"

using namespace v3d;
using namespace v3d::event;

CInputEventHandler::CInputEventHandler()
: m_gamepadStates(0U)
, m_mousePosition(0, 0)
, m_mouseWheel(0.0f)
{
    resetKeyPressed();
}

CInputEventHandler::~CInputEventHandler()
{
    m_keyboardSignatures.clear();
    m_mouseSignatures.clear();
    m_gamepadSignatures.clear();
}

void CInputEventHandler::resetKeyPressed()
{
    for (u32 key = 0; key < eKey_Codes_Count; ++key)
    {
        m_keysPressed[key] = false;
    }
}

void CInputEventHandler::setEnableEvents(bool enable)
{
    if (enable)
    {
        CEventManager::getInstance()->attach(eKeyboardInputEvent, shared_from_this());
        CEventManager::getInstance()->attach(eMouseInputEvent, shared_from_this());
        CEventManager::getInstance()->attach(eGamepadInputEvent, shared_from_this());
    }
    else
    {
        CEventManager::getInstance()->dettach(eKeyboardInputEvent, shared_from_this());
        CEventManager::getInstance()->dettach(eMouseInputEvent, shared_from_this());
        CEventManager::getInstance()->dettach(eGamepadInputEvent, shared_from_this());
    }
}

void CInputEventHandler::update()
{
    CEventManager::getInstance()->update();
}

bool CInputEventHandler::onEvent(const SInputEventPtr& event)
{
    switch (event->_eventType)
    {
        case eKeyboardInputEvent:
        {
            const KeyboardInputEventPtr& keyEvent = std::static_pointer_cast<SKeyboardInputEvent>(event);

            switch (keyEvent->_event)
            {
                case eKeyboardPressDown:
                {
                    m_keysPressed[keyEvent->_key] = true;
                    break;
                }

                case eKeyboardPressUp:
                {
                    m_keysPressed[keyEvent->_key] = false;
                    break;
                }

                default:
                {
                    return false;
                }
            }

            for (std::vector<KeyboardCallback>::const_iterator iter = m_keyboardSignatures.cbegin(); iter != m_keyboardSignatures.cend(); ++iter)
            {
                if ((*iter))
                {
                    (*iter)(keyEvent);
                }
            }
            return true;
        }

        case eMouseInputEvent:
        {
            const MouseInputEventPtr& mouseEvent = std::static_pointer_cast<SMouseInputEvent>(event);

            for (u32 state = 0; state < eMouseCount; ++state)
            {
                m_mouseStates[state] = state == mouseEvent->_event;
            }

            switch (mouseEvent->_event)
            {
                case eMouseMoved:
                case eLeftMousePressedDown:
                case eLeftMousePressedUp:
                case eLeftMouseDoubleClick:
                case eRightMousePressedDown:
                case eRightMousePressedUp:
                case eRightMouseDoubleClick:
                case eMiddleMousePressedDown:
                case eMiddleMousePressedUp:
                case eMiddleMouseDoubleClick:
                {
                    break;
                }

                default:
                {
                    return false;
                }
            }

            m_mousePosition = mouseEvent->_position;
            m_mouseWheel = mouseEvent->_wheel;

            for (std::vector<MouseCallback>::const_iterator iter = m_mouseSignatures.cbegin(); iter != m_mouseSignatures.cend(); ++iter)
            {
                if ((*iter))
                {
                    (*iter)(mouseEvent);
                }
            }
            return true;
        };

        case eGamepadInputEvent:
        {
            const GamepadInputEventPtr& gamepadEvent = std::static_pointer_cast<SGamepadInputEvent>(event);

            m_gamepadStates = gamepadEvent->_buttons;

            for (std::vector<GamepadCallback>::const_iterator iter = m_gamepadSignatures.cbegin(); iter != m_gamepadSignatures.cend(); ++iter)
            {
                if ((*iter))
                {
                    (*iter)(gamepadEvent);
                }
            }
            return true;
        }

        default:
        {
            return false;
        }
    }
}

void CInputEventHandler::connectKeyboardEvent(std::function<void(const KeyboardInputEventPtr&)> signature)
{
    if (signature)
    {
        m_keyboardSignatures.push_back(signature);
    }
}

void CInputEventHandler::connectMouseEvent(std::function<void(const MouseInputEventPtr&)> signature)
{
    if (signature)
    {
        m_mouseSignatures.push_back(signature);
    }
}

void CInputEventHandler::connectGamepadEvent(std::function<void(const GamepadInputEventPtr&)> signature)
{
    if (signature)
    {
        m_gamepadSignatures.push_back(signature);
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

bool CInputEventHandler::isGamepadPressed(const EGamepadButton& code) const
{
    if (code >= (u32)eButtonCount)
    {
        return false;
    }

    return (m_gamepadStates & (1 << code)) ? true : false;
}

const core::Point2D& CInputEventHandler::getCursorPosition() const
{
    return m_mousePosition;
}

float CInputEventHandler::getMouseWheel() const
{
    return m_mouseWheel;
}