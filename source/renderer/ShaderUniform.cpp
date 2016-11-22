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


ShaderUniform::ShaderUniform()
    : m_name("")
    , m_type(ShaderDataType::eUnknown)
    , m_data(EUniformData::eUserUniform)
{
}

ShaderUniform::ShaderUniform(const ShaderUniform& uniform)
    : m_type(ShaderDataType::eUnknown)
    , m_name("")
    , m_data(EUniformData::eUserUniform)
{
    if (uniform.m_data != EUniformData::eUserUniform)
    {
        ShaderUniform::setUniform(uniform.m_name, uniform.m_data);
    }
    else
    {
        //ShaderUniform::setUniform(uniform.m_type, uniform.m_name, uniform.m_value);
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
        //ShaderUniform::setUniform(uniform.m_type, uniform.m_name, uniform.m_value);
    }

    //m_id = -1;

    return *this;
}

ShaderUniform::~ShaderUniform()
{
}

//void ShaderUniform::setUniform(EShaderDataType type, const std::string& name)
//{
//    m_type  = type;
//    m_name = name;
//}

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
        m_type = ShaderDataType::eMatrix4f;
        break;

    case ShaderUniform::eTransformViewPosition:
    case ShaderUniform::eTransformViewUpVector:
    case ShaderUniform::eLightPosition:
    case ShaderUniform::eLightDirection:
    case ShaderUniform::eLightAttenuation:

        m_type = ShaderDataType::eVector3f;
        break;

    case ShaderUniform::eMaterialAmbient:
    case ShaderUniform::eMaterialDiffuse:
    case ShaderUniform::eMaterialSpecular:
    case ShaderUniform::eMaterialEmission:
    case ShaderUniform::eLightAmbient:
    case ShaderUniform::eLightDiffuse:
    case ShaderUniform::eLightSpecular:
        m_type = ShaderDataType::eVector4f;
        break;

    case ShaderUniform::eMaterialShininess:
    case ShaderUniform::eMaterialTransparency:
    case ShaderUniform::eLightRadius:
        m_type = ShaderDataType::eFloat;
        break;

    case ShaderUniform::eLightsCount:
    case ShaderUniform::eCurrentTime:
        m_type = ShaderDataType::eInt;
        break;

    default:
        m_type = ShaderDataType::eUnknown;
        break;
    }
}

ShaderDataType::EShaderDataType ShaderUniform::getDataType() const
{
    return m_type;
}

ShaderUniform::EUniformData ShaderUniform::getKindData() const
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

    ShaderDataType::EShaderDataType uniformType = ShaderDataType::eUnknown;
    EUniformData uniformName = ShaderUniform::getValueByUniformName(varVal);
    bool defaultUniform = (uniformName != EUniformData::eUserUniform);
    if (!defaultUniform)
    {
        if (!root->Attribute("type"))
        {
            LOG_ERROR("RenderPass: Cannot find uniform type in '%s'", varName.c_str());
            return false;
        }
        const std::string varType = root->Attribute("type");

        uniformType = ShaderDataType::getDataTypeByString(varType);
        if (uniformType == ShaderDataType::eUnknown)
        {
            LOG_ERROR("RenderPass: Cannot find uniform type in '%s'", varName.c_str());
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

bool ShaderUniform::parseUserUniform(const tinyxml2::XMLElement* root, const std::string& name, ShaderDataType::EShaderDataType type)
{
    switch (type)
    {
        case ShaderDataType::eInt:
        {
            const s32 value = root->IntAttribute("val");
            //ShaderUniform::setUniform(type, name, (void*)&value);

            return true;
        }

        case ShaderDataType::eFloat:
        {
            const f32 value = root->FloatAttribute("val");
            //ShaderUniform::setUniform(type, name, (void*)&value);

            return true;
        }

        case ShaderDataType::eVector2f:
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
            //ShaderUniform::setUniform(type, name, (void*)&value);

            return true;
        }

        case ShaderDataType::eVector3f:
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
            //ShaderUniform::setUniform(type, name, (void*)&value);

            return true;
        }

        case ShaderDataType::eVector4f:
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
            //ShaderUniform::setUniform(type, name, (void*)&value);

            return true;
        }

        case ShaderDataType::eMatrix3f:
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
            //ShaderUniform::setUniform(type, name, (void*)&value);

            return true;
        }

        case ShaderDataType::eMatrix4f:
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
            //ShaderUniform::setUniform(type, name, (void*)&value);

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
