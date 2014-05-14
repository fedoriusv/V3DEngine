#ifndef _F3D_DRIVER_CONTEXT_H_
#define _F3D_DRIVER_CONTEXT_H_

#include "common.h"
#include "platform/Window.h"

namespace f3d
{
namespace renderer
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	class CDriverContext
	{
	public:

									CDriverContext(const platform::CWindowPtr& window);
		virtual						~CDriverContext();

		virtual void				driverInfo()                                     = 0;
		virtual void				checkForErrors(const std::string& location = "") = 0;
		virtual bool				createContext()                                  = 0;

		const core::Dimension2D&	getViewport() const;
		const platform::CWindowPtr	getWindow() const;

	protected:

		const platform::CWindowPtr	m_window;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	typedef std::shared_ptr<CDriverContext> CDriverContextPtr;

	/////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_F3D_DRIVER_CONTEXT_H_
