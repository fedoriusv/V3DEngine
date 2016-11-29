#include "ShaderUniform.h"
#include "utils/Logger.h"

#include "tinyxml2.h"

#ifndef _PLATFORM_WIN_
#   pragma clang diagnostic ignored "-Wswitch"
#endif  //_PLATFORM_WIN_

namespace v3d
{
namespace renderer
{

using namespace core;

//////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string ShaderUniform::s_uniformName[ETypeContent::eUniformsCount] = {

    "transform.projectionMatrix",
    "transform.modelMatrix",
    "transform.viewMatrix",
    "transform.normalMatrix",
    "transform.viewPosition",
    "transform.viewUpVector",

    "material.ambient",
    "material.diffuse",
    "material.specular",
    "material.emission",
    "material.shininess",
    "material.transparency",

    "lights.count",
    "light.position",
    "light.ambient",
    "light.diffuse",
    "light.specular",
    "light.direction",
    "light.attenuation",
    "light.radius",

    "currentTime",
    "viewportSize"
};


const std::string& ShaderUniform::getUniformNameByValue(ETypeContent type)
{
    return s_uniformName[type];
}

ShaderUniform::ETypeContent ShaderUniform::getValueByUniformName(const std::string& name)
{
    for (int i = 0; i < ETypeContent::eUniformsCount; ++i)
    {
        if (s_uniformName[i] == name)
        {
            return (ETypeContent)i;
        }
    }

    return ETypeContent::eUserUniform;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////


ShaderUniform::ShaderUniform()
    : m_name("")
    , m_initValue(nullptr)
    , m_array(1)

    , m_set(0)
    , m_binding(0)

    , m_buffer(0)
    , m_size(0)
    , m_offset(0)

    , m_dataType(ShaderDataType::EDataType::eUnknown)
    , m_uniformContent(ShaderUniform::eUserUniform)
{
}

ShaderUniform::~ShaderUniform()
{
    if (m_initValue)
    {
        free(m_initValue);
        m_initValue = nullptr;
    }
}

const std::string& ShaderUniform::getName() const
{
    return m_name;
}

ShaderDataType::EDataType ShaderUniform::getDataType() const
{
    return m_dataType;
}

ShaderUniform::ETypeContent ShaderUniform::getType() const
{
    return m_uniformContent;
}

bool ShaderUniform::parse(const tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("ShaderUniform: Not exist xml uniforms element");
        return false;
    }

    if (!root->Attribute("name"))
    {
        LOG_ERROR("ShaderUniform: Cannot find uniform name");
        return false;
    }
    const std::string varName = root->Attribute("name");

    std::string varVal;
    ETypeContent uniformType = ETypeContent::eUserUniform;
    if (root->Attribute("val"))
    {
        varVal = root->Attribute("val");
        uniformType = ShaderUniform::getValueByUniformName(varVal);
    }

    bool defaultUniform = (uniformType != ETypeContent::eUserUniform);
    if (!defaultUniform)
    {
        if (!root->Attribute("type"))
        {
            LOG_ERROR("ShaderUniform: Cannot find uniform type in '%s'", varName.c_str());
            return false;
        }
        const std::string varType = root->Attribute("type");

        ShaderDataType::EDataType dataType = ShaderDataType::EDataType::eUnknown;
        dataType = ShaderDataType::getDataTypeByString(varType);
        if (dataType == ShaderDataType::EDataType::eUnknown)
        {
            LOG_ERROR("ShaderUniform: Cannot find uniform type in '%s'", varName.c_str());
            return false;
        }
        ShaderUniform::parseUserUniform(root, varName, dataType);
    }
    else
    {
        ShaderUniform::setUniform(varName, uniformType);
    }

    return true;
}

bool ShaderUniform::parseUserUniform(const tinyxml2::XMLElement* root, const std::string& name, ShaderDataType::EDataType type)
{
    static const std::string k_separator = ",";
    switch (type)
    {
        case ShaderDataType::EDataType::eInt:
        {
            const s32 value = root->IntAttribute("val");
            u32 array = 1;
            ShaderUniform::setUniform(name, type, array, &value);

            return true;
        }

        case ShaderDataType::EDataType::eInt64:
        {
            const s64 value = root->Int64Attribute("val");
            u32 array = 1;
            ShaderUniform::setUniform(name, type, array, &value);

            return true;
        }

        case ShaderDataType::EDataType::eUint:
        {
            const u32 value = root->UnsignedAttribute("val");
            u32 array = 1;
            ShaderUniform::setUniform(name, type, array, &value);

            return true;
        }

        case ShaderDataType::EDataType::eUint64:
        {
            const u64 value = static_cast<u64>(root->Int64Attribute("val"));
            u32 array = 1;
            ShaderUniform::setUniform(name, type, array, &value);

            return true;
        }

        case ShaderDataType::EDataType::eFloat:
        {
            const f32 value = root->FloatAttribute("val");
            u32 array = 1;
            ShaderUniform::setUniform(name, type, array, &value);

            return true;
        }

        case ShaderDataType::EDataType::eDouble:
        {
            const f64 value = root->DoubleAttribute("val");
            u32 array = 1;
            ShaderUniform::setUniform(name, type, array, &value);

            return true;
        }

        case ShaderDataType::EDataType::eVector2i:
        case ShaderDataType::EDataType::eVector3i:
        case ShaderDataType::EDataType::eVector4i:
        {
            u32 count = ShaderDataType::componentsCount(type);
            s32* elements = new s32[count];
            auto parseElements = [elements](const std::string& valueStr, u32 count) -> void
            {
                size_t pos = 0;
                std::string str = valueStr;

                for (u32 i = 0; i < count - 1; ++i)
                {
                    pos = str.find(",");
                    std::string valStr = str.substr(0, pos);
                    str = str.substr(pos + 1, str.size());

                    elements[i] = std::stoi(valStr);
                }
                elements[count - 1] = std::stoi(str);
            };

            std::string str = root->Attribute("val");
            u32 array = 1;
            parseElements(str, count);

            ShaderUniform::setUniform(name, type, array, elements);

            delete[] elements;
            return true;
        }

        case ShaderDataType::EDataType::eVector2i64:
        case ShaderDataType::EDataType::eVector3i64:
        case ShaderDataType::EDataType::eVector4i64:
        {
            u32 count = ShaderDataType::componentsCount(type);
            s64* elements = new s64[count];
            auto parseElements = [elements](const std::string& valueStr, u32 count) -> void
            {
                size_t pos = 0;
                std::string str = valueStr;

                for (u32 i = 0; i < count - 1; ++i)
                {
                    pos = str.find(",");
                    std::string valStr = str.substr(0, pos);
                    str = str.substr(pos + 1, str.size());

                    elements[i] = std::stoll(valStr);
                }
                elements[count - 1] = std::stoll(str);
            };

            std::string str = root->Attribute("val");
            u32 array = 1;
            parseElements(str, count);

            ShaderUniform::setUniform(name, type, array, elements);

            delete[] elements;
            return true;
        }

        case ShaderDataType::EDataType::eVector2u:
        case ShaderDataType::EDataType::eVector3u:
        case ShaderDataType::EDataType::eVector4u:
        {
            u32 count = ShaderDataType::componentsCount(type);
            u32* elements = new u32[count];
            auto parseElements = [elements](const std::string& valueStr, u32 count) -> void
            {
                size_t pos = 0;
                std::string str = valueStr;

                for (u32 i = 0; i < count - 1; ++i)
                {
                    pos = str.find(",");
                    std::string valStr = str.substr(0, pos);
                    str = str.substr(pos + 1, str.size());

                    elements[i] = static_cast<u32>(std::stoul(valStr));
                }
                elements[count - 1] = static_cast<u32>(std::stoul(str));
            };

            std::string str = root->Attribute("val");
            u32 array = 1;
            parseElements(str, count);

            ShaderUniform::setUniform(name, type, array, elements);

            delete[] elements;
            return true;
        }

        case ShaderDataType::EDataType::eVector2u64:
        case ShaderDataType::EDataType::eVector3u64:
        case ShaderDataType::EDataType::eVector4u64:
        {
            u32 count = ShaderDataType::componentsCount(type);
            u64* elements = new u64[count];
            auto parseElements = [elements](const std::string& valueStr, u32 count) -> void
            {
                size_t pos = 0;
                std::string str = valueStr;

                for (u32 i = 0; i < count - 1; ++i)
                {
                    pos = str.find(",");
                    std::string valStr = str.substr(0, pos);
                    str = str.substr(pos + 1, str.size());

                    elements[i] = std::stoull(valStr);
                }
                elements[count - 1] = std::stoull(str);
            };

            std::string str = root->Attribute("val");
            u32 array = 1;
            parseElements(str, count);

            ShaderUniform::setUniform(name, type, array, elements);

            delete[] elements;
            return true;
        }

        case ShaderDataType::EDataType::eVector2f:
        case ShaderDataType::EDataType::eVector3f:
        case ShaderDataType::EDataType::eVector4f:
        case ShaderDataType::EDataType::eMatrix3f:
        case ShaderDataType::EDataType::eMatrix4f:
        {
            u32 count = ShaderDataType::componentsCount(type);
            f32* elements = new f32[count];
            auto parseElements = [elements](const std::string& valueStr, u32 count) -> void
            {
                size_t pos = 0;
                std::string str = valueStr;

                for (u32 i = 0; i < count - 1; ++i)
                {
                    pos = str.find(",");
                    std::string valStr = str.substr(0, pos);
                    str = str.substr(pos + 1, str.size());

                    elements[i] = std::stof(valStr);
                }
                elements[count - 1] = std::stof(str);
            };

            std::string str = root->Attribute("val");
            u32 array = 1;
            parseElements(str, count);

            ShaderUniform::setUniform(name, type, array, elements);

            delete[] elements;
            return true;
        }

        case ShaderDataType::EDataType::eVector2d:
        case ShaderDataType::EDataType::eVector3d:
        case ShaderDataType::EDataType::eVector4d:
        case ShaderDataType::EDataType::eMatrix3d:
        case ShaderDataType::EDataType::eMatrix4d:
        {
            u32 count = ShaderDataType::componentsCount(type);
            f64* elements = new f64[count];
            auto parseElements = [elements](const std::string& valueStr, u32 count) -> void
            {
                size_t pos = 0;
                std::string str = valueStr;

                for (u32 i = 0; i < count - 1; ++i)
                {
                    pos = str.find(",");
                    std::string valStr = str.substr(0, pos);
                    str = str.substr(pos + 1, str.size());

                    elements[i] = std::stod(valStr);
                }
                elements[count - 1] = std::stod(str);
            };

            std::string str = root->Attribute("val");
            u32 array = 1;
            parseElements(str, count);

            ShaderUniform::setUniform(name, type, array, elements);

            delete[] elements;
            return true;
        }
    }

    return false;
}

void ShaderUniform::setUniform(const std::string& name, ETypeContent data)
{
    ASSERT(data != ETypeContent::eUserUniform, "can't be user unifrom");
    m_name = name;
    m_uniformContent = data;

    switch (data)
    {
    case ShaderUniform::eTransformProjectionMatrix:
    case ShaderUniform::eTransformModelMatrix:
    case ShaderUniform::eTransformViewMatrix:
    case ShaderUniform::eTransformNormalMatrix:
        m_dataType = ShaderDataType::EDataType::eMatrix4f;
        break;

    case ShaderUniform::eTransformViewPosition:
    case ShaderUniform::eTransformViewUpVector:
    case ShaderUniform::eLightPosition:
    case ShaderUniform::eLightDirection:
    case ShaderUniform::eLightAttenuation:

        m_dataType = ShaderDataType::EDataType::eVector3f;
        break;

    case ShaderUniform::eMaterialAmbient:
    case ShaderUniform::eMaterialDiffuse:
    case ShaderUniform::eMaterialSpecular:
    case ShaderUniform::eMaterialEmission:
    case ShaderUniform::eLightAmbient:
    case ShaderUniform::eLightDiffuse:
    case ShaderUniform::eLightSpecular:
        m_dataType = ShaderDataType::EDataType::eVector4f;
        break;

    case ShaderUniform::eMaterialShininess:
    case ShaderUniform::eMaterialTransparency:
    case ShaderUniform::eLightRadius:
        m_dataType = ShaderDataType::EDataType::eFloat;
        break;

    case ShaderUniform::eLightsCount:
    case ShaderUniform::eCurrentTime:
        m_dataType = ShaderDataType::EDataType::eInt;
        break;

    default:
        m_dataType = ShaderDataType::EDataType::eUnknown;
        break;
    }
}

void ShaderUniform::setUniform(const std::string& name, ShaderDataType::EDataType type, u32 array, const void* data)
{
    m_name = name;
    m_dataType = type;
    m_uniformContent = ETypeContent::eUserUniform;

    ASSERT(array > 0, "invalid array");
    u32 size = ShaderDataType::typeSize(type)* array;
    if (!m_initValue)
    {
        m_initValue = malloc(size);
    }
    memcpy(m_initValue, data, size);
}

} //namespace renderer
} //namespace v3d
