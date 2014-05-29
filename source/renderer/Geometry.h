#ifndef _F3D_GEOMETRY_H_
#define _F3D_GEOMETRY_H_

#include "Vertex.h"
#include "renderer/ShaderAttribute.h"

namespace v3d
{

namespace scene
{
    class CShape;
}

namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    enum class EDrawMode
    {
        eTriangleStrip,
        eTriangles,
        eTriangleFan,
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CGeometry
    {
    public:

        CGeometry(const AttributeList& attributes);
        virtual           ~CGeometry();

        virtual void      init()   = 0;
        virtual void      update() = 0;
        virtual void      free()   = 0;

    protected:

        friend            v3d::scene::CShape;

        EDrawMode         m_drawMode;
        SVertexData       m_data;
        AttributeList     m_attributes;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CGeometry> GeometryPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_F3D_GEOMETRY_H_