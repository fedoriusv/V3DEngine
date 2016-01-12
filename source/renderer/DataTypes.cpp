#include "DataTypes.h"

namespace v3d
{
namespace renderer
{

const std::string DataType::s_typeName[EDataType::eDataTypeCount] = 
{
    "int",
    "float",
    "double",
    "vec2",
    "vec3",
    "vec4",
    "mat3",
    "mat4",
};


const std::string& DataType::getStringByDataType(EDataType type)
{
    return s_typeName[type];
}

EDataType DataType::getDataTypeByString(const std::string& name)
{
    for (u32 i = 0; i < EDataType::eDataTypeCount; ++i)
    {
        if (s_typeName[i].compare(name) == 0)
        {
            return (EDataType)i;
        }
    }

    return EDataType::eTypeNone;
}

} //namespace renderer
} //namespace v3d
