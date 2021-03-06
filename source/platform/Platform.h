#ifndef _V3D_PLATFORM_H_
#define _V3D_PLATFORM_H_

#include "Window.h"
#include "renderer/Renderer.h"

namespace v3d
{
namespace platform
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CPlatform
    {
    public:

        CPlatform();
        virtual                     ~CPlatform();

        WindowPtr                   createWindowWithContext(
                                        const core::Dimension2D& size = core::Dimension2D(800U, 600),
                                        const core::Point2D& pos = core::Point2D(100U, 100U),
                                        bool isFullscreen = false,
                                        bool isResizeble = false,
                                        EDriverType driverType = EDriverType::eDriverOpenGL);

        bool                        hasError()      const;
        void                        closeWindow()   const;

        WindowPtr                   getWindow()     const;
        renderer::RendererPtr       getRenderer()   const;

        const platform::EDriverType getDriverType() const;

        bool                        init();
        bool                        begin();
        bool                        end();

    private:

        renderer::RendererPtr       createRenderer(const renderer::DriverContextPtr& driver, const WindowParam& param);

        WindowPtr                   m_window;
        renderer::RendererPtr       m_renderer;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CPlatform>  PlatformPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}
#endif //_V3D_PLATFORM_H_
