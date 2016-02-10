#ifndef _V3D_TEXTURE_H_
#define _V3D_TEXTURE_H_

#include "stream/Resource.h"

namespace v3d
{

namespace scene
{
    class CTextureManager;
}

namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    enum ETextureTarget
    {
        eTextureUnknown = - 1,

        eTexture1D,
        eTexture2D,
        eTexture2DMSAA,
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

    enum EImageType
    {
        eByte,
        eUnsignedByte,
        eShort,
        eUnsignedShort,
        eUnsignedShort_565,
        eUnsignedShort_4444,
        eInt,
        eUnsignedInt,
        eFloat,

        eImageTypeCount
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EImageFormat
    {
        eColorIndex,
        eAlpha,
        eRGB,
        eRGBA,
        eBGR,
        eBGRA,
        eLuminance,
        eLuminanceAlpha,
        eDepthComponent,

        eFormatCount
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    enum ETextureLayer
    {
        eLayer0 = 0,
        eLayer1,
        eLayer2,
        eLayer3,

        eLayerMax,
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * Base Interface for texture entity.
    */
    class CTexture : public stream::CResource
    {
    public:

        CTexture();
        virtual         ~CTexture();

        virtual void    bind(u32 layer, u32 sampler)        = 0;
        virtual void    unbind(u32 layer, u32 sampler)      = 0;
        virtual bool    create()                            = 0;
        virtual void    destroy()                           = 0;

        virtual bool    isValid()                           = 0;

        virtual void    setData(u32 size, void* data) = 0;
        virtual void    setData(const core::Dimension2D& size, void* data, u32 cubemapSide = 0) = 0;
        virtual void    setData(const core::Dimension3D& size, void* data) = 0;

        virtual void    updateData(u32 offset, u32 size, void* data) = 0;
        virtual void    updateData(const core::Dimension2D& offset, const core::Dimension2D& size, void* data, u32 cubemapSide = 0) = 0;
        virtual void    updateData(const core::Dimension3D& offset, const core::Dimension3D& size, void* data) = 0;


        virtual void    readData(void* data, u32 cubemapSide = 0) = 0;

        void            init(const stream::IStreamPtr& stream)  override;
        bool            load()                                  override;

        bool            isEnable()      const;

        ETextureTarget  getTarget()     const;
        ETextureFilter  getMinFiler()   const;
        ETextureFilter  getMagFiler()   const;
        EWrapType       getWrap()       const;
        EAnisotropic    getAnisotropic()const;

        virtual void    setFilterType(ETextureFilter min, ETextureFilter mag);
        virtual void    setWrap(EWrapType wrap);
        virtual void    setAnisotropicLevel(EAnisotropic level);

    private:

        struct STextureData
        {
            STextureData();
            ~STextureData();

            STextureData& operator=(const STextureData&) = delete;

            void            free();
            void            copy(const core::Dimension3D& size, EImageType type, void* data);

            core::Dimension3D   _size;
            EImageFormat        _format;
            EImageType          _type;
            void*               _data;
        };

        using TextureData = std::vector<STextureData>;

    protected:

        friend          v3d::scene::CTextureManager;

        void            clear();

        virtual void    copyToTexture2D(const core::Dimension2D& offset, const core::Dimension2D& size, EImageFormat format, void* data) = 0;

        ETextureTarget  m_target;
        TextureData     m_data;

        bool            m_enable;

        ETextureFilter  m_minFilter;
        ETextureFilter  m_magFilter;
        EAnisotropic    m_anisotropicLevel;
        EWrapType       m_wrap;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CTexture>               TexturePtr;
    typedef std::weak_ptr<CTexture>                 TextureWPtr;
    typedef std::map<std::string, TexturePtr>       TextureMap;
    typedef std::vector<CTexture*>                  TextureList;
    typedef std::vector<const CTexture*>            KTextureList;
    typedef std::map<const u32, const CTexture*>    TextureLayers;

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_V3D_TEXTURE_H_
