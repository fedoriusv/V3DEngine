#ifndef _V3D_DEBUG_GEOMETRY_GL_H_
#define _V3D_DEBUG_GEOMETRY_GL_H_

#include "renderer/DebugGeometry.h"

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CDebugGeometryGL : public CDebugGeometry
    {
    public:

        CDebugGeometryGL(const GeometryPtr& geometry);
        virtual             ~CDebugGeometryGL();

        void                draw()    override;
        void                free()    override;

    private:

        void                initDraw(SDebugObject& object)    override;
        void                refreshDraw(SDebugObject& object) override;

        void                drawNormals();
        void                drawEdges();
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_DEBUG_GEOMETRY_GL_H_
