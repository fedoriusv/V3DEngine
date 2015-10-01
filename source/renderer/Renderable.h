#ifndef _V3D_RENDEREBLE_H_
#define _V3D_RENDEREBLE_H_

#include "Geometry.h"
#include "RenderJob.h"
#include "Material.h"

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CRenderable
    {
    public:

        CRenderable();
        virtual             ~CRenderable();

        virtual void        render() = 0;

        void                setMaterial(const MaterialPtr& material);
        const MaterialPtr&  getMaterial() const;

        const RenderJobPtr& getRenderJob() const;
        const GeometryPtr&  getGeometry() const;

    protected:

        void                setRenderJob(const RenderJobPtr& job);
        void                setGeometry(const GeometryPtr& geometry);

    private:

        MaterialPtr         m_material;
        GeometryPtr         m_geometry;
        RenderJobPtr        m_renderJob;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_RENDEREBLE_H_
