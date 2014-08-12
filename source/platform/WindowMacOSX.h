#ifndef _V3D_PLATFORM_MAC_OSX_H_
#define _V3D_PLATFORM_MAC_OSX_H_

#include "Window.h"

namespace v3d
{
namespace platform
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	class CWindowMacOSX : public CWindow
	{
	public:
        
        CWindowMacOSX(const WindowParam& param);
		virtual			~CWindowMacOSX();
		
		void            minimize()                                override;
		void            maximize()                                override;
		void            restore()                                 override;
		void            setFullScreen( bool value = true )        override;
		void            setResizeble( bool value = true )         override;
		void            setCaption( const std::string& text )     override;
		void            setPosition(const core::Dimension2D& pos) override;
        
		bool            isMaximized()     const                   override;
		bool            isMinimized()     const                   override;
		bool            isActive()        const                   override;
		bool            isFocused()       const                   override;
        
		bool            begin()                                   override;
		bool            end()                                     override;
        
	protected:
        
		void            create()                                  override;
		void            close()                                   override;
        
//        NSAutoreleasePool*          m_autoreleasePool;

	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif