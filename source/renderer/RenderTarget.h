#ifndef _V3D_RENDER_TARGET_H_
#define _V3D_RENDER_TARGET_H_

#include "Target.h"
#include "Texture.h"

namespace tinyxml2
{
    class XMLElement;
}

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CRenderPass;
    class CRenderTechnique;

    /**
    * Inherited class for render target management.
    * Used output to texture or renderbuffer
    * Next attachmens supported: color, depth and stencil
    */
    class CRenderTarget : public ITarget
    {
    public:

        CRenderTarget();
        virtual                         ~CRenderTarget();

        const CTexture*                 getColorTexture(u32 index) const;
        CTexture*                       getColorTexture(u32 index);
        
        const CTexture*                 getDepthTexture() const;
        CTexture*                       getDepthTexture();

        const CTexture*                 getStencilTexture() const;
        CTexture*                       getStencilTexture();

        void                            setClearColor(const core::Vector4D& color);
        const core::Vector4D&           getClearColor() const;

        void                            setViewport(const core::Rect32& size);
        const core::Rect32&             getViewport() const;

        bool                            getClearColorBuffer()   const;
        bool                            getClearDepthBuffer()   const;
        bool                            getClearStencilBuffer() const;

        void                            setClearColorBuffer(bool clear);
        void                            setClearDepthBuffer(bool clear);
        void                            setClearStencilBuffer(bool clear);

        enum EAttachmentsType
        {
            eEmptyAttach = -1,
            eColorAttach,
            eDepthAttach,
            eStencilAttach,
        };

        enum EAttachmentsOutput
        {
            eEmptyOutput = -1,
            eTextureOutput,
            eRenderOutput,
        };

        struct SAttachments
        {
            SAttachments();
            ~SAttachments();

            u32                 _index;
            EAttachmentsType    _type;
            EAttachmentsOutput  _output;
            u32                 _format;

            CTexture*           _texture;
            u32                 _bufferId; //TODO: need rework to object
        };

    protected:

        friend                      CRenderPass;
        friend                      CRenderTechnique;

        bool                        parse(const tinyxml2::XMLElement* root);

        void                        attachTarget(EAttachmentsType type, u32 index, u32 foramt, EAttachmentsOutput output);

        std::deque<SAttachments>    m_attachmentsList;

        core::Vector4D              m_color;
        core::Rect32                m_viewport;

        bool                        m_MSAA;

        bool                        m_clearColorBuffer;
        bool                        m_clearDepthBuffer;
        bool                        m_clearStencilBuffer;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CRenderTarget>          RenderTargetPtr;
    typedef std::weak_ptr<CRenderTarget>            RenderTargetWPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_V3D_RENDER_TARGET_H_
