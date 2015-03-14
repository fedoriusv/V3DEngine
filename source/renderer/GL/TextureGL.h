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
        ~CTextureGL();

        void            bind(u32 layer)   override;
        void            unbind(u32 layer) override;
        bool            create()          override;
        void            destroy()         override;

        static void     reset();

    private:

        void            copyToTexture2D(const Dimension2D& offset, const Dimension2D& size, EImageFormat format, void* data) override;

        void            initTexture1D(u32 texture);
        void            initTexture2D(u32 texture);
        void            initTexture3D(u32 texture);
        void            initTextureCubeMap(u32 texture);

        static void     genTexture(u32& texture);
        static void     deleteTexture(u32 texture);
        static void     bindTexture(ETextureTarget target, u32 texture);
        static void     activeTextureLayer(u32 layer);

        static void     genSampler(u32& sampler);
        static void     deleteSampler(u32 sampler);
        static void     bindSampler(u32 texture, u32 sampler);
        static void     wrapSampler(u32 sampler, EWrapType wrap);
        static void     filterSampler(u32 sampler, ETextureFilter min, ETextureFilter mag);
        static void     anisotropicSampler(u32 sampler, u32 level);
        static void     generateMipmap(ETextureTarget target);

        u32             m_samplerID;

        static u32      s_currentTextureID[eTargetCount];
        static u32      s_currentLayerID;
        static u32      s_currentSamplerID;
   };

    //////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_TEXTURE_GL_H_
