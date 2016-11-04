#pragma once

#include "ImageFormats.h"
#include "stream/Resource.h"
#include "utils/RefCounted.h"
#include "utils/IntrusivePtr.h"
#include "utils/Cloneable.h"

namespace v3d
{

namespace resources
{
    class CImage;
}

namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    const u32 k_textureCubemapSideCount = 6U;

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    enum ETextureTarget
    {
        eTextureNull = -1,

        eTexture1D,
        eTexture1DArray,
        eTextureRectangle,
        eTexture2D,
        eTexture2DMSAA,
        eTexture2DArray,
        eTexture3D,
        eTexture3DMSAA,
        eTextureCubeMap,
        eTextureBuffer,

        eTargetCount
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    enum ETextureFilter
    {
        eNearest,
        eLinear,

        eNearestMipmapNearest,
        eNearestMipmapLinear,
        eLinearMipmapNearest,
        eLinearMipmapLinear,

        eFilterCount
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EAnisotropic
    {
        eAnisotropicNone = 0,
        eAnisotropic2x   = 1 << 1,
        eAnisotropic4x   = 1 << 2,
        eAnisotropic8x   = 1 << 3,
        eAnisotropic16x  = 1 << 4,
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EWrapType
    {
        eRepeat,
        eMirroredRepeat,
        eClampToEdge,
        eClampToBorder,

        eWrapCount
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class Texture;
    class RenderThread;

    using TexturePtr = utils::TIntrusivePtr<Texture>;

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * Texture resource.
    */
    class Texture : public utils::CRefCounted, public utils::TCloneable<TexturePtr>
    {
    public:

        Texture(ETextureTarget target, EImageFormat format, EImageType type, u32 size, const void* data = nullptr, u32 mipCount = 1U);
        Texture(ETextureTarget target, EImageFormat format, EImageType type, const core::Dimension2D& size, const void* data = nullptr, u32 mipCount = 1U);
        Texture(ETextureTarget target, EImageFormat format, EImageType type, const core::Dimension3D& size, const void* data = nullptr, u32 mipCount = 1U);
        Texture(EImageFormat format, EImageType type, const core::Dimension2D& size, const void* data[k_textureCubemapSideCount] = {}, u32 mipCount = 1U);

        Texture(const Texture&) = delete;
        Texture&                            operator=(const Texture&) = delete;

        virtual                             ~Texture();

        virtual void                        bind(u32 unit);
        virtual void                        unbind();

        virtual bool                        isValid() const;
        virtual bool                        isEnable() const;

        virtual void                        update(u32 offset, u32 size, const void* data, u32 mipLevel = 0U);
        virtual void                        update(const core::Dimension2D& offset, const core::Dimension2D& size, const void* data, u32 mipLevel = 0U);
        virtual void                        update(const core::Dimension3D& offset, const core::Dimension3D& size, const void* data, u32 mipLevel = 0U);
        virtual void                        update(u32 cubemapSide, const core::Dimension2D& offset, const core::Dimension2D& size, const void* data, u32 mipLevel = 0U);

        virtual void                        read(void* const data, u32 mipLevel = 0U) const;
        virtual void                        read(u32 cubemapSide, void* const data, u32 mipLevel = 0U) const;

        virtual void                        fill(const void* data, u32 offset = 0U, u32 size = 0U, u32 mipLevel = 0U);
        virtual void                        fill(const void* data, const core::Dimension2D& offset = core::Dimension2D(), const core::Dimension2D& size = core::Dimension2D(), u32 mipLevel = 0U);
        virtual void                        fill(const void* data, const core::Dimension3D& offset = core::Dimension3D(), const core::Dimension3D& size = core::Dimension3D(), u32 mipLevel = 0U);

        virtual ETextureTarget              getTarget() const;
        virtual ETextureFilter              getMinFilter() const;
        virtual ETextureFilter              getMagFilter() const;
        virtual EWrapType                   getWrap() const;
        virtual EAnisotropic                getAnisotropic() const;
        virtual u32                         getMipmapLevels() const;
        virtual const core::Dimension3D&    getSize() const;
        virtual EImageFormat                getFormat() const;
        virtual EImageType                  getType() const;
        
        virtual void                        setFilterType(ETextureFilter min, ETextureFilter mag);
        virtual void                        setWrap(EWrapType wrap);
        virtual void                        setAnisotropicLevel(EAnisotropic level);

        TexturePtr                          getImpl() const;

        TexturePtr                          clone() const override;

    protected:

        Texture();

        virtual void                        copyData(Texture* texture);


        friend                              RenderThread;
        virtual bool                        create(const void* data, u32 srcSize);
        virtual void                        destroy();

    private:

        Texture* const                      m_impl;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::map<std::string, TexturePtr>      TextureMap;
    typedef std::map<const u32, TexturePtr>        TextureUnits;
    typedef std::vector<TexturePtr>                TextureList;

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d
