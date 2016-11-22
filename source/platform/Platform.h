#pragma once

#include "Window.h"
#include "context/DeviceContext.h"
#include "renderer/Renderer.h"

namespace v3d
{
namespace platform
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class Platform
    {
    public:

        static WindowPtr                createWindow(
                                            const core::Dimension2D& size = core::Dimension2D(800U, 600),
                                            const core::Point2D& pos = core::Point2D(100U, 100U),
                                            bool isFullscreen = false,
                                            bool isResizeble = false);

        static renderer::ContextPtr     createContext(
                                            const platform::WindowPtr window,
                                            renderer::ERenderType driverType);

        static renderer::RendererPtr    createRenderer(
                                            const renderer::ContextPtr,
                                            renderer::ERenderType type = renderer::ERenderType::eRenderOpenGL);
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace platform
} //namespace v3d
