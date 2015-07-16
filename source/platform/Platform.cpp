#include "Platform.h"
#include "utils/Logger.h"
#include "renderer/GL/RendererGL.h"

#ifdef _PLATFORM_WIN_
#   include "platform/WindowWin32.h"
#endif

#ifdef _PLATFORM_MACOSX_
#   include "platform/WindowMacOSX.h"
#endif

#ifdef _PLATFORM_LINUX_
#   include "platform/WindowLinux.h"
#endif

#ifdef _OPENGL_DRIVER_
#   include "context/DriverContextGL.h"
#endif


using namespace v3d;
using namespace core;
using namespace platform;
using namespace renderer;

CPlatform::CPlatform()
{
}

CPlatform::~CPlatform()
{
}

WindowPtr CPlatform::createWindowWithContext(const Dimension2D& size, const Point2D& pos,
    bool isFullscreen, bool isResizeble, EDriverType driverType)
{
    WindowPtr window = nullptr;

    WindowParam param;
    param._size = size;
    param._position = pos;
    param._isFullscreen = isFullscreen;
    param._isResizeble = isFullscreen ? false : isResizeble;
    param._driverType = driverType;

#ifdef _PLATFORM_WIN_
    window = std::make_shared<CWindowWin32>(param);
#endif

#ifdef _PLATFORM_MACOSX_
    window = std::make_shared<CWindowMacOSX>(param);
#endif

#ifdef _PLATFORM_LINUX_
    window = std::make_shared<CWindowLinux>(param);
#endif

    window->create();

    window->setResizeble(param._isResizeble);
    window->setFullScreen(param._isFullscreen);
    window->setPosition(param._position);

    renderer::DriverContextPtr driver = nullptr;

    switch (param._driverType)
    {
        case EDriverType::eDriverOpenGL:
        {
            driver = std::make_shared<CDriverContextGL>(window);
            break;
        }

        case EDriverType::eDriverDirect3D:
        {
            //driver = new renderer::CDriverContextD3D(this);
            break;
        }

        default:
        {
            window->close();
            system("pause");

            return nullptr;
        }
    }

    if (!driver->createContext())
    {
        LOG_ERROR("CPlatform::createWindowWithContext: Error create context");
        
        driver->destroyContext();
        driver = nullptr;
        
        window->close();
        system("pause");

        return nullptr;
    }

    m_window = window;
    m_renderer = CPlatform::createRenderer(driver, param);
    if (!m_renderer)
    {
        LOG_ERROR("CPlatform::createWindowWithContext: Error create Renderer");
        
        driver->destroyContext();
        driver = nullptr;

        window->close();
        system("pause");

        return nullptr;
    }

    return window;
}

bool CPlatform::begin()
{
    return m_window->begin();
}

bool CPlatform::end()
{
    return m_window->end();
}

bool CPlatform::hasError() const
{
    return m_window == nullptr;
}

renderer::RendererPtr CPlatform::createRenderer(const DriverContextPtr& context, const WindowParam& param)
{
    RendererPtr renderer = nullptr;

    if (!context)
    {
        return nullptr;
    }

    context->driverInfo();

    switch (param._driverType)
    {
        case EDriverType::eDriverOpenGL:
        {
            renderer = std::make_shared<CRendererGL>(context);
            break;
        }

        case EDriverType::eDriverDirect3D:
        {
            //renderer = std::make_shared<CRenderer>(CRendererD3D());
           break;
        }

        default:
        {
            return nullptr;
        }
    }

    return renderer;
}

platform::WindowPtr CPlatform::getWindow() const
{
    return m_window;
}

RendererPtr CPlatform::getRenderer() const
{
    return m_renderer;
}

void CPlatform::closeWindow() const
{
    if (m_window)
    {
        m_window->close();
    }
}

bool CPlatform::init()
{
    if (!m_renderer)
    {
        return false;
    }

    m_renderer->init();

    return true;
}

const platform::EDriverType CPlatform::getDriverType() const
{
    return m_window->getDriverType();
}
