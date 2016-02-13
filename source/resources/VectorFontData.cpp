#include "VectorFontData.h"
#include "utils/Logger.h"
#include "scene/TextureManager.h"

namespace v3d
{
namespace resources
{

using namespace core;
using namespace scene;
using namespace renderer;

inline int next_p2(int n)
{
    int res = 1;
    while (res < n)
    {
        res <<= 1;
    }

    return res;
}


CVectorFontData::CVectorFontData(const std::string& font)
    : CFontData(font)
    , m_offetTextures(0U, 0U)
    , m_currentTextureIndex(0U)
    , m_loaded(false)
{
    for (u32 i = 0; i < k_fontMapSize; ++i)
    {
        m_charList[i] = false;
    }
}

CVectorFontData::~CVectorFontData()
{
    FT_Done_Face(m_Face);
    FT_Done_FreeType(m_Library);
}

bool CVectorFontData::addCharsToMap(const std::string& text)
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

    if (haveNew)
    {
        if (!CVectorFontData::loadCharList())
        {
            return false;
        }
    }

    return haveNew;
}

void CVectorFontData::init(const stream::IStreamPtr& stream)
{
    CResource::setStream(stream);
}

bool CVectorFontData::load()
{
    const stream::IStreamPtr stream = CResource::getStream();
    if (!stream)
    {
        LOG_ERROR("Empty Stream with name [%s] form Texture", CResource::getResourseName().c_str());
        return false;
    }

    bool success = loadFont(CResource::getResourseName());
    stream->close();

    return success;
}

bool CVectorFontData::loadFont(const std::string& resource)
{

    if (resource.empty())
    {
        ASSERT(false, "FreeTypeFont: empty name");
        return false;
    }

    if (m_loaded)
    {
        ASSERT(false, "FreeTypeFont: Already loaded");
        return false;
    }

    FT_Error error = FT_Init_FreeType(&m_Library);
    if (error)
    {
        ASSERT(false, "FreeTypeFont: Error Init Free Type");
        return false;
    }

    error = FT_New_Face(m_Library, resource.c_str(), 0, &m_Face);
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

        ASSERT(false, "FreeTypeFont:: Font Error");
        return false;
    }

    /*if (!CVectorFontData::loadCharList())
    {
        return false;
    }*/

    return true;
}

bool CVectorFontData::loadCharList()
{
    m_loaded = false;

    FT_Set_Pixel_Sizes(m_Face, m_fontSize, m_fontSize);

    for (u32 i = 0; i < k_fontMapSize; ++i)
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
        LOG_WARNING("FreeTypeData: Empty Char List");
    }

    m_loaded = true;

    return true;
}

bool CVectorFontData::loadCharToMap(u32 charId)
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

    CVectorFontData::fillCharInfo(m_charInfo[charId], ft_bitmap_glyph, glyphSlot, outline_thikness);
    
    FT_Done_Glyph(glyph);

    return true;
}

void CVectorFontData::fillCharInfo(SCharDesc& charDesc, const FT_BitmapGlyph btGlyph, const FT_GlyphSlot glSlot, const FT_Fixed fixed)
{
    const f32 scale = 1.0f;
    const f32 fontHeight = 1.0f;
    const FT_Bitmap& bitmap = btGlyph->bitmap;

    charDesc._advY = (s32)((glSlot->advance.y + fixed) / 64.f * scale);
    charDesc._advX = (s32)((glSlot->advance.x + fixed) / 64.f * scale);
    charDesc._offX = (s32)(btGlyph->left * scale);
    charDesc._offY = (s32)(fontHeight / 1.2f - btGlyph->top * scale);

    u32 width = next_p2(btGlyph->bitmap.width);
    u32 height = next_p2(btGlyph->bitmap.rows);
    //u32 width = btGlyph->bitmap.width;
    //u32 height = btGlyph->bitmap.rows;

    u8* data = new u8[2 *width * height];
    memset(data, 0U, 2 * width * height);

    //TODO: incorrect fill data, need rework
    for (u32 j = 0; j < height; j++)
    {
        for (u32 i = 0; i < width; i++)
        {
            if (j < (u32)bitmap.width && i < (u32)bitmap.rows)
            {
                data[2 * (i + j * width)] = bitmap.buffer[i + (u32)bitmap.width * j];
                data[2 * (i + j * width) + 1] = bitmap.buffer[i + (u32)bitmap.width * j];
            }
        }
    }

   /* for (u32 j = 0; j < height; j++)
    {
        for (u32 i = 0; i < width; i++)
        {
            data[2 * (i + j * width)] = data[2 * (i + j * width) + 1] =
               (i >= (u32)btGlyph->bitmap.width) || (j  >= (u32)btGlyph->bitmap.rows) ? 0 : btGlyph->bitmap.buffer[i + (u32)btGlyph->bitmap.width * j];
        }
    }*/

    static u32 lineHeight;
    if (lineHeight < height)
    {
        lineHeight = height;
    }

    if (m_offetTextures.width + width >= k_fontMapSize)
    {
        m_offetTextures.width = 0;
        m_offetTextures.height += lineHeight + 1;
    }

    if (m_offetTextures.height + lineHeight >= k_fontMapSize)
    {
        m_offetTextures.height = 0;
        m_currentTextureIndex++;
    }

    charDesc._srcX = m_offetTextures.width;
    charDesc._srcY = m_offetTextures.height;
    charDesc._width = width;
    charDesc._height = height;
    charDesc._page = m_currentTextureIndex;

    if (m_currentTextureIndex >= m_charTexture.size())
    {
        CTexture* texture = CTextureManager::getInstance()->createTexture2DFromData(Dimension2D(k_fontMapSize, k_fontMapSize), EImageFormat::eRed, EImageType::eUnsignedByte, nullptr);
        texture->setFilterType(ETextureFilter::eLinear, ETextureFilter::eLinear);
        texture->setWrap(EWrapType::eClampToEdge);

        u32 value = 0U;
        texture->fill(Dimension2D(0U, 0U), Dimension2D(0U, 0U), (void*)&value);

        m_charTexture.push_back(texture);
    }

    m_charTexture[m_currentTextureIndex]->updateData(m_offetTextures, Dimension2D(width, height * 2), data);

    m_offetTextures.width += width + 1;

    delete[] data;
    data = nullptr;
}

} //namespace resources
} //namespace v3d
