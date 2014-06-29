#ifndef _V3D_TEXTURE_GL_H_
#define _V3D_TEXTURE_GL_H_

#include "renderer/Texture.h"

namespace v3d
{
namespace renderer
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    class CTextureGL : public CTexture
    {
    public:

        CTextureGL();
        virtual         ~CTextureGL();

        void            bind(u32 layer)   override;
        void            unbind(u32 layer) override;
        bool            create()          override;
        void            destroy()         override;

    private:


        void            initTexture2D(u32 texture);
        void            initTextureCubeMap(u32 texture);

        void            copyToTexture2D();

        void            genTexture(u32& texture);
        void            deleteTexture(u32 texture);
        void            bindTexture(u32 target, u32 texture);
        void            activeTextureLayer(u32 layer);

        void            genSampler(u32& sampler);
        void            deleteSampler(u32 sampler);
        void            bindSampler(u32 texture, u32 sampler);
        void            wrapSampler(u32 sampler, u32 wrap);
        void            filterSampler(u32 sampler, u32 min, u32 mag);
        void            anisotropicSampler(u32 sampler, u32 level);
        void            generateMipmap(u32 target);

        u32             m_samplerID;
   };

    //////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_TEXTURE_GL_H_
