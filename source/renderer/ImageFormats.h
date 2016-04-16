#ifndef _V3D_IMAGE_FORMATS_H_
#define _V3D_IMAGE_FORMATS_H_

#include "common.h"

namespace v3d
{
namespace renderer
{

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EImageType
    {
        eByte,
        eUnsignedByte,
        eShort,
        eUnsignedShort,
        eUnsignedShort_565,
        eUnsignedShort_4444,
        eInt,
        eUnsignedInt,
        eFloat,

        eImageTypeCount
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EImageFormat
    {
        eRed,
        eRG,
        eRGB,
        eRGBA,
        eBGR,
        eBGRA,
        eDepthComponent,

        eFormatCount
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_V3D_IMAGE_FORMATS_H_
