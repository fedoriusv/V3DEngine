#include "ShaderDataTypes.h"

namespace v3d
{
namespace renderer
{

const std::string ShaderDataType::s_typeName[toEnumType(EDataType::eCount)] =
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


const std::string& ShaderDataType::getStringByDataType(EDataType type)
{
    return s_typeName[toEnumType(type)];
}

ShaderDataType::EDataType ShaderDataType::getDataTypeByString(const std::string& name)
{
    for (u32 i = 0; i < toEnumType(EDataType::eCount); ++i)
    {
        if (s_typeName[i] == name)
        {
            return static_cast<EDataType>(i);
        }
    }

    return EDataType::eUnknown;
}

u32 ShaderDataType::typeSize(EDataType type)
{
    switch (type)
    {
    case v3d::renderer::ShaderDataType::EDataType::eInt:
        return sizeof(s32);

    case v3d::renderer::ShaderDataType::EDataType::eUint:
        return sizeof(u32);

    case v3d::renderer::ShaderDataType::EDataType::eInt64:
        return sizeof(s64);

    case v3d::renderer::ShaderDataType::EDataType::eUint64:
        return sizeof(u64);

    case v3d::renderer::ShaderDataType::EDataType::eFloat:
        return sizeof(f32);

    case v3d::renderer::ShaderDataType::EDataType::eDouble:
        return sizeof(f64);

    case v3d::renderer::ShaderDataType::EDataType::eVector2i:
        return sizeof(core::Point2D);

    case v3d::renderer::ShaderDataType::EDataType::eVector2u:
        return sizeof(core::Point2DU);

    case v3d::renderer::ShaderDataType::EDataType::eVector2i64:
        return sizeof(core::Point2D64);

    case v3d::renderer::ShaderDataType::EDataType::eVector2u64:
        return sizeof(core::Point2DU64);

    case v3d::renderer::ShaderDataType::EDataType::eVector2f:
        return sizeof(core::Vector2D);

    case v3d::renderer::ShaderDataType::EDataType::eVector2d:
        return sizeof(core::Vector2D64);

    case v3d::renderer::ShaderDataType::EDataType::eVector3i:
        return sizeof(core::Point3D);

    case v3d::renderer::ShaderDataType::EDataType::eVector3u:
        return sizeof(core::Point3DU);

    case v3d::renderer::ShaderDataType::EDataType::eVector3i64:
        return sizeof(core::Point3D64);

    case v3d::renderer::ShaderDataType::EDataType::eVector3u64:
        return sizeof(core::Point3DU64);

    case v3d::renderer::ShaderDataType::EDataType::eVector3f:
        return sizeof(core::Vector3D);

    case v3d::renderer::ShaderDataType::EDataType::eVector3d:
        return sizeof(core::Vector3D64);

    case v3d::renderer::ShaderDataType::EDataType::eVector4i:
        return sizeof(core::Point4D);

    case v3d::renderer::ShaderDataType::EDataType::eVector4u:
        return sizeof(core::Point4DU);

    case v3d::renderer::ShaderDataType::EDataType::eVector4i64:
        return sizeof(core::Point4D64);

    case v3d::renderer::ShaderDataType::EDataType::eVector4u64:
        return sizeof(core::Point4DU64);

    case v3d::renderer::ShaderDataType::EDataType::eVector4f:
        return sizeof(core::Vector4D);

    case v3d::renderer::ShaderDataType::EDataType::eVector4d:
        return sizeof(core::Vector4D64);

    case v3d::renderer::ShaderDataType::EDataType::eMatrix3f:
        return sizeof(core::Matrix3D);

    case v3d::renderer::ShaderDataType::EDataType::eMatrix3d:
        return sizeof(core::Matrix3D64);

    case v3d::renderer::ShaderDataType::EDataType::eMatrix4f:
        return sizeof(core::Matrix4D);

    case v3d::renderer::ShaderDataType::EDataType::eMatrix4d:
        return sizeof(core::Matrix4D64);

    default:
        ASSERT(false, "unknown type");
        return 0;
    }

    return 0;
}

u32 ShaderDataType::componentsCount(EDataType type)
{
    switch (type)
    {
    case v3d::renderer::ShaderDataType::EDataType::eInt:
    case v3d::renderer::ShaderDataType::EDataType::eUint:
    case v3d::renderer::ShaderDataType::EDataType::eInt64:
    case v3d::renderer::ShaderDataType::EDataType::eUint64:
    case v3d::renderer::ShaderDataType::EDataType::eFloat:
    case v3d::renderer::ShaderDataType::EDataType::eDouble:
        return 1;

    case v3d::renderer::ShaderDataType::EDataType::eVector2i:
    case v3d::renderer::ShaderDataType::EDataType::eVector2u:
    case v3d::renderer::ShaderDataType::EDataType::eVector2i64:
    case v3d::renderer::ShaderDataType::EDataType::eVector2u64:
    case v3d::renderer::ShaderDataType::EDataType::eVector2f:
    case v3d::renderer::ShaderDataType::EDataType::eVector2d:
        return 2;

    case v3d::renderer::ShaderDataType::EDataType::eVector3i:
    case v3d::renderer::ShaderDataType::EDataType::eVector3u:
    case v3d::renderer::ShaderDataType::EDataType::eVector3i64:
    case v3d::renderer::ShaderDataType::EDataType::eVector3u64:
    case v3d::renderer::ShaderDataType::EDataType::eVector3f:
    case v3d::renderer::ShaderDataType::EDataType::eVector3d:
        return 3;

    case v3d::renderer::ShaderDataType::EDataType::eVector4i:
    case v3d::renderer::ShaderDataType::EDataType::eVector4u:
    case v3d::renderer::ShaderDataType::EDataType::eVector4i64:
    case v3d::renderer::ShaderDataType::EDataType::eVector4u64:
    case v3d::renderer::ShaderDataType::EDataType::eVector4f:
    case v3d::renderer::ShaderDataType::EDataType::eVector4d:
        return 4;

    case v3d::renderer::ShaderDataType::EDataType::eMatrix3f:
    case v3d::renderer::ShaderDataType::EDataType::eMatrix3d:
        return 9;

    case v3d::renderer::ShaderDataType::EDataType::eMatrix4f:
    case v3d::renderer::ShaderDataType::EDataType::eMatrix4d:
        return 16;

    default:
        ASSERT(false, "unknown type");
        return 0;
    }

    return 0;
}

} //namespace renderer
} //namespace v3d
