#ifndef _V3D_VECTOR_FONT_DATA_H_
#define _V3D_VECTOR_FONT_DATA_H_

#include "FontData.h"

#include "ft2build.h" 
#include FT_FREETYPE_H
#include FT_STROKER_H

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CVectorFontData : public CFontData
    {
    public:

        CVectorFontData(const std::string& font);
        virtual                     ~CVectorFontData();

        void                        init(stream::IStream* stream)   override;
        bool                        load()                          override;

        void                        refresh();

        bool                        findCharsOnMap(const std::string& text);
        bool                        addCharsToMap(const std::string& text);
        void                        setFontSize(u32 size);
        bool                        loadCharList();

        //TMp
        std::vector<renderer::TexturePtr>  m_charMaterial;

    private:

        bool                        loadFreeType(const std::string& font);

        bool                        loadCharToMap(u32 charId);
        void                        fillCharInfo(SCharDesc& charDesc, const FT_BitmapGlyph btGlyph, const FT_GlyphSlot glSlot, const FT_Fixed fixed);

        const static u32            k_mapSize = 256U;

        FT_Library                  m_Library;
        FT_Face                     m_Face;
        u32                         m_fontSize;

        std::map<s32, SCharDesc>    m_charInfo;

        renderer::TexturePtr        m_charTextures[k_mapSize];
        bool                        m_charList[k_mapSize];

        const u32                   k_texWidth = 256U;
        const u32                   k_texHight = 256U;
        u32                         m_xOffTextures;
        u32                         m_yOffTextures;
        u32                         m_currentTextureIndex;

        bool                        m_loaded;
        bool                        m_regenerateMap;



    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_FREETYPE_DATA_H_