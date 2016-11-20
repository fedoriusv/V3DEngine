#include "ShaderDataTypes.h"

namespace v3d
{
namespace renderer
{

const std::string ShaderDataType::s_typeName[EShaderDataType::eCount] =
{
    "int",
    "uint",
    "int64_t",
    "uint64_t",
    "float",
    "double",

    "ivec2",
    "uvec2",
    "i64vec2",
    "u64vec2",
    "vec2",
    "dvec2",

    "ivec3",
    "uvec3",
    "i64vec3",
    "u64vec3",
    "vec3",
    "dvec3",

    "ivec4",
    "uvec4",
    "i64vec4",
    "u64vec4",
    "vec4",
    "dvec4",

    "mat3",
    "dmat3",

    "mat4",
    "dmat4",
};


const std::string& ShaderDataType::getStringByDataType(EShaderDataType type)
{
    return s_typeName[type];
}

ShaderDataType::EShaderDataType ShaderDataType::getDataTypeByString(const std::string& name)
{
    for (u32 i = 0; i < EShaderDataType::eCount; ++i)
    {
        if (s_typeName[i] == name)
        {
            return static_cast<EShaderDataType>(i);
        }
    }

    return EShaderDataType::eUnknown;
}

} //namespace renderer
} //namespace v3d
