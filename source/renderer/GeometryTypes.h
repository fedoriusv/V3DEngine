#ifndef _V3D_GEOMETRY_TYPES_H_
#define _V3D_GEOMETRY_TYPES_H_

#include "common.h"

namespace v3d
{
namespace renderer
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EPrimitivesMode
    {
        eTriangles,
        eTriangleStrip,
        eLines,
        eLinesStrip,
        ePoints,

        ePrimitivesModeCount
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EBufferTarget
    {
        eVertexBuffer,
        eIndexBuffer,
        eTransformFeedbackBuffer,

        eBufferTargetCount
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EDataUsageType
    {
        eDrawStatic,
        eDrawDynamic,
        eReadStatic,
        eReadDynamic,
        eCopyStatic,
        eCopyDynamic,

        eDataUsageTypeCount
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_V3D_GEOMETRY_TYPES_H_
