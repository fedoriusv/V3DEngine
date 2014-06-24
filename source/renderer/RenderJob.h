#ifndef _V3D_RENDERJOB_H_
#define _V3D_RENDERJOB_H_

#include "Material.h"
#include "Geometry.h"

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CDebugDraw;

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

        void                job(const bool updated);

    private:

        friend              CDebugDraw;

        void                updateLight(const ShaderDataPtr& data);
        void                updateMaterial(const ShaderDataPtr& data);
        static void         updateTransform(const ShaderDataPtr& data);

        MaterialPtr         m_material;
        GeometryPtr         m_geomerty;
        Matrix4D            m_transform;

        bool                m_transformUpdate;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CRenderJob> RenderJobPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_RENDERJOB_H_
