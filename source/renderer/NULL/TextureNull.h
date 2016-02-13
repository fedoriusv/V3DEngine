#ifndef _V3D_TEXTURE_NULL_H_
#define _V3D_TEXTURE_NULL_H_

#include "renderer/Texture.h"

namespace v3d
{
namespace renderer
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    class CTextureNull : public CTexture
    {
    public:

        CTextureNull();
        ~CTextureNull();

        void            bind(u32 layer, u32 sampler)    override;
        void            unbind(u32 layer, u32 sampler)  override;
        bool            create()                        override;
        void            destroy()                       override;

    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_V3D_TEXTURE_NULL_H_
