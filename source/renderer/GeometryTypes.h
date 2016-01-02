#ifndef _V3D_GEOMETRY_TYPES_H_
#define _V3D_GEOMETRY_TYPES_H_

#include "common.h"

namespace v3d
{
namespace renderer
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EDrawMode
    {
        eTriangles,
        eTriangleStrip,
        eLines,
        eLinesStrip,
        ePoints,

        eDrawModeCount
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EGeometryTarget
    {
        eVertexBuffer,
        eIndexBuffer,
        eTransformFeedbackBuffer,

        eArrayTargetCount
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EGeomertyType
    {
        eGeomertyStatic,
        eGeomertyDynamic,

        eGeometryTypeCount
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_V3D_GEOMETRY_TYPES_H_
