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
    class SamplerGL;
    class CRendererGL;

    /**
    * Inherited class for texture management. GL render only.
    */
    class CTextureGL : public CTexture
    {
    public:

        CTextureGL(ETextureTarget target, EImageFormat format, EImageType type, const core::Dimension3D& size, const void* data, u32 level = 0U);
        CTextureGL(EImageFormat format, EImageType type, const core::Dimension2D& size, const void* data[6], u32 level = 0U);
        ~CTextureGL();

        void                        bind(u32 unit)  override;
        void                        unbind()        override;

        bool                        isValid()  const override;
        bool                        isEnable() const override;

        void                        update(u32 offset, u32 size, const void* data, u32 level = 0U)                                                            override;
        void                        update(const core::Dimension2D& offset, const core::Dimension2D& size, const void* data, u32 level = 0U)                  override;
        void                        update(const core::Dimension3D& offset, const core::Dimension3D& size, const void* data, u32 level = 0U)                  override;
        void                        update(u32 cubemapSide, const core::Dimension2D& offset, const core::Dimension2D& size, const void* data, u32 level = 0U) override;

        void                        read(void* data, u32 level = 0U) const                  override;
        void                        read(u32 cubemapSide, void* data, u32 level = 0U) const override;

        void                        fill(const void* data, u32 offset = 0U, u32 size = 0U, u32 level = 0U)                                                                             override;
        void                        fill(const void* data, const core::Dimension2D& offset = core::Dimension2D(), const core::Dimension2D& size = core::Dimension2D(), u32 level = 0U) override;
        void                        fill(const void* data, const core::Dimension3D& offset = core::Dimension3D(), const core::Dimension3D& size = core::Dimension3D(), u32 level = 0U) override;

        ETextureTarget              getTarget()      const override;
        ETextureFilter              getMinFiler()    const override;
        ETextureFilter              getMagFiler()    const override;
        EWrapType                   getWrap()        const override;
        EAnisotropic                getAnisotropic() const override;
        u32                         getMipmapLevel() const override;
        const core::Dimension3D&    getSize()        const override;

        void                        setFilterType(ETextureFilter min, ETextureFilter mag)   override;
        void                        setWrap(EWrapType wrap)                                 override;
        void                        setAnisotropicLevel(EAnisotropic level)                 override;

        void                        freeMemory(u32 texture);

        u32                         getTextureID() const;

        static void                 reset();
        static u32                  internalFormat(u32 format, u32 type);

    private:

        bool                        create();
        void                        destroy();

        friend                      CRendererGL;

        void                        setSampler(SamplerGL* sampler);

        static bool                 bindTexture(ETextureTarget target, u32 texture);
        static void                 bindTexBuffer(u32 format, u32 texture, u32 buffer, u32 offset = 0, u32 size = 0);
        static bool                 bindTextureUnit(u32 unit);

        static s32                  getActiveTexture(u32 target);
        static s32                  getActiveTextureUnit();

    private:

        ETextureTarget              m_target;

        EImageFormat                m_format;
        EImageType                  m_type;
        core::Dimension3D           m_size;

        bool                        m_enable;

        ETextureFilter              m_minFilter;
        ETextureFilter              m_magFilter;
        EAnisotropic                m_anisotropicLevel;
        EWrapType                   m_wrap;
        u32                         m_mipmapLevel;

    private:

        SamplerGL*                  m_sampler;
        BufferGL*                   m_textureBuffer;

        bool                        m_initialized;

        u32                         m_textureID;

        static u32                  s_currentTextureID[eTargetCount];
        static u32                  s_currentUnitID;
   };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace gl
} //namespace renderer
} //namespace v3d

#endif //_V3D_TEXTURE_GL_H_
