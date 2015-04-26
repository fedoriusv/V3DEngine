#ifndef _V3D_RENDEREBLE_H_
#define _V3D_RENDEREBLE_H_

#include "renderer/Geometry.h"
#include "renderer/Material.h"
#include "renderer/RenderJob.h"

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CRendereble
    {
    public:

        CRendereble();
        virtual             ~CRendereble();

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