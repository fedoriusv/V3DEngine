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

const std::string CShaderUniform::s_uniformName[EUniformData::eUniformsCount] = {

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


const std::string& CShaderUniform::getNameByValue(EUniformData type)
{
    return s_uniformName[type];
}

CShaderUniform::EUniformData CShaderUniform::getValueByName(const std::string& name)
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


CShaderUniform::CShaderUniform()
    : m_type(EDataType::eTypeNone)
    , m_value(nullptr)
    , m_name("")
    , m_data(EUniformData::eUserUniform)

    , m_id(-1)
{
}

CShaderUniform::CShaderUniform(const CShaderUniform& uniform)
    : m_type(EDataType::eTypeNone)
    , m_value(nullptr)
    , m_name("")
    , m_data(EUniformData::eUserUniform)

    , m_id(-1)
{
    if (uniform.m_data != EUniformData::eUserUniform)
    {
        CShaderUniform::setUniform(uniform.m_name, uniform.m_data);
    }
    else
    {
        CShaderUniform::setUniform(uniform.m_type, uniform.m_name, uniform.m_value);
    }

    m_id = uniform.m_id;

}

CShaderUniform& CShaderUniform::operator=(const CShaderUniform& uniform)
{
    if (&uniform == this)
    {
        return *this;
    }

    deallocMemory();
    if (uniform.m_data != EUniformData::eUserUniform)
    {
        CShaderUniform::setUniform(uniform.m_name, uniform.m_data);
    }
    else
    {
        CShaderUniform::setUniform(uniform.m_type, uniform.m_name, uniform.m_value);
    }

    m_id = uniform.m_id;

    return *this;
}

CShaderUniform::~CShaderUniform()
{
    deallocMemory();
}

void CShaderUniform::setUniform(EDataType type, const std::string& name, void* value)
{
    m_type  = type;
    m_name = name;
    if (value)
    {
        allocMemory(type, value);
    }
}

void CShaderUniform::setUniform(const std::string& name, EUniformData data)
{
    m_name = name;
    m_data = data;

    switch (m_data)
    {
    case CShaderUniform::eTransformProjectionMatrix:
    case CShaderUniform::eTransformModelMatrix:
    case CShaderUniform::eTransformViewMatrix:
    case CShaderUniform::eTransformNormalMatrix:
        m_type = eTypeMatrix4;
        break;

    case CShaderUniform::eTransformViewPosition:
    case CShaderUniform::eTransformViewUpVector:
    case CShaderUniform::eLightPosition:
    case CShaderUniform::eLightDirection:
    case CShaderUniform::eLightAttenuation:

        m_type = eTypeVector3;
        break;

    case CShaderUniform::eMaterialAmbient:
    case CShaderUniform::eMaterialDiffuse:
    case CShaderUniform::eMaterialSpecular:
    case CShaderUniform::eMaterialEmission:
    case CShaderUniform::eLightAmbient:
    case CShaderUniform::eLightDiffuse:
    case CShaderUniform::eLightSpecular:
        m_type = eTypeVector4;
        break;

    case CShaderUniform::eMaterialShininess:
    case CShaderUniform::eMaterialTransparency:
    case CShaderUniform::eLightRadius:
        m_type = eTypeFloat;
        break;

    case CShaderUniform::eLightsCount:
    case CShaderUniform::eCurrentTime:
        m_type = eTypeInt;
        break;

    default:
        m_type = eTypeNone;
        break;
    }
}

void CShaderUniform::allocMemory(EDataType type, void* value)
{
    switch(type)
    {
        case EDataType::eTypeInt:
        {
            if (m_value == nullptr)
            {
                m_value = new s32();
            }
            memcpy(m_value, value, sizeof(s32));

            return;
        }

        case EDataType::eTypeFloat:
        {
            if (m_value == nullptr)
            {
                m_value = new f32();
            }
            memcpy(m_value, value, sizeof(f32));

            return;
        }

        case EDataType::ETypeDouble:
        {
            if (m_value == nullptr)
            {
                m_value = new f64();
            }
            memcpy(m_value, value, sizeof(f64));

            return;
        }

        case EDataType::eTypeVector2:
        {
            if (m_value == nullptr)
            {
                m_value = new core::Vector2D();
            }
            memcpy(m_value, value, sizeof(core::Vector2D));

            return;
        }

        case EDataType::eTypeVector3:
        {
            if (m_value == nullptr)
            {
                m_value = new core::Vector3D();
            }
            memcpy(m_value, value, sizeof(core::Vector3D));

            return;
        }

        case EDataType::eTypeVector4:
        {
            if (m_value == nullptr)
            {
                m_value = new core::Vector4D();
            }
            memcpy(m_value, value, sizeof(core::Vector4D));

            return;
        }

        case EDataType::eTypeMatrix3:
        {
            if (m_value == nullptr)
            {
                m_value = new core::Matrix3D();
            }
            memcpy(m_value, value, sizeof(core::Matrix3D));

            return;
        }

        case EDataType::eTypeMatrix4:
        {
            if (m_value == nullptr)
            {
                m_value = new core::Matrix4D();
            }
            memcpy(m_value, value, sizeof(core::Matrix4D));

            return;
        }
    }

    LOG_ERROR("CShaderUniform: Type Data not exist");
}

void CShaderUniform::deallocMemory()
{
    if (m_value != nullptr)
    {
        free(m_value);
        m_value = nullptr;
    }
}

EDataType CShaderUniform::getType() const
{
    return m_type;
}

void* CShaderUniform::getValue() const
{
    return m_value;
}

CShaderUniform::EUniformData CShaderUniform::getData() const
{
    return m_data;
}

s32 CShaderUniform::getID() const
{
    return m_id;
}

const std::string& CShaderUniform::getName() const
{
    return m_name;
}

void CShaderUniform::setID(s32 id)
{
    m_id = id;
}

bool CShaderUniform::parse(const tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("CShaderUniform: Not exist xml uniforms element");
        return false;
    }

    if (!root->Attribute("name"))
    {
        LOG_ERROR("CShaderUniform: Cannot find uniform name");
        return false;
    }
    const std::string varName = root->Attribute("name");

    if (!root->Attribute("val"))
    {
        LOG_ERROR("CRenderPass: Cannot find uniform val");
        return false;
    }
    const std::string varVal = root->Attribute("val");


    EDataType uniformType = EDataType::eTypeNone;
    EUniformData uniformName = CShaderUniform::getValueByName(varVal);
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
        CShaderUniform::parseUserUniform(root, varName, uniformType);

    }
    else
    {
        CShaderUniform::setUniform(varName, uniformName);
    }

    return true;
}

bool CShaderUniform::parseUserUniform(const tinyxml2::XMLElement* root, const std::string& name, EDataType type)
{
    switch (type)
    {
        case EDataType::eTypeInt:
        {
            const s32 value = root->IntAttribute("val");
            CShaderUniform::setUniform(type, name, (void*)&value);

            return true;
        }

        case EDataType::eTypeFloat:
        {
            const f32 value = root->FloatAttribute("val");
            CShaderUniform::setUniform(type, name, (void*)&value);

            return true;
        }

        case EDataType::eTypeVector2:
        {
            Vector2D value(0.f, 0.f);
            if (root->Attribute("val"))
            {
                std::string str = root->Attribute("val");

                f32* val = new f32[2];
                CShaderUniform::parseArrayValue(str, val, 2);
                value.x = val[0];
                value.y = val[1];

                delete[] val;
                val = nullptr;
            }
            CShaderUniform::setUniform(type, name, (void*)&value);

            return true;
        }

        case EDataType::eTypeVector3:
        {
            Vector3D value(0.f, 0.f, 0.f);
            if (root->Attribute("val"))
            {
                std::string str = root->Attribute("val");

                f32* val = new f32[3];
                CShaderUniform::parseArrayValue(str, val, 3);
                value.x = val[0];
                value.y = val[1];
                value.z = val[2];

                delete[] val;
                val = nullptr;
            }
            CShaderUniform::setUniform(type, name, (void*)&value);

            return true;
        }

        case EDataType::eTypeVector4:
        {
            Vector4D value(0.f, 0.f, 0.f, 0.f);
            if (root->Attribute("val"))
            {
                std::string str = root->Attribute("val");

                f32* val = new f32[4];
                CShaderUniform::parseArrayValue(str, val, 4);
                value.x = val[0];
                value.y = val[1];
                value.z = val[2];
                value.w = val[3];

                delete[] val;
                val = nullptr;
            }
            CShaderUniform::setUniform(type, name, (void*)&value);

            return true;
        }

        case EDataType::eTypeMatrix3:
        {
            Matrix3D value;
            if (root->Attribute("val"))
            {
                std::string str = root->Attribute("val");

                f32* val = new f32[9];
                CShaderUniform::parseArrayValue(str, val, 9);
                f32* matrix = value.getPtr();
                memcpy(matrix, val, sizeof(f32)* 9);

                delete[] val;
                val = nullptr;
            }
            CShaderUniform::setUniform(type, name, (void*)&value);

            return true;
        }

        case EDataType::eTypeMatrix4:
        {
            Matrix4D value;
            if (root->Attribute("val"))
            {
                std::string str = root->Attribute("val");

                f32* val = new f32[16];
                CShaderUniform::parseArrayValue(str, val, 16);
                f32* matrix = value.getPtr();
                memcpy(matrix, val, sizeof(f32)* 16);

                delete[] val;
                val = nullptr;
            }
            CShaderUniform::setUniform(type, name, (void*)&value);

            return true;
        }
    }

    return false;
}

void CShaderUniform::parseArrayValue(const std::string& valueStr, f32* array, u32 count)
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
