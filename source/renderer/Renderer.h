#ifndef _F3D_RENDERER_H_
#define _F3D_RENDERER_H_

#include "Singleton.h"
#include "context/DriverContext.h"

namespace f3d
{
namespace renderer
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	class CRenderer : public Singleton<CRenderer>
	{
	public:

								CRenderer(const CDriverContextPtr& context);
		virtual					~CRenderer();

		virtual void			init()                         = 0;

		virtual void			preRender()                    = 0;
		virtual void			postRender()                   = 0;

		virtual void			reshape(u32 width, u32 height) = 0;

		void					setBackColor(const core::Vector3D& color);
		const core::Vector3D&	getBackColor() const;

		void					checkForErrors(const std::string& location = "");

	protected:

		CDriverContextPtr		m_context;
		
		core::Vector3D			m_backColor;
		core::Dimension2D		m_viewportSize;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	typedef std::shared_ptr<CRenderer>	CRendererPtr;

	/////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_F3D_RENDERER_H_