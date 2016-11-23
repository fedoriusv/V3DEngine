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

    /**
    * Bitmap Font data realization.
    */
    class CBitmapFontData : public CFontData
    {
    public:

        CBitmapFontData(const std::string& font);
        ~CBitmapFontData();

        void    init(const stream::IStreamPtr stream)  override;
        bool    load()                                  override;

    private:

        bool    addCharsToMap(const std::string& text)  override final;
        bool    loadFont(const std::string& resource)   override final;

        bool    parse(tinyxml2::XMLElement* root);
        bool    parseInfo(tinyxml2::XMLElement* root);
        bool    parseCommon(tinyxml2::XMLElement* root);
        bool    parsePages(tinyxml2::XMLElement* root);
        bool    parseChars(tinyxml2::XMLElement* root);
        bool    parseKernings(tinyxml2::XMLElement* root);
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace resources
} //namespace v3d

#endif //_V3D_BITMAP_FONT_DATA_H_
