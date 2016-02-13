#ifndef _V3D_TEXTURE_GL_H_
#define _V3D_TEXTURE_GL_H_

#include "renderer/Texture.h"

namespace v3d
{
namespace renderer
{
namespace gl
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    class BufferGL;

    /**
    * Inherited class for texture management. GL render only.
    */
    class CTextureGL : public CTexture
    {
    public:

        explicit CTextureGL(bool useTextureBuffer = false);
        ~CTextureGL();

        void            bind(u32 layer, u32 sampler)                                                                                override;
        void            unbind(u32 layer, u32 sampler)                                                                              override;
        bool            create()                                                                                                    override;
        void            destroy()                                                                                                   override;

        bool            isValid()                                                                                                   override;

        void            updateData(u32 offset, u32 size, void* data)                                                                override;
        void            updateData(const core::Dimension2D& offset, const core::Dimension2D& size, void* data, u32 cubemapSide = 0) override;
        void            updateData(const core::Dimension3D& offset, const core::Dimension3D& size, void* data)                      override;

        void            readData(void* data, u32 cubemapSide = 0)                                                                   override;

        void            setFilterType(ETextureFilter min, ETextureFilter mag)                                                       override;
        void            setWrap(EWrapType wrap)                                                                                     override;
        void            setAnisotropicLevel(EAnisotropic level)                                                                     override;

        void            fill(u32 offset = 0U, u32 size = 0U, void* data = nullptr)                                                                                                   override;
        void            fill(const core::Dimension2D& offset = core::Dimension2D(0U, 0U), const core::Dimension2D& size = core::Dimension2D(0U, 0U), void* data = nullptr)           override;
        void            fill(const core::Dimension3D& offset = core::Dimension3D(0U, 0U, 0U), const core::Dimension3D& size = core::Dimension3D(0U, 0U, 0U), void* data = nullptr)   override;

        u32             getTextureID() const;
        u32             getSamplerID() const;

        static void     reset();

    public:

        DEPRECATED void copyToTexture2D(const core::Dimension2D& offset, const core::Dimension2D& size, EImageFormat format, void* data) override;

        static bool     bindTexture(ETextureTarget target, u32 texture);
        static void     bindTexBuffer(u32 format, u32 texture, u32 buffer, u32 offset = 0, u32 size = 0);
        static bool     activeTextureLayer(u32 layer);

        static bool     bindSampler(u32 texture, u32 sampler);
        static void     wrapSampler(u32 sampler, EWrapType wrap);
        static void     filterSampler(u32 sampler, ETextureFilter min, ETextureFilter mag);
        static void     anisotropicSampler(u32 sampler, u32 level);

    private:

        u32             m_textureID;
        u32             m_samplerID;

        BufferGL*       m_textureBuffer;

        bool            m_initialized;
        const u32       m_mipmapLevel;

        static u32      s_currentTextureID[eTargetCount];
        static u32      s_currentLayerID;
        static u32      s_currentSamplerID;
   };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace gl
} //namespace renderer
} //namespace v3d

#endif //_V3D_TEXTURE_GL_H_
