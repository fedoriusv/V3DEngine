#ifndef _V3D_WINDOW_H_
#define _V3D_WINDOW_H_

#include "common.h"
#include "event/InputEvents.h"

namespace v3d
{
namespace platform
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	enum class EDriverType
	{
		eDriverNull = -1,
		eDriverOpenGL,
		eDriverDirect3D
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	enum class EPlatformType
	{
		ePlatformNull = -1,
		ePlatformWin32,
		ePlatformLinux,
		ePlatformMacOSX
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	struct WindowParam
	{
		EDriverType       driverType;
		core::Dimension2D size;
		core::Dimension2D position;
		bool              isFullscreen;
		bool              isResizeble;
		bool              isVisible;
		
		WindowParam() 
			: driverType(EDriverType::eDriverNull)
			, size(core::Dimension2D(800U, 600U))
			, position(core::Dimension2D(0U, 0U))
			, isFullscreen(false)
			, isResizeble(false)
			, isVisible(true)
		{
		}
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	class CPlatform;

	class CWindow
	{
	public:
									CWindow(const WindowParam& param);
		virtual						~CWindow();
		
		virtual void				minimize()                                = 0;
		virtual void				maximize()                                = 0;
		virtual void				restore()                                 = 0;
		virtual void				setFullScreen(bool value = true)          = 0;
		virtual void				setResizeble(bool value = true)           = 0;
		virtual void				setCaption(const std::string& text)       = 0;
		virtual void				setPosition(const core::Dimension2D& pos) = 0;

		virtual bool				isMaximized()     const                   = 0;
		virtual bool				isMinimized()     const                   = 0;
		virtual bool				isActive()        const                   = 0;
		virtual bool				isFocused()       const                   = 0;
		bool						isFullscreen()    const;
		bool						isResizeble()     const;
		const core::Dimension2D&	getSize()         const;
		const core::Dimension2D&	getPosition()     const;
		const EPlatformType			getPlatformType() const;
		const EDriverType			getDriverType()   const;

		virtual bool				begin()                                  = 0;
		virtual bool				end()                                    = 0;

        const event::SKeyCodes&     getKeyCodes();

	protected:

		friend						CPlatform;

		virtual void				create()                                 = 0;
		virtual void				close()                                  = 0;

        virtual void                addKeyCodes()                            = 0;

		WindowParam					m_param;
		EPlatformType				m_platformType;

        event::SKeyCodes            m_keys;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	typedef std::shared_ptr<CWindow>	WindowPtr;

	/////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_WINDOW_H_
