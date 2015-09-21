#ifndef _V3D_RENDERJOB_H_
#define _V3D_RENDERJOB_H_

#include "Geometry.h"
#include "RenderTarget.h"

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CMaterial;

    class CRenderJob
    {
    public:

        CRenderJob(const CMaterial* material, const GeometryPtr& geometry, const core::Matrix4D& transform);
        ~CRenderJob();

        void                    setMaterial(const CMaterial* material);
        const CMaterial*        getMaterial() const;
        CMaterial*              getMaterial();

        void                    setGeometry(const GeometryPtr& geometry);
        const GeometryPtr&      getGeometry() const;

        void                    setTransform(const core::Matrix4D& transform);
        const core::Matrix4D&   getTransform() const;

        void                    setRenderTarget(u32 target);
        u32                     getRenderTarget() const;

    private:

        const CMaterial*        m_material;
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
