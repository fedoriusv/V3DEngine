#ifndef _V3D_SHAPE_H_
#define _V3D_SHAPE_H_

#include "scene/Node.h"
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
                                        
        void                            init()             override;
        void                            update(s32 time)   override;
        void                            render()           override;

    protected:

        virtual void                    refresh() = 0;

        EShapeType                      m_shapeType;

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
