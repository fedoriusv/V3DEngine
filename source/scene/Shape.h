#ifndef _V3D_SHAPE_H_
#define _V3D_SHAPE_H_

#include "scene/Node.h"
#include "renderer/Material.h"
#include "renderer/Geometry.h"
#include "renderer/RenderJob.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    enum class EShapeType
    {
        eUnknownShape,
        eSampleShape,
        eCubeShape,
        eSphereShape,
        eCylinderShape,
        eTorusShape,
        ePlaneShape,
        eDiskShape,
        eHeightMapShape,

        eCountShape
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CShape : public CNode
    {
    public:

        CShape();
        virtual                      ~CShape();

        EShapeType                   getShapeType() const;

        void                         setMaterial(const renderer::MaterialPtr& material);
        const renderer::MaterialPtr& getMaterial() const;

        void                         init()  override;

    protected:

        EShapeType                   m_shapeType;

        renderer::MaterialPtr        m_material;
        renderer::GeometryPtr        m_geometry;
        renderer::RenderJobPtr       m_renderJob;

        SVertexData&                getGeometryData();
        renderer::EDrawMode         getGeometryDrawMode() const;
        void                        setGeometryDrawMode(renderer::EDrawMode mode);

    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_SHAPE_H_
