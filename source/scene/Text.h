#ifndef _V3D_TEXT_H_
#define _V3D_TEXT_H_

#include "Node.h"
#include "resources/FontData.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CText : public CNode
    {
    public:

        enum EAlignMode
        {
            eFontAlignLeft          = 0,
            eFontAlignCenter        = 1,
            eFontAlignRight         = 2,
        };

        CText(const std::string& font);
        virtual                         ~CText();

        void                            setText(const std::string& text);
        const std::string&              getText() const;

        void                            setSize(u32 size);
        u32                             getSize() const;

        void                            setAlignMode(EAlignMode mode);
        EAlignMode                      getAlignMode() const;

        void                            init()                          override;
        void                            update(s32 time)                override;
        void                            render()                        override;

    protected:

        void                            refresh();
        void                            build();

        f32                             getTextWidth();
        f32                             adjustForKerningPairs(const resources::CFontData::SCharDesc* info, s32 charId);

        std::string                     m_text;
        u32                             m_size;
        EAlignMode                      m_align;

        const f32                       k_textScale;
        const f32                       k_spacing;

        std::string                     m_font;
        resources::FontDataPtr          m_data;

    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CText>          FontPtr;
    typedef std::map<std::string, FontPtr>  FontList;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_TEXT_H_