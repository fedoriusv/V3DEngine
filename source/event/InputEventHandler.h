#ifndef _F3D_INPUT_EVENT_HANDLER_H_
#define _F3D_INPUT_EVENT_HANDLER_H_

#include "common.h"
#include "event/EventHandler.h"
#include "event/InputEvents.h"

namespace f3d
{
namespace event
{

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	class CInputEventHandler : public CEventHandler
	{

	public:

		CInputEventHandler();
		~CInputEventHandler();

		void						connectKeyboardEvent(std::function<void(const SKeyboardInputEventPtr&)>);
		void						connectMouseEvent(std::function<void(const SMouseInputEventPtr&)>);

		void						update()                               override;

		bool						isKeyPressed(const EKeyCode& code)     const;

		bool						isLeftMousePressed()                   const;
		bool						isRightMousePressed()                  const;
		bool						isMiddleMousePressed()                 const;

		const core::Dimension2D&	getCursorPosition()                    const;
		float						getMouseWheel()                        const;

	private:

		bool						onEvent(const SInputEventPtr event)    override;
		void						resetKeyPressed();

		bool						m_keysPressed[eKey_Codes_Count];
		bool						m_mouseStates[eMouseCount];

		core::Dimension2D			m_mousePosition; //WARN: need Point class
		float						m_mouseWheel;

		std::function<void(const SKeyboardInputEventPtr&)>	m_keyboardSignature;
		std::function<void(const SMouseInputEventPtr&)>	m_mouseSignature;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	typedef std::shared_ptr<CInputEventHandler>		InputEventHandlerPtr;

	/////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_F3D_INPUT_EVENT_HANDLER_H_