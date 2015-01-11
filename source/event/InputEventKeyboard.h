#ifndef _V3D_INPUT_EVENT_KEYBOARD_H_
#define _V3D_INPUT_EVENT_KEYBOARD_H_

#include "common.h"
#include "InputEvents.h"
#include "KeyCodes.h"

namespace v3d
{
namespace event
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EKeyboardInputEvent
    {
        eKeyboardUnknown = 0,
        eKeyboardPressDown,
        eKeyboardPressUp,

        eKeyboardCount
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    struct SKeyboardInputEvent : public SInputEvent
    {
        SKeyboardInputEvent();
        virtual ~SKeyboardInputEvent();

        bool operator == (const SKeyboardInputEvent& event) const;

        v3d::c8              _character;
        EKeyCode             _key;
        EKeyboardInputEvent  _event;
        v3d::u16             _modifers;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<SKeyboardInputEvent> KeyboardInputEventPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    struct SKeyCodes
    {
    public:

        void                    add(EKeyCode code, u32 systemCode);
        EKeyCode                get(u32 code);

    private:

        std::map<u32, EKeyCode> _key;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_INPUT_EVENT_KEYBOARD_H_
