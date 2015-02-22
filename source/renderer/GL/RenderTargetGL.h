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

            bool            create()    override;
            void            destroy()   override;

            u32             m_frameBufferID;
            u32             m_depthBufferID;

            static u32      m_currentFBO;
            static u32      m_currentRBO;

            u32             m_lastFrameIndex;

        private:

            static void     genFramebuffer(u32& buffer);
            static bool     bindFramebuffer(const u32 buffer);
            static void     deleteFramebuffers(u32& buffer);

            static void     genRenderbuffer(u32& buffer);
            static bool     bindRenderbuffer(const u32 buffer);
            static void     deleteRenderbuffers(u32& buffer);

            static void     framebufferTexture2D(s32 attachment, s32 target, u32 texture);
        };
    }
}

#endif //_V3D_RENDER_TARGET_GL_H_