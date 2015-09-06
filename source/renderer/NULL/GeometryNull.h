#ifndef _V3D_GEOMETRY_NULL_H_
#define _V3D_GEOMETRY_NULL_H_

#include "renderer/Geometry.h"

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CGeometryNull : public CGeometry
    {
    public:

        CGeometryNull(const CRenderTechnique* technique);
        ~CGeometryNull();
        
        void                init()      override;
        void                free()      override;
        void                draw()      override;
        void                refresh()   override;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_GEOMETRY_NULL_H_
