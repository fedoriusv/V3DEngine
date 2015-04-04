#ifndef _V3D_BILLBOARD_H_
#define _V3D_BILLBOARD_H_

#include "Node.h"

#include "renderer/Material.h"
#include "renderer/RenderJob.h"
#include "renderer/Geometry.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CBillboard : public CNode
    {
    public:

        CBillboard(const std::string& texture);
        ~CBillboard();

        void                            render()            override;
        void                            update(s32 time)    override;

        void                            init()              override;

        void                            setMaterial(const renderer::MaterialPtr& material);
        const renderer::MaterialPtr&    getMaterial() const;

        const renderer::RenderJobPtr&   getRenderJob() const;

    private:

        void                            build();

        renderer::MaterialPtr           m_material;
        renderer::GeometryPtr           m_geometry;
        renderer::RenderJobPtr          m_renderJob;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_BILLBOARD_H_