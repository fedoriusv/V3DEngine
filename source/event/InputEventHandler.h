#ifndef _V3D_INPUT_EVENT_HANDLER_H_
#define _V3D_INPUT_EVENT_HANDLER_H_

#include "common.h"
#include "EventReceiver.h"
#include "InputEventKeyboard.h"
#include "InputEventMouse.h"
#include "InputEventGamepad.h"

namespace v3d
{
namespace event
{

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CInputEventHandler : public IEventReceiver, public std::enable_shared_from_this<CInputEventHandler>
    {

    public:

        CInputEventHandler();
        ~CInputEventHandler();

        void                            connectKeyboardEvent(std::function<void(const KeyboardInputEventPtr&)>);
        void                            connectMouseEvent(std::function<void(const MouseInputEventPtr&)>);
        void                            connectGamepadEvent(std::function<void(const GamepadInputEventPtr&)>);

        void                            update();

        bool                            isKeyPressed(const EKeyCode& code)              const;

        bool                            isLeftMousePressed()                            const;
        bool                            isRightMousePressed()                           const;
        bool                            isMiddleMousePressed()                          const;

        bool                            isGamepadPressed(const EGamepadButton& code)    const;

        const core::Point2D&            getCursorPosition()                             const;
        float                           getMouseWheel()                                 const;

        void                            setEnableEvents(bool enable);

    private:

        bool                            onEvent(const SInputEventPtr& event)            override;
        void                            resetKeyPressed();

        bool                            m_keysPressed[eKey_Codes_Count];
        bool                            m_mouseStates[eMouseCount];
        u32                             m_gamepadStates;

        core::Point2D                   m_mousePosition;
        float                           m_mouseWheel;

        typedef std::function<void(const KeyboardInputEventPtr&)>   KeyboardCallback;
        typedef std::function<void(const MouseInputEventPtr&)>      MouseCallback;
        typedef std::function<void(const GamepadInputEventPtr&)>    GamepadCallback;

        std::vector<KeyboardCallback>   m_keyboardSignatures;
        std::vector<MouseCallback>      m_mouseSignatures;
        std::vector<GamepadCallback>    m_gamepadSignatures;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CInputEventHandler>             InputEventHandlerPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_INPUT_EVENT_HANDLER_H_
