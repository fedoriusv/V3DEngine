#ifndef _V3D_DATA_TYPES_H_
#define _V3D_DATA_TYPES_H_

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
        eTypeVector2,
        eTypeVector3,
        eTypeVector4,
        eTypeMatrix3,
        eTypeMatrix4,

        eDataTypeCount
    };

    class CDataType
    {
    public:

        static const std::string&   getStringByType(EDataType type);
        static EDataType            getTypeByString(const std::string& name);

    private:

        static const std::string    s_typeName[eDataTypeCount];
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_DATA_TYPES_H_