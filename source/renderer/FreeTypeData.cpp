#include "FreeTypeData.h"

using namespace v3d;
using namespace v3d::scene;
using namespace v3d::renderer;


inline int next_p2(int n)
{
    int res = 1;
    while (res < n)
    {
        res <<= 1;
    }

    return res;
}


CFreeTypeData::CFreeTypeData(const std::string& font)
    : m_loaded(false)
    , m_font(font)
    , m_regenerateMap(false)
{
    for (u32 i = 0; i < k_mapSize; ++i)
    {
        m_charList[i] = false;
        m_charTextures[i] = nullptr;
    }
    
}

CFreeTypeData::~CFreeTypeData()
{
}

const std::string& CFreeTypeData::getFontName() const
{
    return m_font;
}

void CFreeTypeData::addedCharsToMap(const std::string& text)
{
    for (std::string::const_iterator chr = text.begin(); chr < text.end(); ++chr)\
    {
        m_charList[(*chr)] = true;
    }

    m_regenerateMap = true;
}

bool CFreeTypeData::findeCharsOnMap(const std::string& text)
{
    for (std::string::const_iterator chr = text.begin(); chr < text.end(); ++chr)\
    {
        if (!m_charList[(*chr)])
        {
            return false;
        }
    }

    return true;
}

void CFreeTypeData::init(stream::IStream* stream)
{
}

bool CFreeTypeData::load()
{
    if (m_font.empty())
    {
        ASSERT(false && "FreeTypeFont: empty name");
        return false;
    }

    if (m_loaded)
    {
        ASSERT(false && "FreeTypeFont: Already loaded");
        return false;
    }

    FT_Error error = FT_Init_FreeType(&m_Library);
    if (error)
    {
        ASSERT(false && "FreeTypeFont: Error Init Free Type");
        return false;
    }

    error = FT_New_Face(m_Library, m_font.c_str(), 0, &m_Face);
    if (error == FT_Err_Unknown_File_Format)
    {
        ASSERT(false && "FreeTypeFont:: Unknown Font File Format");
        return false;
    }
    else if (error)
    {
        ASSERT(false && "FreeTypeFont: Font Error");
        return false;
    }

    int iPXSize = 32;
    m_loadedPixelSize = iPXSize;
    FT_Set_Pixel_Sizes(m_Face, iPXSize, iPXSize);

    for (u32 i = 0; i < k_mapSize; ++i)
    {
        if (m_charList[i])
        {
            CFreeTypeData::loadCharToMap(i);
        }
        
    }
    //loadCharToMap('A');
    //loadCharToMap('a');
    //loadCharToMap('b');
    //loadCharToMap('c');
    //loadCharToMap('d');
    //loadCharToMap('e');
    //loadCharToMap('f');
    //loadCharToMap('g');
    //loadCharToMap('h');

    //ftFace->style_flags = ftFace->style_flags | FT_STYLE_FLAG_ITALIC;
    //if(FT_Set_Char_Size(m_FontFace, size << 6, size << 6, 96, 96) != 0)
    //http://www.asciitable.com

    //FT_UInt num_chars = 128;
    //m_vertices.malloc(4 * num_chars);

    //for (uint glyphIndex = 32/*' '*/; glyphIndex < 126/*~*/; ++glyphIndex)
    //{
    //    CFreeTypeFont::createChar(m_Face, glyphIndex);
    //}
    //m_bLoaded = true;

    //RENDERER->initBufferObjects(m_vertices);
    ////m_vertices.clear();

    //FT_Done_Face(m_Face);
    //FT_Done_FreeType(m_Library);
}

void CFreeTypeData::copyToTexture(u32 width, u32 height, u8* data, SCharDesc* charDesc)
{
    //m_iTexWidth = 128;
    //m_iTexHight = 128;

    //static int lineHeight;
    //if (lineHeight < _height) lineHeight = _height;

    //if (m_xOffTextures + _width >= m_iTexWidth)
    //{
    //    m_xOffTextures = 0;
    //    m_yOffTextures += lineHeight + 1;
    //}

    //if (m_yOffTextures + lineHeight >= m_iTexHight)
    //{
    //    m_yOffTextures = 0;
    //    m_currentTextureIndx++;
    //}

    //if (m_currentTextureIndx >= m_charMaterial.size())
    //{	// Create big texture
    //    //char textureId[64];
    //    //sprintf(textureId, "textureFont%s%d", m_FontFileName.c_str(), m_currentTextureIndx);

    //    unsigned char* textureData = new GLubyte[2 * m_iTexWidth * m_iTexHight];
    //    memset(textureData, 0, 2 * m_iTexWidth * m_iTexHight);

    //    CTexture* texture = TEXTURE_MGR->createTexture2DFromData(m_iTexWidth, m_iTexHight, IF_DEPTH_COMPONENT, IT_UNSIGNED_BYTE, textureData);
    //    texture->getSampler()->setFilterType(FT_LINEAR, FT_LINEAR);
    //    texture->getSampler()->setWrapType(WT_CLAMP_TO_EDGE);

    //    m_charMaterial.push_back(texture);

    //    delete[] textureData;
    //}

    //RENDERER->copyToTexture2D(m_charMaterial[m_currentTextureIndx]->getTextureID(), m_xOffTextures, m_yOffTextures, _width, _height, _data);

    //// Apply texture coord
    //_charDesc->_iBearingX = m_xOffTextures;
    //_charDesc->_iBearingY = m_yOffTextures;
    //_charDesc->_iWidth = _width;
    //_charDesc->_iHeight = _height;
    //_charDesc->_page = m_currentTextureIndx;

    //m_xOffTextures += _width + 1;
}

void CFreeTypeData::createChar(const FT_Face& ftFace, FT_UInt glyphIndex)
{
    //FT_Load_Glyph(m_Face, FT_Get_Char_Index(m_Face, _glyphIndex), FT_LOAD_DEFAULT);
    //FT_Render_Glyph(m_Face->glyph, FT_RENDER_MODE_NORMAL);

    //FT_Stroker stroker;
    //FT_Stroker_New(m_Library, &stroker);

    //FT_Glyph glyph;
    //FT_Get_Glyph(m_Face->glyph, &glyph);
    //FT_Glyph_StrokeBorder(&glyph, stroker, 0, 5);

    //FT_Bitmap* pBitmap = &m_Face->glyph->bitmap;

    //uint width = pBitmap->width;
    //uint hight = pBitmap->rows;

    ////uint iTW = next_p2(width);
    ////uint iTH = next_p2(hight);

    ////GLubyte* bData = new GLubyte[iTW*iTH];
    //// Copy glyph data and add dark pixels elsewhere
    ///*for ( uint ch = 0; ch < iTH; ++ ch)
    //{
    //for ( uint cw = 0; cw < iTW; ++cw )
    //{
    //bData[ch*iTW+cw] = (ch >= hight || cw >= width) ? 0 : pBitmap->buffer[(hight-ch-1)*width+cw];
    //}
    //}*/

    //unsigned int * bData = new unsigned int[width*hight];
    //memset(bData, NULL, width * hight * sizeof(unsigned char));
    //memcpy(bData, pBitmap->buffer, sizeof(unsigned char)* width * hight);

    ////texture
    //if (m_pCharTextures[_glyphIndex])
    //{
    //    delete m_pCharTextures[_glyphIndex];
    //    m_pCharTextures[_glyphIndex] = nullptr;
    //}
    //m_pCharTextures[_glyphIndex] = TEXTURE_MGR->createTexture2DFromData(width, hight, IF_DEPTH_COMPONENT, IT_UNSIGNED_BYTE, bData);
    //m_pCharTextures[_glyphIndex]->getSampler()->setFilterType(FT_LINEAR, FT_LINEAR);
    //m_pCharTextures[_glyphIndex]->getSampler()->setWrapType(WT_CLAMP_TO_EDGE);

    //// Calculate glyph data
    //m_iAdvX[_glyphIndex] = m_Face->glyph->advance.x >> 6;
    //m_iBearingX[_glyphIndex] = m_Face->glyph->metrics.horiBearingX >> 6;
    //m_iCharWidth[_glyphIndex] = m_Face->glyph->metrics.width >> 6;

    //m_iAdvY[_glyphIndex] = (m_Face->glyph->metrics.height - m_Face->glyph->metrics.horiBearingY) >> 6;
    //m_iBearingY[_glyphIndex] = m_Face->glyph->metrics.horiBearingY >> 6;
    //m_iCharHeight[_glyphIndex] = m_Face->glyph->metrics.height >> 6;

    //m_iNewLine = max(m_iNewLine, int(m_Face->glyph->metrics.height >> 6));

    //delete[] bData;
    //bData = nullptr;


    //m_vertices.Vertex.vertices[_glyphIndex * 4].setX(0.0f);
    //m_vertices.Vertex.vertices[_glyphIndex * 4].setY(float(-m_iAdvY[_glyphIndex] + hight));
    //m_vertices.Vertex.vertices[_glyphIndex * 4].setZ(0.0f);

    //m_vertices.Vertex.vertices[_glyphIndex * 4 + 1].setX(0.0f);
    //m_vertices.Vertex.vertices[_glyphIndex * 4 + 1].setY(float(-m_iAdvY[_glyphIndex]));
    //m_vertices.Vertex.vertices[_glyphIndex * 4 + 1].setZ(0.0f);

    //m_vertices.Vertex.vertices[_glyphIndex * 4 + 2].setX(float(width));
    //m_vertices.Vertex.vertices[_glyphIndex * 4 + 2].setY(float(-m_iAdvY[_glyphIndex] + hight));
    //m_vertices.Vertex.vertices[_glyphIndex * 4 + 2].setZ(0.0f);

    //m_vertices.Vertex.vertices[_glyphIndex * 4 + 3].setX(float(width));
    //m_vertices.Vertex.vertices[_glyphIndex * 4 + 3].setY(float(-m_iAdvY[_glyphIndex]));
    //m_vertices.Vertex.vertices[_glyphIndex * 4 + 3].setZ(0.0f);


    ///*m_vertices.TexCoord.at(0).vertices[_glyphIndex*4].setX(0.0f);
    //m_vertices.TexCoord.at(0).vertices[_glyphIndex*4].setY(1.0f);

    //m_vertices.TexCoord.at(0).vertices[_glyphIndex*4+1].setX(0.0f);
    //m_vertices.TexCoord.at(0).vertices[_glyphIndex*4+1].setY(0.0f);

    //m_vertices.TexCoord.at(0).vertices[_glyphIndex*4+2].setX(1.0f);
    //m_vertices.TexCoord.at(0).vertices[_glyphIndex*4+2].setY(1.0f);

    //m_vertices.TexCoord.at(0).vertices[_glyphIndex*4+3].setX(1.0f);
    //m_vertices.TexCoord.at(0).vertices[_glyphIndex*4+3].setY(0.0f);*/

    //m_vertices.TexCoord.at(0).vertices[_glyphIndex * 4].setX(0.0f);
    //m_vertices.TexCoord.at(0).vertices[_glyphIndex * 4].setY(0.0f);

    //m_vertices.TexCoord.at(0).vertices[_glyphIndex * 4 + 1].setX(0.0f);
    //m_vertices.TexCoord.at(0).vertices[_glyphIndex * 4 + 1].setY(1.0f);

    //m_vertices.TexCoord.at(0).vertices[_glyphIndex * 4 + 2].setX(1.0f);
    //m_vertices.TexCoord.at(0).vertices[_glyphIndex * 4 + 2].setY(0.0f);

    //m_vertices.TexCoord.at(0).vertices[_glyphIndex * 4 + 3].setX(1.0f);
    //m_vertices.TexCoord.at(0).vertices[_glyphIndex * 4 + 3].setY(1.0f);
}

bool CFreeTypeData::loadCharToMap(u32 charId)
{
    //if (m_charInfo.find(_char) != m_charInfo.end())
    //{
    //    return true;
    //}

    //static float	outline_thikness = 0 * 64;
    //FT_Glyph		glyph;
    //FT_GlyphSlot	glyphSlot;
    //FT_Stroker		stroker;

    //if (FT_Load_Glyph(m_Face, _char/*FT_Get_Char_Index(m_Face, _char)*/, FT_LOAD_DEFAULT))
    //{
    //    return false;
    //}
    //glyphSlot = m_Face->glyph;

    //if (FT_Load_Char(m_Face, _char/*FT_Get_Char_Index(m_Face, _char)*/, FT_LOAD_RENDER))
    //{
    //    return false;
    //}

    //if (FT_Stroker_New(m_Library, &stroker))
    //{
    //    return false;
    //}

    //FT_Stroker_Set(stroker, outline_thikness, FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);
    //if (FT_Get_Glyph(m_Face->glyph, &glyph))
    //{
    //    return false;
    //}

    //if (FT_Glyph_StrokeBorder(&glyph, stroker, 0, 1))//FT_Glyph_Stroke( &glyph, stroker, 1 ))
    //{
    //    //			return false;
    //}

    //if (FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL/*FT_RENDER_MODE_LCD*/, 0, 1))
    //{
    //    return false;
    //}

    //FT_BitmapGlyph ft_bitmap_glyph = (FT_BitmapGlyph)glyph;
    //FT_Stroker_Done(stroker);

    //int scale = 1.0f;
    //int fontHeight = 1.0f;

    //SCharDesc charDscr;
    //charDscr._iAdvX = (glyphSlot->advance.x + outline_thikness) / 64 * scale;
    //charDscr._iBearingX = ft_bitmap_glyph->left * scale;
    //charDscr._iBearingY = fontHeight / 1.2 - ft_bitmap_glyph->top * scale;

    //int width = next_p2(ft_bitmap_glyph->bitmap.width);
    //int height = next_p2(ft_bitmap_glyph->bitmap.rows);
    //unsigned char* expanded_data = new unsigned char[2 * width * height];
    //for (int j = 0; j < height; j++)
    //{
    //    for (int i = 0; i < width; i++)
    //    {
    //        expanded_data[2 * (i + j * width)] = expanded_data[2 * (i + j * width) + 1] =
    //            (i >= ft_bitmap_glyph->bitmap.width || j >= ft_bitmap_glyph->bitmap.rows) ?
    //            0 : ft_bitmap_glyph->bitmap.buffer[i + ft_bitmap_glyph->bitmap.width * j];
    //    }
    //}

    //CFreeTypeFont::copyToTexture(width, height, expanded_data, &charDscr);

    //m_charInfo[_char] = charDscr;

    //delete[] expanded_data;
    //FT_Done_Glyph(glyph);

    return true;
}