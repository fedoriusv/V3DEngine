#ifndef _V3D_RENDER_TARGET_GL_H_
#define _V3D_RENDER_TARGET_GL_H_

#include "renderer/RenderTarget.h"

namespace v3d
{
    namespace renderer
    {
        class CRenderTargetGL : public CRenderTarget
        {
        public:

            CRenderTargetGL();
            ~CRenderTargetGL();

            void            bind()      override;

        private:

            void            create()    override;
            void            destroy()   override;



            u32             m_fboID;
        };
    }
}

#endif //_V3D_RENDER_TARGET_GL_H_