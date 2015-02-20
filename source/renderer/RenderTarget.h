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
        virtual                 ~CRenderTarget();

        virtual void            bind() = 0;

        const TexturePtr&       getColorTexture() const;
        TexturePtr&             getColorTexture();
        
        const TexturePtr&       getDepthTexture() const;
        TexturePtr&             getDepthTexture();

        void                    setClearColor(const core::Vector4D& color);
        const core::Vector4D&   getCearColor() const;

        void                    setViewportSize(const core::Dimension2D& size);
        const core::Dimension2D& getViewportSize() const;

        void                    setClearColorBuffer(bool clear);
        bool                    getClearColorBuffer() const;
        bool                    hasClearColorTarget() const;

        void                    setClearDepthBuffer(bool clear);
        bool                    getClearDepthBuffer() const;
        bool                    hasClearDepthTarget() const;

        EImageFormat            getImageFormat()    const;
        EImageType              getImageType()      const;

    protected:

        friend                  CRenderPass;

        virtual bool            create()    = 0;
        virtual void            destroy()   = 0;

        void                    setColorTexture(const TexturePtr& texture);
        void                    setDepthTexture(const TexturePtr& texture);

        bool                    parse(const tinyxml2::XMLElement* root);

        TexturePtr              m_colorTexture;
        TexturePtr              m_depthTexture;

        core::Vector4D          m_color;
        std::string             m_name;

        bool                    m_clearColorBuffer;
        bool                    m_clearDepthBuffer;

        bool                    m_hasClearColor;
        bool                    m_hasClearDepth;

        s32                     m_depthSize;

        EImageFormat            m_imageFormat;
        EImageType              m_imageType;


        core::Dimension2D       m_viewportSize;

        static RenderTargetPtr  s_default;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CRenderTarget> RenderTargetPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_RENDER_TARGET_H_