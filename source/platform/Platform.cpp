#include "Platform.h"
#include "utils/Logger.h"
#include "renderer/GL/RendererGL.h"

#ifdef _PLATFORM_WIN_
#	include "platform/WindowWin32.h"
#endif

#ifdef _PLATFORM_MACOSX_
#	include "platform/WindowMacOSX.h"
#endif

#ifdef _PLATFORM_LINUX_
#	include "platform/PlatformLinux.h"
#endif

#ifdef _OPENGL_DRIVER_
#	include "context/DriverContextGL.h"
#endif


using namespace f3d;
using namespace f3d::platform;
using namespace f3d::renderer;

CPlatform::CPlatform()
{
}

CPlatform::~CPlatform()
{
}

CWindowPtr CPlatform::createWindowWithContext(const core::Dimension2D& size, const core::Dimension2D& pos,
	bool isFullscreen, bool isResizeble, EDriverType driverType )
{
	CWindowPtr window = nullptr;

	WindowParam param;
	param.size         = size;
	param.position     = pos;
	param.isFullscreen = isFullscreen;
	param.isResizeble  = isFullscreen ? false : isResizeble;
	param.driverType   = driverType;

#ifdef _PLATFORM_WIN_
	window = std::make_shared<CWindowWin32>(CWindowWin32(param));
#endif

#ifdef _PLATFORM_MACOSX_
	window = std::make_shared<CWindowMacOSX>(CWindowMacOSX(param));
#endif

#ifdef _PLATFORM_LINUX_
	window = new CPlatformLinux(param);
#endif

	window->create();

	window->setResizeble(param.isResizeble);
	window->setFullScreen(param.isFullscreen);

	renderer::CDriverContextPtr driver = nullptr;
	
	switch (param.driverType)
	{
		case EDriverType::eDriverOpenGL:
		{
			driver = std::make_shared<CDriverContextGL>(CDriverContextGL(window));
		}
		break;

		case EDriverType::eDriverDirect3D:
		{
			//driver = new renderer::CDriverContextD3D(this);
		}
		break;

		default:
		{
			window->close();
			system("pause");

			return nullptr;
		}
		break;
	}

	if (!driver->createContext())
	{
		LOG_ERROR("Error create context")

		driver = nullptr;
		window->close();
		system("pause");

		return nullptr;
	}

	m_window = window;
	m_renderer = CPlatform::createRenderer(driver, param);
	if (!m_renderer)
	{
		LOG_ERROR("Error create Renderer")

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

renderer::CRendererPtr CPlatform::createRenderer(const CDriverContextPtr& context, const WindowParam& param)
{
	CRendererPtr renderer = nullptr;

	if (!context)
	{
		return nullptr;
	}

	context->driverInfo();

	switch (param.driverType)
	{
		case EDriverType::eDriverOpenGL:
		{
			renderer = std::make_shared<CRendererGL>(CRendererGL(context));
		}
		break;

		case EDriverType::eDriverDirect3D:
		{
			//renderer = std::make_shared<CRenderer>(CRendererD3D());
		}
		break;

		default:
		{
			return nullptr;
		}
	}

	return renderer;
}
