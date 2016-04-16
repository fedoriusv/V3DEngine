#ifndef _V3D_TEXTURE_H_
#define _V3D_TEXTURE_H_

#include "stream/Resource.h"
#include "ImageFormats.h"

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

        virtual void    bind(u32 layer, u32 sampler)                                                                                = 0;
        virtual void    unbind(u32 layer, u32 sampler)                                                                              = 0;
        virtual bool    create()                                                                                                    = 0;
        virtual void    destroy()                                                                                                   = 0;
        virtual bool    isValid()                                                                                                   = 0;

        virtual void    updateData(u32 offset, u32 size, void* data)                                                                = 0;
        virtual void    updateData(const core::Dimension2D& offset, const core::Dimension2D& size, void* data, u32 cubemapSide = 0) = 0;
        virtual void    updateData(const core::Dimension3D& offset, const core::Dimension3D& size, void* data) = 0;

        virtual void    readData(void* data, u32 cubemapSide = 0)                                                                   = 0;

        virtual void    fill(u32 offset = 0U, u32 size = 0U, void* data = nullptr)                                                                                                   = 0;
        virtual void    fill(const core::Dimension2D& offset = core::Dimension2D(0U, 0U), const core::Dimension2D& size = core::Dimension2D(0U, 0U), void* data = nullptr)           = 0;
        virtual void    fill(const core::Dimension3D& offset = core::Dimension3D(0U, 0U, 0U), const core::Dimension3D& size = core::Dimension3D(0U, 0U, 0U), void* data = nullptr)   = 0;

        void            init(const stream::IStreamPtr& stream)  override;
        bool            load()                                  override;

        bool            isEnable()      const;

        ETextureTarget  getTarget()     const;
        ETextureFilter  getMinFiler()   const;
        ETextureFilter  getMagFiler()   const;
        EWrapType       getWrap()       const;
        EAnisotropic    getAnisotropic()const;
        u32             getMipmapLevel()const;

        virtual void    setFilterType(ETextureFilter min, ETextureFilter mag);
        virtual void    setWrap(EWrapType wrap);
        virtual void    setAnisotropicLevel(EAnisotropic level);
        virtual void    setMipmapLevel(u32 level);

        const core::Dimension3D& getSize(u32 cubemapSide = 0U) const;

    private:

        struct STextureData
        {
            STextureData();
            ~STextureData();

            STextureData&       operator=(const STextureData&) = delete;

            void                free();
            void                copy(const core::Dimension3D& size, EImageType type, void* data);

            core::Dimension3D   _size;
            EImageFormat        _format;
            EImageType          _type;
            void*               _data;
        };

        using TextureData = std::vector<STextureData>;

    protected:

        friend          v3d::scene::CTextureManager;

        void            clear();

        ETextureTarget  m_target;
        TextureData     m_data;

        bool            m_enable;

        ETextureFilter  m_minFilter;
        ETextureFilter  m_magFilter;
        EAnisotropic    m_anisotropicLevel;
        EWrapType       m_wrap;
        u32             m_mipmapLevel;

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
