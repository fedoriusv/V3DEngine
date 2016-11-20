#pragma once

#include "common.h"

namespace v3d
{
namespace renderer
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    class ShaderDataType
    {
    public:

        enum EShaderDataType : s16
        {
            eUnknown = -1,

            eInt,
            eUint,
            eInt64,
            eUint64,
            eFloat,
            eDouble,

            eVector2i,
            eVector2u,
            eVector2i64,
            eVector2u64,
            eVector2f,
            eVector2d,

            eVector3i,
            eVector3u,
            eVector3i64,
            eVector3u64,
            eVector3f,
            eVector3d,

            eVector4i,
            eVector4u,
            eVector4i64,
            eVector4u64,
            eVector4f,
            eVector4d,

            eMatrix3f,
            eMatrix3d,

            eMatrix4f,
            eMatrix4d,

            eCount
        };

        static const std::string&   getStringByDataType(EShaderDataType type);
        static EShaderDataType      getDataTypeByString(const std::string& name);

    private:

        static const std::string    s_typeName[EShaderDataType::eCount];
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d
