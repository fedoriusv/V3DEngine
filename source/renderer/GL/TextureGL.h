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

    public:

        void            copyToTexture2D(const Dimension2D& offset, const Dimension2D& size, EImageFormat format, void* data) override;

        void            initTexture1D(u32 texture);
        void            initTexture2D(u32 texture);
        void            initTexture2DMSAA(u32 texture);
        void            initTexture3D(u32 texture);
        void            initTextureCubeMap(u32 texture);

        static void     genTexture(u32& texture);
        static void     deleteTexture(u32 texture);
        static bool     bindTexture(ETextureTarget target, u32 texture);
        static bool     activeTextureLayer(u32 layer);

        static void     genSampler(u32& sampler);
        static void     deleteSampler(u32 sampler);
        static bool     bindSampler(u32 texture, u32 sampler);
        static void     wrapSampler(u32 sampler, EWrapType wrap);
        static void     filterSampler(u32 sampler, ETextureFilter min, ETextureFilter mag);
        static void     anisotropicSampler(u32 sampler, u32 level);
        static void     generateMipmap(ETextureTarget target);

    private:

        u32             m_samplerID;

        bool            m_initialized;

        static u32      s_currentTextureID[eTargetCount];
        static u32      s_currentLayerID;
        static u32      s_currentSamplerID;
   };

    //////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_TEXTURE_GL_H_
