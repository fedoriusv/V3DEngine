#ifndef _V3D_RENDER_TARGET_H_
#define _V3D_RENDER_TARGET_H_

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

    class CRenderTarget
    {
    public:

        CRenderTarget();
        virtual                         ~CRenderTarget();

        virtual void                    bind()   = 0;
        virtual void                    unbind() = 0;

        const TexturePtr&               getColorTexture(u32 index) const;
        TexturePtr&                     getColorTexture(u32 index);
        
        const TexturePtr&               getDepthTexture() const;
        TexturePtr&                     getDepthTexture();

        void                            setClearColor(const core::Vector4D& color);
        const core::Vector4D&           getCearColor() const;

        void                            setViewport(const core::Rect& size);
        const core::Rect&               getViewport() const;

        bool                            getClearColorBuffer()   const;
        bool                            getClearDepthBuffer()   const;
        bool                            getclearStencilBuffer() const;

        void                            setClearColorBuffer(bool clear);
        void                            setClearDepthBuffer(bool clear);
        void                            setClearStencilBuffer(bool clear);

        const std::string&              getName() const;

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

            u32                 _index;
            EAttachmentsType    _type;
            EAttachmentsOutput  _output;
            u32                 _format;

            TexturePtr          _texture;
            u32                 _rboID;
        };

    protected:

        friend                      CRenderPass;

        virtual bool                create()    = 0;
        virtual void                destroy()   = 0;

        bool                        parse(const tinyxml2::XMLElement* root);

        std::deque<SAttachments>    m_attachmentsList;

        void                        attachTarget(EAttachmentsType type, u32 index, u32 foramt, EAttachmentsOutput output);

        core::Vector4D              m_color;
        core::Rect                  m_viewport;

        std::string                 m_name;

        bool                        m_clearColorBuffer;
        bool                        m_clearDepthBuffer;
        bool                        m_clearStencilBuffer;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CRenderTarget>          RenderTargetPtr;
    typedef std::map<std::string, RenderTargetPtr>  RenderTargetMap;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_RENDER_TARGET_H_