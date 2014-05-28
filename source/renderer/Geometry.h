#ifndef _F3D_GEOMETRY_H_
#define _F3D_GEOMETRY_H_

#include "common.h"
#include "renderer/ShaderData.h"

namespace f3d
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

        CGeometry(const ShaderDataPtr& shaderData);
        virtual           ~CGeometry();

        virtual void      init()   = 0;
        virtual void      update() = 0;
        virtual void      free()   = 0;

    protected:

        friend            f3d::scene::CShape;

        EDrawMode         m_drawMode;
        SVertexData       m_data;
        ShaderDataPtr     m_shaderData;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CGeometry> GeometryPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_F3D_GEOMETRY_H_