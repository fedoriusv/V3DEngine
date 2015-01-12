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

        void                        connectKeyboardEvent(std::function<void(const KeyboardInputEventPtr&)>);
        void                        connectMouseEvent(std::function<void(const MouseInputEventPtr&)>);
        void                        connectGamepadEvent(std::function<void(const GamepadInputEventPtr&)>);

        void                        update();

        bool                        isKeyPressed(const EKeyCode& code)              const;

        bool                        isLeftMousePressed()                            const;
        bool                        isRightMousePressed()                           const;
        bool                        isMiddleMousePressed()                          const;

        bool                        isGamepadPressed(const EGamepadButton& code)    const;

        const core::Dimension2D&    getCursorPosition()                             const;
        float                       getMouseWheel()                                 const;

        void                        setEnableEvents(bool enable);

    private:

        bool                        onEvent(const SInputEventPtr& event)            override;
        void                        resetKeyPressed();

        bool                        m_keysPressed[eKey_Codes_Count];
        bool                        m_mouseStates[eMouseCount];
        u32                         m_gamepadStates;

        core::Dimension2D           m_mousePosition; //WARN: need Point class
        float                       m_mouseWheel;

        std::function<void(const KeyboardInputEventPtr&)>  m_keyboardSignature;
        std::function<void(const MouseInputEventPtr&)>     m_mouseSignature;
        std::function<void(const GamepadInputEventPtr&)>   m_gamepadSignature;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CInputEventHandler>             InputEventHandlerPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_INPUT_EVENT_HANDLER_H_
