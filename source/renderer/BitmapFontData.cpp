#include "BitmapFontData.h"
#include "utils/Logger.h"
#include "scene/TextureManager.h"
#include "tinyxml2.h"

using namespace v3d;
using namespace v3d::renderer;

CBitmapFontData::CBitmapFontData(const std::string& font)
    : CFontData(font)
{
    m_fontType = EFontType::eBitmapFont;
}

CBitmapFontData::~CBitmapFontData()
{
}

void CBitmapFontData::init(stream::IStream* stream)
{
    CResource::setStream(stream);
}

bool CBitmapFontData::load()
{
    stream::IStream* stream = CResource::getStream();
    if (!stream)
    {
        LOG_ERROR("Empty Stream with name [%s] form Bitmap Font", CResource::getResourseName().c_str());
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
        LOG_ERROR("Empty Data from Stream with name [%s] form Bitmap Font", CResource::getResourseName().c_str());
        return false;
    }

    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError success = doc.Parse(resource.c_str());

    if (success)
    {
        LOG_ERROR("Error Parse Stream name [%s] form  Bitmap Font", CResource::getResourseName().c_str());
        return false;
    }

    tinyxml2::XMLElement* fontElement = doc.FirstChildElement("font");
    if (!fontElement)
    {
        LOG_ERROR("Can not find font in Stream name [%s] form Bitmap Font", CResource::getResourseName().c_str());
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

    TexturePtr texture = scene::CTextureManager::getInstance()->load(file);
    if (!texture)
    {
        LOG_ERROR("BitmapFontData Error parse. Load file %s", file.c_str());
        return false;
    }

    if (m_charTexture.size() < pageId + 1)
    {
        m_charTexture.resize(pageId + 1);
    }

    m_charTexture[pageId] = texture;

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

        charDecs._advX = x;
        charDecs._advY = y;
        charDecs._height = height;
        charDecs._width = width;
        charDecs._bearingX = xoffset;
        charDecs._bearingY = yoffset;
        charDecs._page = page;

        m_charInfo.insert(std::map<s32, SCharDesc>::value_type(id, charDecs));

        varElement = varElement->NextSiblingElement("char");
    }

    return true;
}