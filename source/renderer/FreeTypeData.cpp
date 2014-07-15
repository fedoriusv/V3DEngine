#include "FreeTypeData.h"
#include "utils/Logger.h"

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

    , m_xOffTextures(0U)
    , m_yOffTextures(0U)
    , m_currentTextureIndex(0U)
{
    for (u32 i = 0; i < k_mapSize; ++i)
    {
        m_charTextures[i] = nullptr;
        m_charList[i] = false;
    }
}

CFreeTypeData::~CFreeTypeData()
{
    FT_Done_Face(m_Face);
    FT_Done_FreeType(m_Library);
}

const std::string& CFreeTypeData::getFontName() const
{
    return m_font;
}

bool CFreeTypeData::findCharsOnMap(const std::string& text)
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

bool CFreeTypeData::addCharsToMap(const std::string& text)
{
    bool haveNew = false;

    for (std::string::const_iterator chr = text.begin(); chr < text.end(); ++chr)
    {
        if (!m_charList[(*chr)])
        {
            haveNew = true;
        }
        m_charList[(*chr)] = true;
    }

    m_regenerateMap = haveNew;

    return haveNew;
}

void CFreeTypeData::init(stream::IStream* stream)
{
    CResource::setStream(stream);
}

bool CFreeTypeData::load()
{
    stream::IStream* stream = CResource::getStream();
    if (!stream)
    {
        LOG_ERROR("Empty Stream with name [%s] form Texture", CResource::getResourseName().c_str());
        return false;
    }

    bool success = loadFreeType(CResource::getResourseName());
    stream->close();

    return success;
}

bool CFreeTypeData::loadFreeType(const std::string& font)
{
    if (font.empty())
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

    error = FT_New_Face(m_Library, font.c_str(), 0, &m_Face);
    if (error)
    {

        std::function<const char*(FT_Error)> strError = [](FT_Error error)
        {
            switch (error)
            {
            case FT_Err_Cannot_Open_Resource:
                return "cannot open resource";
            case FT_Err_Unknown_File_Format:
                return "unknown file format";
            case FT_Err_Invalid_File_Format:
                return "broken file";
            case FT_Err_Invalid_Version:
                return "invalid FreeType version";
            default:
                return "unknown error";
            }
        };
        LOG_ERROR("FreeTypeFont: Font Error : %s", strError(error));

        ASSERT(false && "FreeTypeFont:: Font Error");
        return false;
    }

    if (!CFreeTypeData::loadCharList())
    {
        return false;
    }

    return true;
}

bool CFreeTypeData::loadCharList()
{
    m_loaded = false;

    int iPXSize = 32;
    m_loadedPixelSize = iPXSize;
    FT_Set_Pixel_Sizes(m_Face, iPXSize, iPXSize);

    //loadCharToMap('A');
    //for (u32 i = 0; i < k_mapSize; ++i)
    //{
    //    if (m_charList[i])
    //    {
    //        if (!loadCharToMap(i))
    //        {
    //            LOG_ERROR("FreeTypeData: Error create symbol %c", reinterpret_cast<c8*>(i));
    //        }
    //    }
    //}

    //ftFace->style_flags = ftFace->style_flags | FT_STYLE_FLAG_ITALIC;
    //if(FT_Set_Char_Size(m_FontFace, size << 6, size << 6, 96, 96) != 0)
    //http://www.asciitable.com

    //FT_UInt num_chars = 128;
    //m_vertices.malloc(4 * num_chars);

    for (u32 glyphIndex = 32/*' '*/; glyphIndex < 126/*~*/; ++glyphIndex)
    {
        CFreeTypeData::createChar(m_Face, glyphIndex);
    }
    m_loaded = true;

    //RENDERER->initBufferObjects(m_vertices);
    ////m_vertices.clear();
}

void CFreeTypeData::refresh()
{
    if (!m_regenerateMap)
    {
        return;
    }

    CFreeTypeData::loadCharList();
}

void CFreeTypeData::copyToTexture(u32 width, u32 height, u8* data, SCharDesc* charDesc)
{
    static u32 lineHeight;
    if (lineHeight < height)
    {
        lineHeight = height;
    }

    if (m_xOffTextures + width >= k_texWidth)
    {
        m_xOffTextures = 0;
        m_yOffTextures += lineHeight + 1;
    }

    if (m_yOffTextures + lineHeight >= k_texHight)
    {
        m_yOffTextures = 0;
        m_currentTextureIndex++;
    }

    //if (m_currentTextureIndex >= m_charMaterial.size())
    //{
        // Create big texture
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

void CFreeTypeData::createChar(const FT_Face& face, FT_UInt glyphIndex)
{
    FT_Load_Glyph(face, FT_Get_Char_Index(m_Face, glyphIndex), FT_LOAD_DEFAULT);
    FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);

    FT_Stroker stroker;
    FT_Stroker_New(m_Library, &stroker);

    FT_Glyph glyph;
    FT_Get_Glyph(face->glyph, &glyph);
    FT_Glyph_StrokeBorder(&glyph, stroker, 0, 5);

    FT_Bitmap* pBitmap = &face->glyph->bitmap;

    u32 width = pBitmap->width;
    u32 hight = pBitmap->rows;

    u32 * bData = new u32[width*hight];
    memset(bData, NULL, width * hight * sizeof(u32));
    memcpy(bData, pBitmap->buffer, sizeof(u32)* width * hight);

    //texture
    if (m_charTextures[glyphIndex])
    {
        m_charTextures[glyphIndex] = nullptr;
    }
    
    //m_charTextures[glyphIndex] = TEXTURE_MGR->createTexture2DFromData(width, hight, IF_DEPTH_COMPONENT, IT_UNSIGNED_BYTE, bData);
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
    if (m_charInfo.find(charId) != m_charInfo.end())
    {
        return true;
    }

    static FT_Fixed outline_thikness = 0 * 64;
    FT_Glyph        glyph;
    FT_GlyphSlot    glyphSlot;
    FT_Stroker      stroker;

    if (FT_Load_Glyph(m_Face, charId/*FT_Get_Char_Index(m_Face, _char)*/, FT_LOAD_DEFAULT))
    {
        return false;
    }
    glyphSlot = m_Face->glyph;

    if (FT_Load_Char(m_Face, charId/*FT_Get_Char_Index(m_Face, _char)*/, FT_LOAD_RENDER))
    {
        return false;
    }

    if (FT_Stroker_New(m_Library, &stroker))
    {
        return false;
    }

    FT_Stroker_Set(stroker, outline_thikness, FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);
    if (FT_Get_Glyph(m_Face->glyph, &glyph))
    {
        return false;
    }

    if (FT_Glyph_StrokeBorder(&glyph, stroker, 0, 1))//FT_Glyph_Stroke( &glyph, stroker, 1 ))
    {
        //			return false;
    }

    if (FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL/*FT_RENDER_MODE_LCD*/, 0, 1))
    {
        return false;
    }

    FT_BitmapGlyph ft_bitmap_glyph = (FT_BitmapGlyph)glyph;
    FT_Stroker_Done(stroker);

    f32 scale = 1.0f;
    f32 fontHeight = 1.0f;

    SCharDesc charDscr;
    charDscr._advX = (s32)((glyphSlot->advance.x + outline_thikness) / 64.f * scale);
    charDscr._bearingX = (s32)(ft_bitmap_glyph->left * scale);
    charDscr._bearingY = (s32)(fontHeight / 1.2f - ft_bitmap_glyph->top * scale);

    u32 width = next_p2(ft_bitmap_glyph->bitmap.width);
    u32 height = next_p2(ft_bitmap_glyph->bitmap.rows);
    u8* expanded_data = new u8[2 * width * height];
    for (u32 j = 0; j < height; j++)
    {
        for (u32 i = 0; i < width; i++)
        {
            expanded_data[2 * (i + j * width)] = expanded_data[2 * (i + j * width) + 1] =
                (i >= ft_bitmap_glyph->bitmap.width || j >= ft_bitmap_glyph->bitmap.rows) ?
                0 : ft_bitmap_glyph->bitmap.buffer[i + ft_bitmap_glyph->bitmap.width * j];
        }
    }

    CFreeTypeData::copyToTexture(width, height, expanded_data, &charDscr);

    m_charInfo[charId] = charDscr;

    delete[] expanded_data;
    expanded_data = nullptr;

    FT_Done_Glyph(glyph);

    return true;
}