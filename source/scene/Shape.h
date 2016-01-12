#ifndef _V3D_SHAPE_H_
#define _V3D_SHAPE_H_

#include "scene/Node.h"
#include "renderer/Renderable.h"

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

    class CShape : public CNode, public renderer::CRenderable
    {
    public:

        CShape();
        virtual                         ~CShape();

        EShapeType                      getShapeType() const;
                                        
        void                            init()             override;
        void                            update(s32 time)   override;
        void                            render()           override;

    protected:

        EShapeType                     m_shapeType;

        SVertexData&                   getGeometryData();
        renderer::EPrimitivesMode      getGeometryDrawMode() const;
        void                           setGeometryDrawMode(renderer::EPrimitivesMode mode);

    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_SHAPE_H_
