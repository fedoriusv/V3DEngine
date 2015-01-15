#ifndef _V3D_RENDER_TARGET_H_
#define _V3D_RENDER_TARGET_H_

#include "Object.h"
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

    class CRenderTarget : public CObject
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

    protected:

        virtual void            create()    = 0;
        virtual void            destroy()   = 0;

        bool                    parse(tinyxml2::XMLElement* root);

        TexturePtr              m_colorTexture;
        TexturePtr              m_depthTexture;

        core::Vector4D          m_color;
        core::Dimension2D       m_viewportSize;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CRenderTarget> RenderTargetPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_RENDER_TARGET_H_