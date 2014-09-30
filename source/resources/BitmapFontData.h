#ifndef _V3D_BITMAP_FONT_DATA_H_
#define _V3D_BITMAP_FONT_DATA_H_

#include "FontData.h"

namespace tinyxml2
{
    class XMLElement;
}

namespace v3d
{
namespace resources
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CBitmapFontData : public CFontData
    {
    public:

        CBitmapFontData(const std::string& font);
        virtual ~CBitmapFontData();

        void    init(const stream::IStreamPtr& stream)  override;
        bool    load()                                  override;

    private:

        bool    addCharsToMap(const std::string& text)  override;
        bool    loadFont(const std::string& resource)   override;

        bool    parse(tinyxml2::XMLElement* root);
        bool    parseInfo(tinyxml2::XMLElement* root);
        bool    parseCommon(tinyxml2::XMLElement* root);
        bool    parsePages(tinyxml2::XMLElement* root);
        bool    parseChars(tinyxml2::XMLElement* root);
        bool    parseKernings(tinyxml2::XMLElement* root);
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_BITMAP_FONT_DATA_H_