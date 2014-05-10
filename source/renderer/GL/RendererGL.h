#ifndef _F3D_RENDERER_GL_H_
#define _F3D_RENDERER_GL_H_

#include "renderer/Renderer.h"

namespace f3d
{
	namespace renderer
	{
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		class CRendererGL : public CRenderer
		{
		public:

							CRendererGL(CDriverContext* context);
			virtual			~CRendererGL();

			void			init()                         override;

			void			preRender()                    override;
			void			postRender()                   override;

			void			reshape(u32 width, u32 height) override;

		private:

			core::Matrix4D	m_projectionMatrix;
			core::Matrix4D	m_viewMatrix;
		};

		/////////////////////////////////////////////////////////////////////////////////////////////////////
	}
}

#endif //_F3D_RENDERER_GL_H_