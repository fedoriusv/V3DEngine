#ifndef _V3D_SHAPE_H_
#define _V3D_SHAPE_H_

#include "scene/Node.h"
#include "renderer/Material.h"
#include "renderer/Geometry.h"
#include "renderer/RenderJob.h"
#include "renderer/DebugGeometry.h"

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
        eRectangleShape,

        eCountShape
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CShape : public CNode
    {
    public:

        CShape();
        virtual                         ~CShape();

        EShapeType                      getShapeType() const;
                                        
        void                            setMaterial(const renderer::MaterialPtr& material);
        const renderer::MaterialPtr&    getMaterial() const;

        const renderer::RenderJobPtr&   getRenderJob() const;

        void                            init()             override;
        void                            update(s32 time)   override;
        void                            render()           override;

    protected:

        virtual void                    refresh() = 0;

        EShapeType                      m_shapeType;
                                        
        renderer::MaterialPtr           m_material;
        renderer::GeometryPtr           m_geometry;
        renderer::RenderJobPtr          m_renderJob;
#ifdef _DEBUG                           
        renderer::DebugGeometryPtr      m_debug;
#endif

        SVertexData&                   getGeometryData();
        renderer::CGeometry::EDrawMode getGeometryDrawMode() const;
        void                           setGeometryDrawMode(renderer::CGeometry::EDrawMode mode);

    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_SHAPE_H_
