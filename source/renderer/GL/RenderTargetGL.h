#ifndef _V3D_RENDER_TARGET_GL_H_
#define _V3D_RENDER_TARGET_GL_H_

#include "renderer/RenderTarget.h"

namespace v3d
{
    namespace renderer
    {
        class CRenderTargetGL : public CRenderTarget, public std::enable_shared_from_this<CRenderTargetGL>
        {
        public:

            CRenderTargetGL();
            ~CRenderTargetGL();

            void            bind()      override;
            void            unbind()    override;


            bool            hasClearColorTarget()   const;
            bool            hasClearDepthTarget()   const;
            bool            hasClearStencilTarget() const;

        public:

            static void     genFramebuffer(u32& buffer);
            static bool     bindFramebuffer(u32 buffer);
            static void     deleteFramebuffers(u32& buffer);

            static void     genRenderbuffer(u32& buffer);
            static bool     bindRenderbuffer(u32 buffer);
            static void     deleteRenderbuffers(u32& buffer);

            static void     framebufferTexture2D(s32 attachment, s32 target, u32 texture);
            static void     framebufferRenderbuffer(s32 attachment, s32 target, u32 buffer);

            static void     blitFramebuffer(const Rect& src, const Rect& dst, u32 mask);

        private:

            bool            create()    override;
            void            destroy()   override;

            void            createRenderbuffer(SAttachments& attach, const Rect& rect);
            void            createRenderToTexture(SAttachments& attach, const Rect& rect);

            u32             m_frameBufferID;
            u32             m_renderBufferID;

            static u32      s_currentFBO;
            static u32      s_currentRBO;

            u32             m_lastFrameIndex;

            bool            m_hasClearColor;
            bool            m_hasClearDepth;
            bool            m_hasClearStencil;

            std::vector<u32> m_attachBuffers;
        };
    }
}

#endif //_V3D_RENDER_TARGET_GL_H_