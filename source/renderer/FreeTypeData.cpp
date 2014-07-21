#include "FreeTypeData.h"
#include "utils/Logger.h"
#include "scene/TextureManager.h"

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
    , m_fontSize(32U)

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

void CFreeTypeData::setFontSize(u32 size)
{
    m_fontSize = size;
}

const CFreeTypeData::SCharDesc& CFreeTypeData::getCharInfo(const s32 charCode) const
{
    return m_charInfo.find(charCode)->second;
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

    FT_Set_Pixel_Sizes(m_Face, m_fontSize, m_fontSize);

    for (u32 i = 0; i < k_mapSize; ++i)
    {
        if (m_charList[i])
        {
            if (!loadCharToMap(i))
            {
                LOG_ERROR("FreeTypeData: Error create symbol %c", reinterpret_cast<c8*>(i));
            }
        }
    }

    if (m_charInfo.size() == 0)
    {
        LOG_WARRNING("FreeTypeData: Empty Char List");
        return false;
    }

    m_loaded = true;

    return true;
}

void CFreeTypeData::refresh()
{
    if (!m_regenerateMap)
    {
        return;
    }

    CFreeTypeData::loadCharList();
}

bool CFreeTypeData::loadCharToMap(u32 charId)
{
    if (m_charInfo.find(charId) != m_charInfo.end())
    {
        return true;
    }

    static FT_Fixed outline_thikness = 0 * 64;
    FT_Glyph        glyph;
    FT_Stroker      stroker;

    if (FT_Load_Glyph(m_Face, charId/*FT_Get_Char_Index(m_Face, charId)*/, FT_LOAD_DEFAULT))
    {
        return false;
    }

    if (FT_Load_Char(m_Face, charId/*FT_Get_Char_Index(m_Face, charId)*/, FT_LOAD_RENDER))
    {
        return false;
    }

    FT_GlyphSlot glyphSlot = m_Face->glyph;

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
        //return false;
    }

    if (FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL/*FT_RENDER_MODE_LCD*/, 0, 1))
    {
        return false;
    }

    FT_BitmapGlyph ft_bitmap_glyph = (FT_BitmapGlyph)glyph;
    FT_Stroker_Done(stroker);

    CFreeTypeData::fillCharInfo(m_charInfo[charId], ft_bitmap_glyph, glyphSlot, outline_thikness);
    
    FT_Done_Glyph(glyph);

    return true;
}

void CFreeTypeData::fillCharInfo(SCharDesc& charDesc, const FT_BitmapGlyph btGlyph, const FT_GlyphSlot glSlot, const FT_Fixed fixed)
{
    f32 scale = 1.0f;
    f32 fontHeight = 1.0f;

    charDesc._advY = (s32)((glSlot->advance.y + fixed) / 64.f * scale);
    charDesc._advX = (s32)((glSlot->advance.x + fixed) / 64.f * scale);
    charDesc._bearingX = (s32)(btGlyph->left * scale);
    charDesc._bearingY = (s32)(fontHeight / 1.2f - btGlyph->top * scale);


   /* f32 iAdvX = glSlot->advance.x >> 6;
    f32 iBearingX = glSlot->metrics.horiBearingX >> 6;
    f32 iCharWidth = glSlot->metrics.width >> 6;
    f32 iAdvY = (glSlot->metrics.height - glSlot->metrics.horiBearingY) >> 6;
    f32 iBearingY = glSlot->metrics.horiBearingY >> 6;
    f32 iCharHeight = glSlot->metrics.height >> 6;*/

    u32 width = next_p2(btGlyph->bitmap.width);
    u32 height = next_p2(btGlyph->bitmap.rows);
    u8* data = new u8[2 * width * height];

    for (u32 j = 0; j < height; j++)
    {
        for (u32 i = 0; i < width; i++)
        {
            data[2 * (i + j * width)] = data[2 * (i + j * width) + 1] =
               (i >= btGlyph->bitmap.width) || (j  >= btGlyph->bitmap.rows) ? 0 : btGlyph->bitmap.buffer[i + btGlyph->bitmap.width * j];
        }
    }

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

    charDesc._bearingX = m_xOffTextures;
    charDesc._bearingY = m_yOffTextures;
    charDesc._width = width;
    charDesc._height = height;
    charDesc._page = m_currentTextureIndex;

    if (m_currentTextureIndex >= m_charMaterial.size())
    {
        TexturePtr texture = CTextureManager::getInstance()->createTexture2DFromData(Dimension2D(k_texWidth, k_texHight), eDepthComponent, eUnsignedByte, NULL);
        texture->setFilterType(ETextureFilter::eLinear, ETextureFilter::eLinear);
        texture->setWrap(EWrapType::eClampToEdge);

        m_charMaterial.push_back(texture);
    }

    CTextureManager::getInstance()->copyToTexture2D(m_charMaterial[m_currentTextureIndex], Dimension2D(m_xOffTextures, m_yOffTextures), Dimension2D(width, height), eLuminanceAlpha, data);

    m_xOffTextures += width + 1;

    delete[] data;
    data = nullptr;
}