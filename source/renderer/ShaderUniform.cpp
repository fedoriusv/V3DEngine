#include "ShaderUniform.h"
#include "ShaderData.h"
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

const std::string ShaderUniform::s_uniformName[EUniformData::eUniformsCount] = {

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


const std::string& ShaderUniform::getUniformNameByValue(EUniformData type)
{
    return s_uniformName[type];
}

ShaderUniform::EUniformData ShaderUniform::getValueByUniformName(const std::string& name)
{
    for (int i = 0; i < EUniformData::eUniformsCount; ++i)
    {
        if (s_uniformName[i].compare(name) == 0)
        {
            return (EUniformData)i;
        }
    }

    return EUniformData::eUserUniform;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////


ShaderUniform::ShaderUniform(ConstantBuffer* buffer)
    : m_name("")
    , m_type(EDataType::eTypeNone)
    , m_data(EUniformData::eUserUniform)

    , m_set(-1)
    , m_binding(-1)

    , 
{
}

ShaderUniform::ShaderUniform(const ShaderUniform& uniform)
    : m_type(EDataType::eTypeNone)
    , m_value(nullptr)
    , m_name("")
    , m_data(EUniformData::eUserUniform)

    , m_id(-1)
{
    if (uniform.m_data != EUniformData::eUserUniform)
    {
        ShaderUniform::setUniform(uniform.m_name, uniform.m_data);
    }
    else
    {
        ShaderUniform::setUniform(uniform.m_type, uniform.m_name, uniform.m_value);
    }
}

ShaderUniform& ShaderUniform::operator=(const ShaderUniform& uniform)
{
    if (&uniform == this)
    {
        return *this;
    }

    if (uniform.m_data != EUniformData::eUserUniform)
    {
        ShaderUniform::setUniform(uniform.m_name, uniform.m_data);
    }
    else
    {
        ShaderUniform::setUniform(uniform.m_type, uniform.m_name, uniform.m_value);
    }

    m_id = -1;

    return *this;
}

ShaderUniform::~ShaderUniform()
{
}

void ShaderUniform::setUniform(EDataType type, const std::string& name)
{
    m_type  = type;
    m_name = name;
}

void ShaderUniform::setUniform(const std::string& name, EUniformData data)
{
    m_name = name;
    m_data = data;

    switch (m_data)
    {
    case ShaderUniform::eTransformProjectionMatrix:
    case ShaderUniform::eTransformModelMatrix:
    case ShaderUniform::eTransformViewMatrix:
    case ShaderUniform::eTransformNormalMatrix:
        m_type = eTypeMatrix4;
        break;

    case ShaderUniform::eTransformViewPosition:
    case ShaderUniform::eTransformViewUpVector:
    case ShaderUniform::eLightPosition:
    case ShaderUniform::eLightDirection:
    case ShaderUniform::eLightAttenuation:

        m_type = eTypeVector3;
        break;

    case ShaderUniform::eMaterialAmbient:
    case ShaderUniform::eMaterialDiffuse:
    case ShaderUniform::eMaterialSpecular:
    case ShaderUniform::eMaterialEmission:
    case ShaderUniform::eLightAmbient:
    case ShaderUniform::eLightDiffuse:
    case ShaderUniform::eLightSpecular:
        m_type = eTypeVector4;
        break;

    case ShaderUniform::eMaterialShininess:
    case ShaderUniform::eMaterialTransparency:
    case ShaderUniform::eLightRadius:
        m_type = eTypeFloat;
        break;

    case ShaderUniform::eLightsCount:
    case ShaderUniform::eCurrentTime:
        m_type = eTypeInt;
        break;

    default:
        m_type = eTypeNone;
        break;
    }
}

EDataType ShaderUniform::getDataType() const
{
    return m_type;
}

ShaderUniform::EUniformData ShaderUniform::getData() const
{
    return m_data;
}

const std::string& ShaderUniform::getName() const
{
    return m_name;
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

    if (!root->Attribute("val"))
    {
        LOG_ERROR("ShaderUniform: Cannot find uniform val");
        return false;
    }
    const std::string varVal = root->Attribute("val");

    EDataType uniformType = EDataType::eTypeNone;
    EUniformData uniformName = ShaderUniform::getValueByUniformName(varVal);
    bool defaultUniform = (uniformName != EUniformData::eUserUniform);
    if (!defaultUniform)
    {
        if (!root->Attribute("type"))
        {
            LOG_ERROR("CRenderPass: Cannot find uniform type in '%s'", varName.c_str());
            return false;
        }
        const std::string varType = root->Attribute("type");

        uniformType = DataType::getDataTypeByString(varType);
        if (uniformType == EDataType::eTypeNone)
        {
            LOG_ERROR("CRenderPass: Cannot find uniform type in '%s'", varName.c_str());
            return false;
        }
        ShaderUniform::parseUserUniform(root, varName, uniformType);

    }
    else
    {
        ShaderUniform::setUniform(varName, uniformName);
    }

    return true;
}

bool ShaderUniform::parseUserUniform(const tinyxml2::XMLElement* root, const std::string& name, EDataType type)
{
    switch (type)
    {
        case EDataType::eTypeInt:
        {
            const s32 value = root->IntAttribute("val");
            ShaderUniform::setUniform(type, name, (void*)&value);

            return true;
        }

        case EDataType::eTypeFloat:
        {
            const f32 value = root->FloatAttribute("val");
            ShaderUniform::setUniform(type, name, (void*)&value);

            return true;
        }

        case EDataType::eTypeVector2:
        {
            Vector2D value(0.f, 0.f);
            if (root->Attribute("val"))
            {
                std::string str = root->Attribute("val");

                f32* val = new f32[2];
                ShaderUniform::parseArrayValue(str, val, 2);
                value.x = val[0];
                value.y = val[1];

                delete[] val;
                val = nullptr;
            }
            ShaderUniform::setUniform(type, name, (void*)&value);

            return true;
        }

        case EDataType::eTypeVector3:
        {
            Vector3D value(0.f, 0.f, 0.f);
            if (root->Attribute("val"))
            {
                std::string str = root->Attribute("val");

                f32* val = new f32[3];
                ShaderUniform::parseArrayValue(str, val, 3);
                value.x = val[0];
                value.y = val[1];
                value.z = val[2];

                delete[] val;
                val = nullptr;
            }
            ShaderUniform::setUniform(type, name, (void*)&value);

            return true;
        }

        case EDataType::eTypeVector4:
        {
            Vector4D value(0.f, 0.f, 0.f, 0.f);
            if (root->Attribute("val"))
            {
                std::string str = root->Attribute("val");

                f32* val = new f32[4];
                ShaderUniform::parseArrayValue(str, val, 4);
                value.x = val[0];
                value.y = val[1];
                value.z = val[2];
                value.w = val[3];

                delete[] val;
                val = nullptr;
            }
            ShaderUniform::setUniform(type, name, (void*)&value);

            return true;
        }

        case EDataType::eTypeMatrix3:
        {
            Matrix3D value;
            if (root->Attribute("val"))
            {
                std::string str = root->Attribute("val");

                f32* val = new f32[9];
                ShaderUniform::parseArrayValue(str, val, 9);
                f32* matrix = value.getPtr();
                memcpy(matrix, val, sizeof(f32)* 9);

                delete[] val;
                val = nullptr;
            }
            ShaderUniform::setUniform(type, name, (void*)&value);

            return true;
        }

        case EDataType::eTypeMatrix4:
        {
            Matrix4D value;
            if (root->Attribute("val"))
            {
                std::string str = root->Attribute("val");

                f32* val = new f32[16];
                ShaderUniform::parseArrayValue(str, val, 16);
                f32* matrix = value.getPtr();
                memcpy(matrix, val, sizeof(f32)* 16);

                delete[] val;
                val = nullptr;
            }
            ShaderUniform::setUniform(type, name, (void*)&value);

            return true;
        }
    }

    return false;
}

void ShaderUniform::parseArrayValue(const std::string& valueStr, f32* array, u32 count)
{
    size_t pos = 0;
    std::string str = valueStr;

    for (u32 i = 0; i < count - 1; ++i)
    {
        pos = str.find(",");
        std::string valStr = str.substr(0, pos);
        str = str.substr(pos + 1, str.size());

        array[i] = ::std::stof(valStr);
    }

    array[count - 1] = ::std::stof(str);
}

} //namespace renderer
} //namespace v3d
