#ifndef _V3D_RENDER_TARGET_H_
#define _V3D_RENDER_TARGET_H_

#include "Target.h"
#include "ImageFormats.h"
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
    class CTexture;
    class CRenderBuffer;

    /**
    * Inherited class for render target management.
    * Used output to texture or renderbuffer
    * Next attachmens supported: color, depth and stencil
    */
    class CRenderTarget : public ITarget
    {
    public:

        CRenderTarget();
        CRenderTarget(const CRenderTarget&)             = delete;
        CRenderTarget& operator=(const CRenderTarget&)  = delete;

        ~CRenderTarget();

        const TexturePtr                getColorTexture(u32 index) const;
        TexturePtr                      getColorTexture(u32 index);
        
        const TexturePtr                getDepthTexture() const;
        TexturePtr                      getDepthTexture();

        const TexturePtr                getStencilTexture() const;
        TexturePtr                      getStencilTexture();

        void                            setColorValue(const core::Vector4D& color);
        const core::Vector4D&           getColorValue() const;

        void                            setDepthValue(f32 depth);
        f32                             getDepthValue() const;

        void                            setStencilValue(s32 stensil);
        s32                             getStencilValue() const;

        void                            setViewport(const core::Rect32& size);
        const core::Rect32&             getViewport() const;

        bool                            isClearColorBuffer() const;
        bool                            isClearDepthBuffer() const;
        bool                            isClearStencilBuffer() const;

        void                            setClearColorBuffer(bool clear);
        void                            setClearDepthBuffer(bool clear);
        void                            setClearStencilBuffer(bool clear);

        enum class EAttachmentsType
        {
            eEmptyAttach = -1,
            eColorAttach,
            eDepthAttach,
            eStencilAttach,
            eDepthStencilAttach,
        };

        enum class EAttachmentsOutput
        {
            eEmptyOutput = -1,
            eTextureOutput,
            eRenderOutput,
        };

        struct SAttachments
        {
            SAttachments();
            SAttachments(const SAttachments& attach) = delete;
            SAttachments(const SAttachments&&);
            ~SAttachments();

            SAttachments&       operator=(const SAttachments&) = delete;
            SAttachments&       operator=(const SAttachments&&);

            u32                 _index;
            bool                _active;
            EAttachmentsType    _attachmentType;
            EAttachmentsOutput  _output;
            
            EImageFormat        _format;
            EImageType          _type;

            union
            {
                Texture*           _texture;
                CRenderBuffer*     _buffer;
            };
        };

    protected:

        friend                      CRenderPass;
        friend                      CRenderTechnique;

        bool                        parse(const tinyxml2::XMLElement* root);
        bool                        formatParser(const std::string& str, EImageFormat& format, EImageType& type);

        void                        attachTarget(EAttachmentsType attach, u32 index, EImageFormat format, EImageType type, EAttachmentsOutput output, bool active = true);

        std::deque<SAttachments>    m_attachmentsList;

        core::Vector4D              m_color;
        f32                         m_depth;
        s32                         m_stencil;

        core::Rect32                m_viewport;

        bool                        m_MSAA;

        bool                        m_clearColorBuffer;
        bool                        m_clearDepthBuffer;
        bool                        m_clearStencilBuffer;

    private:

        static const std::string    k_defaultName;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CRenderTarget>          RenderTargetPtr;
    typedef std::weak_ptr<CRenderTarget>            RenderTargetWPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_V3D_RENDER_TARGET_H_
