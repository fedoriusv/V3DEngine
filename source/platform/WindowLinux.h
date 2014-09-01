#ifndef _V3D_PLATFORM_LINUX_H_
#define _V3D_PLATFORM_LINUX_H_

#include "Window.h"

#include <X11/Xlib.h>
#include <GL/glx.h>

namespace v3d
{
namespace platform
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CWindowLinux : public CWindow
    {
    public:
                CWindowLinux(const WindowParam& param);
        virtual ~CWindowLinux();

        void            minimize()                                  override;
        void            maximize()                                  override;
        void            restore()                                   override;
        void            setFullScreen(bool value = false)           override;
        void            setResizeble(bool value = true)             override;
        void            setCaption(const std::string& text)         override;
        void            setPosition(const core::Dimension2D& pos)   override;
        bool            isMaximized() const                         override;
        bool            isMinimized() const                         override;
        bool            isActive() const                            override;
        bool            isFocused() const                           override;
        bool            begin()                                     override;
        bool            end()                                       override;

        Window          getWidow()         const;
        Display*        getDisplay()       const;
        GLXFBConfig     getFBConfig()      const;
        XVisualInfo*    getVisualInfo()    const;

    protected:

        void            create()                                    override;
        void            close()                                     override;

        Display*        m_display;
        Window          m_window;
        int             m_screen;

        GLXFBConfig     m_glxFBConfigs;
        GLXWindow       m_glxWindow;
        GLXContext      m_glxContext;
        XVisualInfo*    m_visualInfo;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif
