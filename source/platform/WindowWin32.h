#ifndef _F3D_WINDOW_WIN32_H_
#define _F3D_WINDOW_WIN32_H_

#include "Window.h"
#include <windows.h>

namespace f3d
{
namespace platform
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	class CWindowWin32 : public CWindow
	{
	public:

				CWindowWin32(const WindowParam& param);
		virtual	~CWindowWin32();

		void	minimize()                                  override;
		void	maximize()                                  override;
		void	restore()                                   override;
		void	setFullScreen(bool value = false)           override;
		void	setResizeble(bool value = true)             override;
		void	setCaption(const std::string& text)         override;
		void	setPosition(const core::Dimension2D& pos)   override;

		bool	isMaximized() const                         override;
		bool	isMinimized() const                         override;
		bool	isActive()    const                         override;
		bool	isFocused()   const                         override;

		bool	begin()                                     override;
		bool	end()                                       override;

		HWND	getHandleWindow() const;

	protected:

		void	create()                                    override;
		void	close()                                     override;

		HWND	m_window;
		HDC		m_context;

	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	typedef std::shared_ptr<CWindowWin32>	CWindowWin32Ptr;

	/////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_F3D_WINDOW_WIN32_H_