#ifndef _V3D_RENDER_TARGET_GL_H_
#define _V3D_RENDER_TARGET_GL_H_

#include "renderer/RenderTarget.h"

namespace v3d
{
namespace renderer
{
namespace gl
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * Inherited class for render target management. GL render only.
    * Used output to texture or renderbuffer
    */
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

    private:

        bool            create()    override;
        void            destroy()   override;

        void            createRenderToRenderbuffer(SAttachments& attach, const core::Rect32& rect);
        void            createRenderToTexture(SAttachments& attach, const core::Rect32& rect);
        void            copyToRenderbuffer(const RenderTargetPtr& dst);

        enum EFramebufferTarget
        {
            eFBTargetWrite,
            eFBTargetRead,

            eFBTargetCount
        };

        u32             m_frameBufferId;

        u32             m_lastFrameIndex;

        bool            m_hasClearColor;
        bool            m_hasClearDepth;
        bool            m_hasClearStencil;

        std::vector<u32> m_attachBuffers;

        bool            m_initialized;

        static bool     bindFramebuffer(u32 buffer, EFramebufferTarget target = eFBTargetWrite);

        static void     framebufferTexture2D(s32 attachment, s32 target, u32 texture);
        static void     framebufferRenderbuffer(s32 attachment, u32 buffer);

        static void     blitFramebuffer(const core::Rect32& src, const core::Rect32& dst, u32 mask, u32 filter);

        static u32      s_currentFBO[eFBTargetCount];
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace gl
} //namespace renderer
} //namespace v3d

#endif //_V3D_RENDER_TARGET_GL_H_
