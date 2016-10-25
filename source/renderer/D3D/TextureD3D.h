#pragma once

#include "renderer/Texture.h"

#ifdef _V3D_TEXTURE_D3D_H_
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
    class CTextureD3D : public Texture
    {
    public:

        CTextureD3D(ETextureTarget target, EImageFormat format, EImageType type, const core::Dimension3D& size, const void* data, u32 level = 0U);
        CTextureD3D(EImageFormat format, EImageType type, const core::Dimension2D& size, const void* data[6], u32 level = 0U);
        ~CTextureD3D();

        void                        bind(u32 unit)   override;
        void                        unbind()         override;

        bool                        isValid()  const override;
        bool                        isEnable() const override;

        void                        update(u32 offset, u32 size, const void* data, u32 level = 0U)                                                              override;
        void                        update(const core::Dimension2D& offset, const core::Dimension2D& size, const void* data, u32 level = 0U)                    override;
        void                        update(const core::Dimension3D& offset, const core::Dimension3D& size, const void* data, u32 level = 0U)                    override;
        void                        update(u32 cubemapSide, const core::Dimension2D& offset, const core::Dimension2D& size, const void* data, u32 level = 0U)   override;

        void                        read(void const* data, u32 level = 0U) const override;
        void                        read(u32 cubemapSide, void const* data, u32 level = 0U) const override;

        void                        setFilterType(ETextureFilter min, ETextureFilter mag)   override;
        void                        setWrap(EWrapType wrap)                                 override;
        void                        setAnisotropicLevel(EAnisotropic level)                 override;

        void                        fill(const void* data, u32 offset = 0U, u32 size = 0U, u32 level = 0U)                                                                             override;
        void                        fill(const void* data, const core::Dimension2D& offset = core::Dimension2D(), const core::Dimension2D& size = core::Dimension2D(), u32 level = 0U) override;
        void                        fill(const void* data, const core::Dimension3D& offset = core::Dimension3D(), const core::Dimension3D& size = core::Dimension3D(), u32 level = 0U) override;

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

        bool                        m_initialized;

    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace d3d
} //namespace renderer
} //namespace v3d

#endif //_V3D_TEXTURE_D3D_H_
