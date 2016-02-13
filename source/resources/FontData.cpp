#include "FontData.h"

namespace v3d
{
namespace resources
{

CFontData::CFontData(const std::string& font)
    : m_fontType(EFontType::eUnknownFont)
    , m_mapSize(k_fontMapSize, k_fontMapSize)
    , m_font(font)
    , m_fontSize(32U)
{
}

CFontData::~CFontData()
{
    m_charTexture.clear(); //Texture deleted form texture manager
    m_charInfo.clear();
}

CFontData::EFontType CFontData::getFontType() const
{
    return m_fontType;
}

void CFontData::setFontType(EFontType type)
{
    m_fontType = type;
}

const CFontData::SCharDesc* CFontData::getCharInfo(s32 charCode) const
{
    std::map<s32, SCharDesc>::const_iterator it = m_charInfo.find(charCode);
    if (it == m_charInfo.end())
    {
        return nullptr;
    };

    return &it->second;
}

const std::string& CFontData::getFontName() const
{
    return m_font;
}

void CFontData::setFontSize(u32 size)
{
    m_fontSize = size;
}

const u32 CFontData::getFontSize() const
{
    return m_fontSize;
}

} //namespace resources
} //namespace v3d
