#ifndef _V3D_PLATFORM_LINUX_H_
#define _V3D_PLATFORM_LINUX_H_

#include "Platform.h"

#include <X11/Xlib.h>
#include <GL/glx.h>

namespace v3d
{
namespace platform
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	class CPlatformLinux : public CPlatform
	{
	public:
						CPlatformLinux( const PlatformParam& param );
		virtual			~CPlatformLinux();

		virtual void	minimizeWindow();
		virtual void	maximizeWindow();
		virtual void	restoreWindow();
		virtual void	setFullScreen( bool value = false );
		virtual void	setResizeble ( bool value = true );
		virtual void	setWindowCaption( const std::string& text );

		virtual bool	isWindowMaximized() const;
		virtual bool	isWindowMinimized() const;
		virtual bool	isWindowActive()    const;
		virtual bool	isWindowFocused()   const;

		virtual bool	begin();
		virtual bool	end();

		Window			getWidow()         const;
		Display*		getDisplay()       const;
		GLXFBConfig		getFBConfig()      const;
		XVisualInfo*	getVisualInfo()    const;

	protected:

		void			createWindows();
		void			closeWindow();

		Display*		m_display;
		Window			m_window;
		int				m_screen;

		GLXFBConfig		m_glxFBConfigs;
		GLXWindow		m_glxWindow;
		GLXContext		m_glxContext;
		XVisualInfo*	m_visualInfo;

	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif
