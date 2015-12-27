#include "DataTypes.h"

using namespace v3d;
using namespace renderer;

const std::string CDataType::s_typeName[eDataTypeCount] = {

    "int",
    "float",
    "double",
    "vec2",
    "vec3",
    "vec4",
    "mat3",
    "mat4",
};


const std::string& CDataType::getStringByDataType(EDataType type)
{
    return s_typeName[type];
}

EDataType CDataType::getDataTypeByString(const std::string& name)
{
    for (int i = 0; i < eDataTypeCount; ++i)
    {
        if (s_typeName[i].compare(name) == 0)
        {
            return (EDataType)i;
        }
    }

    return eTypeNone;
}
