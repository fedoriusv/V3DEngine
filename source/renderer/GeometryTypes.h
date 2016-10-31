#ifndef _V3D_GEOMETRY_TYPES_H_
#define _V3D_GEOMETRY_TYPES_H_

#include "common.h"

namespace v3d
{
namespace renderer
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////

    //enum EPrimitivesTopology
    //{
    //    eTriangles,
    //    eTriangleStrip,
    //    eTriangleFan,
    //    eLines,
    //    eLinesStrip,
    //    eLinesLoop,
    //    ePoints,
    //    ePatches,

    //    ePrimitivesTopologyCount
    //};

    ////////////////////////////////////////////////////////////////////////////////////////////////////////

    //enum EBufferTarget
    //{
    //    eVertexBuffer,
    //    eIndexBuffer,
    //    eTransformFeedbackBuffer,
    //    eTextureStoreBuffer,

    //    eBufferTargetCount
    //};

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EDataUsageType
    {
        eWriteStatic,
        eWriteDynamic,
        eReadStatic,
        eReadDynamic,
        eCopyStatic,
        eCopyDynamic,

        eDataUsageTypeCount
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    //class GeometryType
    //{
    //public:

    //    static const std::string&   getStringByPrimitivesTopology(EPrimitivesTopology type);
    //    static EPrimitivesTopology  getPrimitivesTopologyByString(const std::string& name);

    //private:

    //    static const std::string    s_typeName[EPrimitivesTopology::ePrimitivesTopologyCount];
    //};

} //namespace renderer
} //namespace v3d

#endif //_V3D_GEOMETRY_TYPES_H_
