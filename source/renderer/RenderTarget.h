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

        void                            setClearDepth(f32 depth);
        f32                             getClearDepth() const;

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
            eDepthStencilAttach,
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
            EAttachmentsType    _attachmentType;
            EAttachmentsOutput  _output;
            
            EImageFormat        _format;
            EImageType          _type;

            CTexture*           _texture;
            u32                 _bufferId; //TODO: need rework to object
        };

    protected:

        friend                      CRenderPass;
        friend                      CRenderTechnique;

        bool                        parse(const tinyxml2::XMLElement* root);
        bool                        formatParser(const std::string& str, EImageFormat& format, EImageType& type);

        void                        attachTarget(EAttachmentsType attach, u32 index, EImageFormat format, EImageType type, EAttachmentsOutput output);

        std::deque<SAttachments>    m_attachmentsList;

        core::Vector4D              m_color;
        f32                         m_depth;
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
