#ifndef _V3D_FREETYPE_FONT_H_
#define _V3D_FREETYPE_FONT_H_

#include "Font.h"
#include "renderer/FreeTypeData.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CFreeTypeFont : public CFont
    {
    public:

        CFreeTypeFont(const std::string& font);
        virtual                         ~CFreeTypeFont();

        void                            init()                          override;
        void                            update(f64 time)                override;
        void                            render()                        override;

    private:

        std::string                     m_font;
        renderer::FreeTypeDataPtr       m_data;

        bool                            load(const std::string& font)   override;

    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_FREETYPE_FONT_H_