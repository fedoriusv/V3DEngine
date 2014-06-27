#ifndef _V3D_RENDERJOB_H_
#define _V3D_RENDERJOB_H_

#include "Material.h"
#include "Geometry.h"

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

        CRenderJob(const MaterialPtr& material, const GeometryPtr& geomerty, const Matrix4D& transform);
        ~CRenderJob();

        void                setMaterial(const MaterialPtr& material);
        const MaterialPtr&  getMaterial() const;

        void                setGeomery(const GeometryPtr& geomerty);
        const GeometryPtr&  getGeometry() const;

        void                setTransform(const Matrix4D& transform);
        const Matrix4D&     getTransform() const;

    private:

        friend              CDebugGeometry;
        friend              CDebugLight;

        MaterialPtr         m_material;
        GeometryPtr         m_geomerty;
        Matrix4D            m_transform;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CRenderJob> RenderJobPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_RENDERJOB_H_
