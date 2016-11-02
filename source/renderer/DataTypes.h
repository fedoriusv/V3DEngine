#pragma once

#include "common.h"

namespace v3d
{
namespace renderer
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EDataType
    {
        eTypeNone = -1,
        eTypeInt,
        eTypeFloat,
        ETypeDouble,
        eTypeVector2,
        eTypeVector3,
        eTypeVector4,
        eTypeMatrix3,
        eTypeMatrix4,

        eDataTypeCount
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    class DataType
    {
    public:

        static const std::string&   getStringByDataType(EDataType type);
        static EDataType            getDataTypeByString(const std::string& name);

    private:

        static const std::string    s_typeName[EDataType::eDataTypeCount];
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d
