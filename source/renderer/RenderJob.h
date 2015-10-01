#ifndef _V3D_RENDERJOB_H_
#define _V3D_RENDERJOB_H_

#include "Geometry.h"
#include "RenderTarget.h"
#include "Material.h"

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CRenderJob
    {
    public:

        CRenderJob(const MaterialPtr& material, const GeometryPtr& geometry, const core::Matrix4D& transform);
        ~CRenderJob();

        const MaterialPtr&      getMaterial() const;
        MaterialPtr&            getMaterial();

        const GeometryPtr&      getGeometry() const;

        const core::Matrix4D&   getTransform() const;

        void                    setRenderTarget(u32 target);
        u32                     getRenderTarget() const;

        void                    setTransform(const core::Matrix4D& transform);

    protected:

        void                    setMaterial(const MaterialPtr& material);
        void                    setGeometry(const GeometryPtr& geometry);

    private:

        MaterialPtr             m_material;
        GeometryPtr             m_geometry;
        core::Matrix4D          m_transform;
        u32                     m_targetIndex;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CRenderJob> RenderJobPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_RENDERJOB_H_
