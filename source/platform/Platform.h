#ifndef _F3D_PLATFORM_H_
#define _F3D_PLATFORM_H_

#include "Window.h"
#include "renderer/Renderer.h"

namespace f3d
{
namespace platform
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	class CPlatform
	{
	public:

									CPlatform();
		virtual						~CPlatform();

		CWindowPtr					createWindowWithContext(
										const core::Dimension2D& size = core::Dimension2D(800U, 600),
										const core::Dimension2D& pos  = core::Dimension2D(0U, 0U),
										bool isFullscreen             = false,
										bool isResizeble              = false,
										EDriverType driverType        = EDriverType::eDriverOpenGL);

		bool						hasError()    const;
		void						closeWindow() const;

		CWindowPtr					getWindow()   const;
		renderer::CRendererPtr		getRenderer() const;

		const platform::EDriverType	getDriverType() const;
	
		bool						init();
		bool						begin();
		bool						end();

	private:

		renderer::CRendererPtr		createRenderer(const renderer::CDriverContextPtr& driver, const WindowParam& param);

		CWindowPtr					m_window;
		renderer::CRendererPtr		m_renderer;
		
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	typedef std::shared_ptr<CPlatform>	CPlatformPtr;

	/////////////////////////////////////////////////////////////////////////////////////////////////////
}
}
#endif //_F3D_PLATFORM_H_