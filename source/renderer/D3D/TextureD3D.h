#ifndef _V3D_TEXTURE_D3D_H_
#define _V3D_TEXTURE_D3D_H_

#include "renderer/Texture.h"

namespace v3d
{
namespace renderer
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    class CTextureD3D : public CTexture
    {
    public:

        CTextureD3D();
        ~CTextureD3D();

        void            bind(u32 layer, u32 sampler)    override;
        void            unbind(u32 layer, u32 sampler)  override;
        bool            create()                        override;
        void            destroy()                       override;

    protected:

        void            copyToTexture2D(const Dimension2D& offset, const Dimension2D& size, EImageFormat format, void* data) override;
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_TEXTURE_D3D_H_
