#ifndef _F3D_EVENT_HANDLER_H_
#define _F3D_EVENT_HANDLER_H_

#include "common.h"
#include "event/InputEvents.h"

namespace f3d
{
namespace event
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	class CEventHandler
	{
	public:
		CEventHandler();
		virtual					~CEventHandler();

		virtual void			update();
		void					pushEvent(const SInputEventPtr event);

	private:

		virtual bool			onEvent(const SInputEventPtr event) = 0;

		std::queue<const SInputEventPtr> m_events;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_F3D_EVENT_HANDLER_H_