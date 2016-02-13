#ifndef _V3D_TEXTURE_D3D_H_
#define _V3D_TEXTURE_D3D_H_

#include "renderer/Texture.h"

namespace v3d
{
namespace renderer
{
namespace d3d
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * Inherited class for texture management. D3D render only.
    */
    class CTextureD3D : public CTexture
    {
    public:

        CTextureD3D();
        ~CTextureD3D();

        void            bind(u32 layer, u32 sampler)                                                                                override;
        void            unbind(u32 layer, u32 sampler)                                                                              override;
        bool            create()                                                                                                    override;
        void            destroy()                                                                                                   override;
        bool            isValid()                                                                                                   override;

        void            updateData(u32 offset, u32 size, void* data)                                                                override;
        void            updateData(const core::Dimension2D& offset, const core::Dimension2D& size, void* data, u32 cubemapSide = 0) override;
        void            updateData(const core::Dimension3D& offset, const core::Dimension3D& size, void* data)                      override;

        void            readData(void* data, u32 cubemapSide = 0)                                                                   override;

        void            fill(u32 offset = 0U, u32 size = 0U, void* data = nullptr)                                                                                                   override;
        void            fill(const core::Dimension2D& offset = core::Dimension2D(0U, 0U), const core::Dimension2D& size = core::Dimension2D(0U, 0U), void* data = nullptr)           override;
        void            fill(const core::Dimension3D& offset = core::Dimension3D(0U, 0U, 0U), const core::Dimension3D& size = core::Dimension3D(0U, 0U, 0U), void* data = nullptr)   override;

    protected:

        DEPRECATED void copyToTexture2D(const core::Dimension2D& offset, const core::Dimension2D& size, EImageFormat format, void* data) override;
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace d3d
} //namespace renderer
} //namespace v3d

#endif //_V3D_TEXTURE_D3D_H_
