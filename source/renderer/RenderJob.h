#ifndef _V3D_RENDERJOB_H_
#define _V3D_RENDERJOB_H_

#include "Material.h"
#include "Geometry.h"
#include "RenderTarget.h"

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CDebugGeometry;
    class CDebugLight;

    class CRenderJob
    {
    public:

        CRenderJob(const MaterialPtr& material, const GeometryPtr& geometry, const core::Matrix4D& transform);
        ~CRenderJob();

        void                    setMaterial(const MaterialPtr& material);
        const MaterialPtr&      getMaterial() const;

        void                    setGeometry(const GeometryPtr& geometry);
        const GeometryPtr&      getGeometry() const;

        void                    setTransform(const core::Matrix4D& transform);
        const core::Matrix4D&   getTransform() const;

        void                    setRenderTarget(u32 target);
        u32                     getRenderTarget() const;

    private:

        friend                  CDebugGeometry;
        friend                  CDebugLight;

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
