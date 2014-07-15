#ifndef _V3D_FREETYPE_DATA_H_
#define _V3D_FREETYPE_DATA_H_

#include "stream/Resource.h"
#include "renderer/Texture.h"

#include "ft2build.h" 
#include FT_FREETYPE_H
#include FT_STROKER_H

namespace v3d
{
namespace scene
{
    class CFontManager;
}
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CFreeTypeData : public stream::CResource
    {
    public:

        CFreeTypeData(const std::string& font);
        virtual                     ~CFreeTypeData();

        const std::string&          getFontName() const;

        void                        init(stream::IStream* stream)   override;
        bool                        load()                          override;

    private:

        friend class                CFontManager;

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

        void                        copyToTexture(u32 width, u32 height, u8* data, SCharDesc* charDesc);
        void                        createChar(const FT_Face& ftFace, FT_UInt glyphIndex);
        bool                        loadCharToMap(u32 charId);

        bool                        findeCharsOnMap(const std::string& text);
        void                        addedCharsToMap(const std::string& text);

        std::string                 m_font;

        const static u32            k_mapSize = 256U;

        FT_Library                  m_Library;
        FT_Face                     m_Face;

        std::map<s32, SCharDesc>    m_charInfo;
        renderer::TexturePtr        m_charTextures[k_mapSize];

        bool                        m_charList[k_mapSize];

        u32                         m_loadedPixelSize;

        bool                        m_loaded;
        bool                        m_regenerateMap;



    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CFreeTypeData>         FreeTypeDataPtr;
    typedef std::map<std::string, FreeTypeDataPtr> FreeTypeDataList;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_FREETYPE_DATA_H_