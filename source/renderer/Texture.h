#ifndef _V3D_TEXTURE_H_
#define _V3D_TEXTURE_H_

#include "Object.h"
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
        eDouble,
        eHalf,

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
        eLumiance,
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

    class CTexture : public stream::CResource
    {
    public:

        CTexture();
        virtual         ~CTexture();

        virtual void    bind(u32 layer, u32 sampler)        = 0;
        virtual void    unbind(u32 layer, u32 sampler)      = 0;
        virtual bool    create()                            = 0;
        virtual void    destroy()                           = 0;

        void            init(const stream::IStreamPtr& stream)  override;
        bool            load()                                  override;

        bool            isEnable()      const;

        u32             getTextureID()  const;
        ETextureTarget  getTarget()     const;
        ETextureFilter  getMinFiler()   const;
        ETextureFilter  getMagFiler()   const;
        EWrapType       getWrap()       const;
        EAnisotropic    getAnisotropic()const;

        void            setFilterType(ETextureFilter min, ETextureFilter mag);
        void            setWrap(EWrapType wrap);
        void            setAnisotropicLevel(EAnisotropic level);

    private:

        struct STextureData
        {
            u16             _width;
            u16             _height;
            u16             _depth;
            EImageFormat    _format;
            EImageType      _type;
            void*           _data;
        };

        typedef std::vector<STextureData>  TextureData;

    protected:

        friend          v3d::scene::CTextureManager;

        void            clear();

        virtual void    copyToTexture2D(const Dimension2D& offset, const Dimension2D& size, EImageFormat format, void* data) = 0;

        u32             m_textureID;

        ETextureTarget  m_target;
        TextureData     m_data;

        bool            m_enable;

        ETextureFilter  m_minFilter;
        ETextureFilter  m_magFilter;
        EAnisotropic    m_anisotropicLevel;
        EWrapType       m_wrap;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CTexture>           TexturePtr;
    typedef std::weak_ptr<CTexture>             TextureWPtr;
    typedef std::vector<TexturePtr>             TextureList;
    typedef std::map<std::string, TexturePtr>   TextureMap;
    typedef std::map<const u32, TexturePtr>     TextureLayers;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_TEXTURE_H_
