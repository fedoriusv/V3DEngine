#ifndef _V3D_FONT_H_
#define _V3D_FONT_H_

#include "Node.h"

#include "renderer/Geometry.h"
#include "renderer/Material.h"
#include "renderer/RenderJob.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CFont : public CNode
    {
    public:

        CFont();
        virtual                         ~CFont();

        virtual void                    setText(const std::string& text);
        const std::string&              getText() const;

        virtual void                    setSize(const u32 size);
        u32                             getSize() const;

        void                            setMaterial(const renderer::MaterialPtr& material);
        const renderer::MaterialPtr&    getMaterial() const;

        void                            init()                          override;
        void                            update(f64 time)                override;
        void                            render()                        override;

    protected:

        virtual void                    refresh() = 0;

        std::string                     m_text;
        u32                             m_size;

        renderer::MaterialPtr           m_material;
        renderer::GeometryPtr           m_geometry;
        renderer::RenderJobPtr          m_renderJob;

    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CFont>          FontPtr;
    typedef std::map<std::string, FontPtr>  FontList;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_FONT_H_