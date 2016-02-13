#ifndef _V3D_VECTOR_FONT_DATA_H_
#define _V3D_VECTOR_FONT_DATA_H_

#include "FontData.h"

#include "ft2build.h" 
#include FT_FREETYPE_H
#include FT_STROKER_H

namespace v3d
{
namespace resources
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * Vector Font data realization.
    */
    class CVectorFontData : public CFontData
    {
    public:

        explicit CVectorFontData(const std::string& font);
        ~CVectorFontData();

        void                        init(const stream::IStreamPtr& stream)  override;
        bool                        load()                                  override;

    private:

        bool                        addCharsToMap(const std::string& text)  override final;
        bool                        loadFont(const std::string& resource)   override final;

        bool                        loadCharToMap(u32 charId);
        bool                        loadCharList();
        void                        fillCharInfo(SCharDesc& charDesc, const FT_BitmapGlyph btGlyph, const FT_GlyphSlot glSlot, const FT_Fixed fixed);

        FT_Library                  m_Library;
        FT_Face                     m_Face;

        bool                        m_charList[k_fontMapSize];

        core::Dimension2D           m_offetTextures;
        u32                         m_currentTextureIndex;

        bool                        m_loaded;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace resources
} //namespace v3d

#endif //_V3D_FREETYPE_DATA_H_
