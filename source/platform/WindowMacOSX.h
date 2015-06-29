#ifndef _V3D_PLATFORM_MAC_OSX_H_
#define _V3D_PLATFORM_MAC_OSX_H_

#include "Window.h"

@class NSAutoreleasePool;

namespace v3d
{
namespace platform
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	class CWindowMacOSX : public CWindow
	{
	public:
        
        CWindowMacOSX(const WindowParam& param);
		virtual                 ~CWindowMacOSX();
		
		void                    minimize()                                  override;
		void                    maximize()                                  override;
		void                    restore()                                   override;
		void                    setFullScreen( bool value = true )          override;
		void                    setResizeble( bool value = true )           override;
		void                    setCaption( const std::string& text )       override;
        void                    setPosition(const core::Point2D& pos)       override;
        void                    setCursorPosition(const core::Point2D& pos) override;
        
		bool                    isMaximized()     const                     override;
		bool                    isMinimized()     const                     override;
		bool                    isActive()        const                     override;
		bool                    isFocused()       const                     override;
        const core::Point2D&    getPosition()                               override;
        void                    getCursorPosition(core::Point2D& pos)       override;
        
		bool                    begin()                                     override;
		bool                    end()                                       override;
        
	protected:
        
		void                    create()                                    override;
		void                    close()                                     override;
        
        void                    addKeyCodes()                               override;
        
        NSAutoreleasePool*      m_autoreleasePool;
        id                      m_window;

	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif