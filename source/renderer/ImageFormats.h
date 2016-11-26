#ifndef _V3D_IMAGE_FORMATS_H_
#define _V3D_IMAGE_FORMATS_H_

#include "common.h"

namespace v3d
{
namespace renderer
{

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    enum class EImageType : u32
    {
        eByte,
        eUnsignedByte,
        eShort,
        eUnsignedShort,
        eInt,
        eUnsignedInt,
        eUnsignedInt24_8,
        eHalfFloat,
        eFloat,

        eImageTypeCount
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    enum class EImageFormat : u32
    {
        eRed,
        eRG,
        eRGB,
        eRGBA,
        eDepthComponent,
        eStencilIndex,

        eFormatCount
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class ImageFormat
    {
    public:

        static u32 typeSize(EImageType type)
        {
            switch (type)
            {
            case EImageType::eByte:
                return sizeof(s8);

            case EImageType::eUnsignedByte:
                return sizeof(u8);

            case EImageType::eShort:
                return sizeof(s16);

            case EImageType::eUnsignedShort:
                return sizeof(u16);

            case EImageType::eInt:
                return sizeof(s32);

            case EImageType::eUnsignedInt:
            case EImageType::eUnsignedInt24_8:
                return sizeof(u32);

            case EImageType::eHalfFloat:
                return sizeof(f32) / 2;

            case EImageType::eFloat:
                return sizeof(f32);

            default:
                ASSERT(false, "Unkouwn type size");
                return 0;
            };
        }

        static u32 componentCount(EImageFormat format)
        {
            switch (format)
            {
            case EImageFormat::eRed:
            case EImageFormat::eStencilIndex:
            case EImageFormat::eDepthComponent:
                return 1;

            case EImageFormat::eRG:
                return 2;

            case EImageFormat::eRGB:
                return 3;

            case EImageFormat::eRGBA:
                return 4;

            default:
                ASSERT(false, "Unkouwn format component count");
                return 0;
            };
        }
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_V3D_IMAGE_FORMATS_H_
