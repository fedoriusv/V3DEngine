#ifndef _F3D_RENDERER_H_
#define _F3D_RENDERER_H_

#include "common.h"

namespace f3d
{
namespace renderer
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	class CDriverContext;

	class CRenderer
	{
	public:

								CRenderer(CDriverContext* context);
		virtual					~CRenderer();

		virtual void			init()                         = 0;

		virtual void			preRender()                    = 0;
		virtual void			postRender()                   = 0;

		virtual void			reshape(u32 width, u32 height) = 0;

		void					setBackColor(const core::Vector3D& color);
		const core::Vector3D&	getBackColor() const;

	protected:

		CDriverContext*			m_context;
		
		core::Vector3D			m_backColor;
		core::Dimension2D		m_viewportSize;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_F3D_RENDERER_H_