#include "ShaderUniform.h"
#include "ShaderData.h"
#include "utils/Logger.h"

#include "tinyxml2.h"

using namespace v3d;
using namespace renderer;

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
    : m_type(eTypeNone)
    , m_value(nullptr)
    , m_attribute("")
    , m_data(eUserUniform)
    , m_id(-1)
{
}

CShaderUniform::~CShaderUniform()
{
    deallocMemory();
}

void CShaderUniform::setUniform(EDataType type, const std::string& attribute, void* value)
{
    m_type  = type;
    m_attribute = attribute;
    if (value)
    {
        allocMemory(type, value);
    }
}

void CShaderUniform::setUniform(const std::string& attribute, EUniformData data)
{
    m_attribute = attribute;
    m_data = data;

}

void CShaderUniform::allocMemory(EDataType type, void* value)
{
    switch(type)
    {
        case EDataType::eTypeInt:
        {
            if (m_value == nullptr)
            {
                m_value = new int();
            }
            memcpy(m_value, value, sizeof(int));

            return;
        }

        case EDataType::eTypeFloat:
        {
            if (m_value == nullptr)
            {
                m_value = new float();
            }
            memcpy(m_value, value, sizeof(float));

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

const std::string& CShaderUniform::getAttribute() const
{
    return m_attribute;
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
        LOG_ERROR("CRenderPass: Cannot find uniform va");
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

        uniformType = CShaderData::getDataTypeByName(varType);
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
        pos = str.find(";");
        std::string valStr = str.substr(0, pos);
        str = str.substr(pos + 1, str.size());

        array[i] = ::std::stof(valStr);
    }

    array[count - 1] = ::std::stof(str);
}
