#ifndef _V3D_FONT_DATA_H_
#define _V3D_FONT_DATA_H_

#include "stream/Resource.h"
#include "renderer/Texture.h"

namespace v3d
{
namespace scene
{
    class CFontManager;
}
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CFontData : public stream::CResource
    {
    public:

        enum class EFontType
        {
            eUnknownFont,
            eBitmapFont,
            eVectorFont
        };

        struct SCharDesc
        {
            u32     _width;
            u32     _height;
            s32     _advX;
            s32     _advY;
            s32     _bearingX;
            s32     _bearingY;
            u32     _page;
        };

        CFontData(const std::string& font);
        virtual                     ~CFontData();

        EFontType                   getFontType() const;
        const std::string&          getFontName() const;

        const SCharDesc*            getCharInfo(const s32 charCode) const;

    protected:

        friend                      scene::CFontManager;

        void                        setFontType(EFontType type);

        virtual bool                loadFont(const std::string& resource) = 0;

        EFontType                   m_fontType;

        std::map<s32, SCharDesc>    m_charInfo;
        std::vector<TexturePtr>     m_charTexture;

        std::string                 m_font;
        u32                         m_fontSize;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CFontData>         FontDataPtr;
    typedef std::map<std::string, FontDataPtr> FontDataList;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_FONT_DATA_H_
