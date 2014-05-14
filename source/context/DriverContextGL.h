#ifndef _F3D_DRIVER_CONTEXT_GL_H_
#define _F3D_DRIVER_CONTEXT_GL_H_

#include "DriverContext.h"

namespace f3d
{
namespace renderer
{

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	class CDriverContextGL : public CDriverContext
	{
	public:

						CDriverContextGL(const platform::CWindowPtr& window);
		virtual			~CDriverContextGL();

		virtual void	driverInfo();
		virtual void	checkForErrors(const std::string& location = "");
		virtual bool	createContext();

	protected:

#if defined(_PLATFORM_WIN_)
		bool			createWin32Context();
#elif defined(_PLATFORM_LINUX_)
		bool			createLinuxContext();
#endif
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_F3D_DRIVER_CONTEXT_GL_H_