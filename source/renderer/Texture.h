#ifndef _V3D_TEXTURE_H_
#define _V3D_TEXTURE_H_

#include "Object.h"
#include "stream/Resource.h"

namespace v3d
{
namespace renderer
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	enum ETextureTarget
	{
		eUnknown = -1,
		eTexture1D,
		eTexture2D,
		eTexture3D,
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
        eInt,
        eUnsignedInt,
        eFloat,
        eDouble,
        eHalf,

        eTypeCount
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

	struct STextureData
	{
		u16				width;
		u16				height;
		u16				depth;
		EImageFormat	format;
		EImageType		type;
		void*			data;
	};

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::vector<STextureData>  TextureData;

	/////////////////////////////////////////////////////////////////////////////////////////////////////

    class CTexture : public stream::CResource
    {
    public:
        CTexture();
        virtual         ~CTexture();

        virtual void    bind(u32 layer)                     = 0;
        virtual void    unbind(u32 layer)                   = 0;
        virtual bool    create()                            = 0;
        virtual void    destroy()                           = 0;

        void            init(stream::IStream* stream) override;
        bool            load()                        override;

		u32				getTextureID()  const;
		ETextureTarget	getTarget()     const;
		ETextureFilter	getMinFiler()   const;
		ETextureFilter	getMagFiler()   const;
        EWrapType       getWrap()       const;
        EAnisotropic    getAnisotropic()const;

        void            setFilterType(ETextureFilter min, ETextureFilter mag);
        void            setWrap(EWrapType wrap);
        void            setAnisotropicLevel(EAnisotropic level);

    protected:

#ifdef USE_DEVIL
        bool            loadDevIL();
#endif //USE_DEVIL

        void            clear();

        u32             m_textureID;

        ETextureTarget  m_target;
        TextureData     m_data;

        ETextureFilter  m_minFilter;
        ETextureFilter  m_magFilter;
        EAnisotropic    m_anisotropicLevel;
        EWrapType       m_wrap;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CTexture>           TexturePtr;
    typedef std::vector<TexturePtr>             TextureList;
    typedef std::map<std::string, TexturePtr>   TextureMap;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_TEXTURE_H_
