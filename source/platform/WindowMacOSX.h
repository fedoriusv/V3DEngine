#ifndef _V3D_PLATFORM_MAC_OSX_H_
#define _V3D_PLATFORM_MAC_OSX_H_

#include "Window.h"

namespace v3d
{
namespace platform
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    
    typedef struct SAppNS
    {
        void*               _eventSource;
        id                  _autoreleasePool;
        id                  _delegate;
    } AppNS;
    
    typedef struct SWindowNS
    {
        id                  _window;
        id                  _delegate;
        id                  _view;
    } WindowNS;

    
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
        const core::Point2D&    getPosition()     const                     override;
        void                    getCursorPosition(core::Point2D& pos) const override;
        
		bool                    begin()                                     override;
		bool                    end()                                       override;
        
        const WindowNS&         getHandleWindow() const;
        
	protected:
        
		void                    create()                                    override;
		void                    close()                                     override;
        
        void                    addKeyCodes()                               override;
        
        AppNS                   m_app;
        WindowNS                m_window;
        
        bool                    m_close;
        
       
        bool                    init();
        bool                    initAppKit();
        void                    terminate();

	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif