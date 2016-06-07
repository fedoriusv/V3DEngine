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
        ePrimitivesNone = -1,

        eTriangles,
        eTriangleStrip,
        eTriangleFan,
        eLines,
        eLinesStrip,
        eLinesLoop,
        ePoints,
        ePatches,

        ePrimitivesModeCount
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EBufferTarget
    {
        eVertexBuffer,
        eIndexBuffer,
        eTransformFeedbackBuffer,
        eTextureStoreBuffer,

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

    class GeometryType
    {
    public:

        static const std::string&   getStringByPrimitivesMode(EPrimitivesMode type);
        static EPrimitivesMode      getPrimitivesModeByString(const std::string& name);

    private:

        static const std::string    s_typeName[EPrimitivesMode::ePrimitivesModeCount];
    };

} //namespace renderer
} //namespace v3d

#endif //_V3D_GEOMETRY_TYPES_H_
