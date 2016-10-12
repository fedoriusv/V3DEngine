#include "Platform.h"
#include "utils/Logger.h"

#ifdef _PLATFORM_WIN_
#   include "platform/WindowWinApi.h"
#endif //_PLATFORM_WIN_

#ifdef _PLATFORM_MACOSX_
#   include "platform/WindowMacOSX.h"
#endif //_PLATFORM_MACOSX_

#ifdef _PLATFORM_LINUX_
#   include "platform/WindowLinux.h"
#endif //_PLATFORM_LINUX_

#ifdef _OPENGL_RENDER_
#   include "context/DeviceContextGL.h"
#   include "renderer/GL/RendererGL.h"
#endif //_OPENGL_RENDER_

#ifdef _DIRECT3D_RENDER_
#   include "context/DeviceContextD3D.h"
#   include "renderer/D3D/RendererD3D.h"
#endif //_DIRECT3D_RENDER_

#ifdef _VULKAN_RENDER_
#   include "context/DeviceContextVK.h"
#   include "renderer/VK/RendererVK.h"
#endif //_VULKAN_RENDER_


namespace v3d
{
namespace platform
{

using namespace renderer;
using namespace core;

WindowPtr Platform::createWindow(const core::Dimension2D & size, const core::Point2D & pos, bool isFullscreen, bool isResizeble)
{
    WindowPtr window = nullptr;

    WindowParam param;
    param._size = size;
    param._position = pos;
    param._isFullscreen = isFullscreen;
    param._isResizeble = isFullscreen ? false : isResizeble;

#ifdef _PLATFORM_WIN_
    window = std::make_shared<WindowWinApi>(param);
#elif _PLATFORM_MACOSX_
    window = std::make_shared<CWindowMacOSX>(param);
#elif _PLATFORM_LINUX_
    window = std::make_shared<CWindowLinux>(param);
#endif //_PLATFORM_LINUX_

    if (!window)
    {
        LOG_ERROR("Platform::createWindow: Windows is't allocated");
        return nullptr;
    }

    window->create();

    window->setResizeble(param._isResizeble);
    window->setFullScreen(param._isFullscreen);
    window->setPosition(param._position);

    return window;
}

renderer::ContextPtr Platform::createContext(const platform::WindowPtr window, ERenderType driverType)
{
    renderer::ContextPtr context = nullptr;
    switch (driverType)
    {
        case ERenderType::eRenderOpenGL:
        {
#ifdef _OPENGL_RENDER_
            context = std::make_shared<gl::DeviceContextGL>(window);
#else //_OPENGL_RENDER_
            LOG_ERROR("Platform::createContext: _OPENGL_RENDER_ not defined");
#endif //_OPENGL_RENDER_
            break;
        }

        case ERenderType::eRenderDirect3D:
        {
#ifdef _DIRECT3D_RENDER_
            context = std::make_shared<d3d::DeviceContextD3D>(window);
#else //_DIRECT3D_RENDER_
            LOG_ERROR("Platform::createContext: _DIRECT3D_RENDER_ not defined");
#endif //_DIRECT3D_RENDER_
            break;
        }

        case ERenderType::eRenderVulkan:
        {
#ifdef _VULKAN_RENDER_
            context = std::make_shared<vk::DeviceContextVK>(window);
#else //_VULKAN_RENDER_
            LOG_ERROR("Platform::createContext: _VULKAN_RENDER_ not defined");
#endif //_VULKAN_RENDER_
            break;
        }

        default:
        {
            LOG_ERROR("Platform::createContext: driverType is unkown");
            return nullptr;
        }
    }

    if (!context)
    {
        LOG_ERROR("Platform::createContext: driverType is nullptr");
        return nullptr;
    }

    if (!context->create())
    {
        LOG_ERROR("Platform::createContext: Create context is Failed");
        context->destroy();
        return nullptr;
    }

    context->fillGrapthicCaps();

    return context;
}

renderer::RendererPtr Platform::createRenderer(const renderer::ContextPtr context, ERenderType driverType)
{
    RendererPtr renderer = nullptr;
    switch (driverType)
    {
        case ERenderType::eRenderOpenGL:
        {
#ifdef _OPENGL_RENDER_
            renderer = std::make_shared<gl::RendererGL>(context);
#else //_OPENGL_RENDER_
            LOG_ERROR("Platform::createRenderer: _OPENGL_RENDER_ not defined");
#endif //_OPENGL_RENDER_
            break;
        }

        case ERenderType::eRenderDirect3D:
        {
#ifdef _DIRECT3D_RENDER_
            renderer = std::make_shared<d3d::RendererD3D>(context);
#else //_DIRECT3D_RENDER_
            LOG_ERROR("Platform::createRenderer: _DIRECT3D_RENDER_ not defined");
#endif //_DIRECT3D_RENDER_
           break;
        }

        case ERenderType::eRenderVulkan:
        {
#ifdef _VULKAN_RENDER_
            renderer = std::make_shared<vk::RendererVK>(context);
#else //_VULKAN_RENDER_
            LOG_ERROR("Platform::createRenderer: _VULKAN_RENDER_ not defined");
#endif //_VULKAN_RENDER_
            break;
        }

        default:
        {
            return nullptr;
        }
    }

    if (!renderer->create())
    {
        LOG_ERROR("Platform::createRenderer: create render is filed");
        return nullptr;
    }

    return renderer;
}

} //namespace platform
} //namespace v3d
