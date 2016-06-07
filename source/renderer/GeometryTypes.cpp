#include "GeometryTypes.h"

namespace v3d
{
namespace renderer
{

const std::string GeometryType::s_typeName[EPrimitivesMode::ePrimitivesModeCount] = 
{
    "triangles",
    "trianglestrip",
    "trianglefan",
    "lines",
    "linesstrip",
    "linesloop",
    "points",
    "patches"
};

const std::string& GeometryType::getStringByPrimitivesMode(EPrimitivesMode type)
{
    return s_typeName[type];
}

EPrimitivesMode GeometryType::getPrimitivesModeByString(const std::string& name)
{
    for (u32 i = 0; i < EPrimitivesMode::ePrimitivesModeCount; ++i)
    {
        if (s_typeName[i] == name)
        {
            return (EPrimitivesMode)i;
        }
    }

    return EPrimitivesMode::ePrimitivesNone;
}

} //namespace renderer
} //namespace v3d
