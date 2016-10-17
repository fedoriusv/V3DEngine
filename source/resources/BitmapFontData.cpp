#include "BitmapFontData.h"
#include "utils/Logger.h"
#include "scene/TextureManager.h"
#include "tinyxml2.h"

namespace v3d
{
namespace resources
{

using namespace renderer;

CBitmapFontData::CBitmapFontData(const std::string& font)
    : CFontData(font)
{
    m_fontType = EFontType::eBitmapFont;
}

CBitmapFontData::~CBitmapFontData()
{
}

void CBitmapFontData::init(const stream::IStreamPtr& stream)
{
    IResource::setStream(stream);
}

bool CBitmapFontData::load()
{
    const stream::IStreamPtr stream = IResource::getStream();
    if (!stream)
    {
        LOG_ERROR("Empty Stream with name [%s] form Bitmap Font", IResource::getResourseName().c_str());
        return false;
    }

    std::string data;

    u32 size = stream->size();
    if (size > 0)
    {
        stream->seekBeg(0);
        stream->read(data);
    }
    stream->close();

    bool success = loadFont(data);
    stream->close();

    return success;
}

bool CBitmapFontData::loadFont(const std::string& resource)
{
    if (resource.empty())
    {
        LOG_ERROR("Empty Data from Stream with name [%s] form Bitmap Font", IResource::getResourseName().c_str());
        return false;
    }

    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError success = doc.Parse(resource.c_str());

    if (success)
    {
        LOG_ERROR("Error Parse Stream name [%s] form  Bitmap Font", IResource::getResourseName().c_str());
        return false;
    }

    tinyxml2::XMLElement* fontElement = doc.FirstChildElement("font");
    if (!fontElement)
    {
        LOG_ERROR("Can not find font in Stream name [%s] form Bitmap Font", IResource::getResourseName().c_str());
        return false;
    }

    return parse(fontElement);
}

bool CBitmapFontData::parse(tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("Bitmap Font Error parse. Cannot read font element");
        return false;
    }

    tinyxml2::XMLElement* infoElement = root->FirstChildElement("info");
    if (!infoElement)
    {
        LOG_ERROR("BitmapFontData Error parse. Have no info section");
        return false;
    }
    if (!parseInfo(infoElement))
    {
        return false;
    }

    tinyxml2::XMLElement* commonElement = root->FirstChildElement("common");
    if (!commonElement)
    {
        LOG_ERROR("BitmapFontData Error parse. Have no common section");
        return false;
    }
    if (!parseCommon(commonElement))
    {
        return false;
    }

    tinyxml2::XMLElement* pagesElement = root->FirstChildElement("pages");
    if (!pagesElement)
    {
        LOG_ERROR("BitmapFontData Error parse. Have no pages section");
        return false;
    }
    if (!parsePages(pagesElement))
    {
        return false;
    }

    tinyxml2::XMLElement* charsElement = root->FirstChildElement("chars");
    if (!charsElement)
    {
        LOG_ERROR("BitmapFontData Error parse. Have no chars section");
        return false;
    }
    if (!parseChars(charsElement))
    {
        return false;
    }

    tinyxml2::XMLElement* kerningsElement = root->FirstChildElement("kernings");
    if (!kerningsElement)
    {
        LOG_ERROR("BitmapFontData Error parse. Have no chars section");
        return false;
    }
    if (!parseKernings(kerningsElement))
    {
        return false;
    }

    return true;
}

bool CBitmapFontData::parseInfo(tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("Bitmap Font Error parse. Cannot read font element");
        return false;
    }

    const std::string faceName = root->Attribute("face");
    if (faceName.empty())
    {
        LOG_ERROR("BitmapFontData Error parse. Have no face name");
        return false;
    }

    m_font = faceName;

    const s32 size = root->IntAttribute("size");
    if (size <= 0)
    {
        LOG_ERROR("BitmapFontData Error parse. Invalid Font Size");
        return false;
    }

    m_fontSize = size;

    //TODO: if need more info

    return true;
}

bool CBitmapFontData::parseCommon(tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("Bitmap Font Error parse. Cannot read common element");
        return false;
    }

    const s32 width = root->IntAttribute("scaleW");
    const s32 height = root->IntAttribute("scaleH");
    if (width <= 0 || height <= 0)
    {
        LOG_ERROR("BitmapFontData Error parse. Invaild Size Width or Height");
        return false;
    }

    const s32 pages = root->IntAttribute("pages");
    if (pages <= 0)
    {
        LOG_ERROR("BitmapFontData Error parse. Invalid Pages count");
        return false;
    }

    m_mapSize.width = width;
    m_mapSize.height = height;

    m_charTexture.resize(pages);

    return true;
}

bool CBitmapFontData::parsePages(tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("Bitmap Font Error parse. Cannot read pages element");
        return false;
    }

    tinyxml2::XMLElement* pageElement = root->FirstChildElement("page");
    if (!pageElement)
    {
        LOG_ERROR("BitmapFontData Error parse. Have no page section");
        return false;
    }

    const s32 pageId = pageElement->IntAttribute("id");
    if (pageId < 0)
    {
        LOG_ERROR("BitmapFontData Error parse. Invalid Page id");
        return false;
    }

    if (!pageElement->Attribute("file"))
    {
        LOG_ERROR("BitmapFontData Error parse. Invalid File Page Name");
        return false;
    }
    const std::string file = pageElement->Attribute("file");

    const std::string& folder = getResourseFolder();
    scene::CTextureManager::getInstance()->registerPath(folder);

    const TexturePtr texture = scene::CTextureManager::getInstance()->load(file);
    if (!texture)
    {
        LOG_ERROR("BitmapFontData Error parse. Load file %s", file.c_str());
        return false;
    }

    if (m_charTexture.size() < static_cast<u32>(pageId) + 1)
    {
        LOG_ERROR("BitmapFontData Error parse. Invalid Page id");
        return false;
    }

    m_charTexture[pageId] = utils::const_pointer_cast<Texture>(texture);

    return true;
}

bool CBitmapFontData::parseChars(tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("Bitmap Font Error parse. Cannot read chars element");
        return false;
    }

    tinyxml2::XMLElement* varElement = root->FirstChildElement("char");
    while (varElement)
    {
        const s32 id = varElement->IntAttribute("id");
       
        const s32 x = varElement->IntAttribute("x");
        const s32 y = varElement->IntAttribute("y");
        const s32 width = varElement->IntAttribute("width");
        const s32 height = varElement->IntAttribute("height");
        const s32 xoffset = varElement->IntAttribute("xoffset");
        const s32 yoffset = varElement->IntAttribute("yoffset");
        const s32 xadvance = varElement->IntAttribute("xadvance");
        const s32 page = varElement->IntAttribute("page");

        SCharDesc charDecs;

        charDecs._srcX = x;
        charDecs._srcY = y;
        charDecs._height = height;
        charDecs._width = width;
        charDecs._offX = xoffset;
        charDecs._offY = yoffset;
        charDecs._advX = xadvance;
        charDecs._advY = 0;
        charDecs._page = page;

        m_charInfo.insert(std::map<s32, SCharDesc>::value_type(id, charDecs));

        varElement = varElement->NextSiblingElement("char");
    }

    return true;
}

bool CBitmapFontData::parseKernings(tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("Bitmap Font Error parse. Cannot read kernings element");
        return false;
    }

    tinyxml2::XMLElement* varElement = root->FirstChildElement("kerning");
    while (varElement)
    {
        const s32 first = varElement->IntAttribute("first");
        const s32 second = varElement->IntAttribute("second");
        const s32 amount = varElement->IntAttribute("amount");

        if (first >= 0 && first < 256 && getCharInfo(first))
        {
            m_charInfo[first]._kerningPairs.push_back(second);
            m_charInfo[first]._kerningPairs.push_back(amount);
        }

        varElement = varElement->NextSiblingElement("kerning");
    }

    return true;
}

bool CBitmapFontData::addCharsToMap(const std::string& text)
{
    return true;
}

} //namespace resources
} //namespace v3d
