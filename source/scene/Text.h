#ifndef _V3D_TEXT_H_
#define _V3D_TEXT_H_

#include "Node.h"

#include "renderer/Geometry.h"
#include "renderer/Material.h"
#include "renderer/RenderJob.h"
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

        void                            setSize(const u32 size);
        u32                             getSize() const;

        void                            setAlignMode(EAlignMode mode);
        EAlignMode                      getAlignMode() const;

        void                            setMaterial(const renderer::MaterialPtr& material);
        const renderer::MaterialPtr&    getMaterial() const;

        const renderer::RenderJobPtr&   getRenderJob() const;

        void                            init()                          override;
        void                            update(f64 time)                override;
        void                            render()                        override;

    protected:

        void                            refresh();
        void                            build();

        f32                             getTextWidth();
        f32                             adjustForKerningPairs(const resources::CFontData::SCharDesc* info, const s32 charId);

        std::string                     m_text;
        u32                             m_size;
        EAlignMode                      m_align;

        const f32                       k_textScale;
        const f32                       k_spacing;

        renderer::MaterialPtr           m_material;
        renderer::GeometryPtr           m_geometry;
        renderer::RenderJobPtr          m_renderJob;

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