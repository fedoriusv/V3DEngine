#include "ShaderUniform.h"
#include "utils/Logger.h"

using namespace v3d;
using namespace v3d::renderer;

//////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string CShaderUniform::s_uniformName[EUniformData::eUniformsCount] = {

    "transform.projectionMatrix",
    "transform.modelMatrix",
    "transform.viewMatrix",
    "transform.normalMatrix",
    "transform.viewPosition",
    "transform.orthoMatrix",

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

const EUniformData CShaderUniform::getValueByName(const std::string& name)
{
    for (int i = 0; i < EUniformData::eUniformsCount; ++i)
    {
        if (s_uniformName[i].compare(name) == 0)
        {
            return (EUniformData)i;
        }
    }

    return EUniformData::eUniformUser;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////


CShaderUniform::CShaderUniform()
    : m_uniformType(EDataType::eTypeNone)
    , m_uniformValue (nullptr)
    , m_attribute ("")
    , m_uniformData(EUniformData::eUniformUser)
    , m_id(-1)
{
}

CShaderUniform::~CShaderUniform()
{
    deallocMemory();
}

void CShaderUniform::setUniform(EDataType type, const std::string& attribute, void* value)
{
    m_uniformType  = type;
    m_attribute    = attribute;
    if (value)
    {
        allocMemory(type, value);
    }
}

void CShaderUniform::setUniform(const std::string& attribute, EUniformData data)
{
    m_attribute = attribute;
    m_uniformData = data;

}

void CShaderUniform::allocMemory(EDataType type, void* value)
{
    switch(type)
    {
        case EDataType::eTypeInt:
        {
            if (m_uniformValue == nullptr)
            {
                m_uniformValue = new int();
            }
            memcpy(m_uniformValue, value, sizeof(int));

            return;
        }

        case EDataType::eTypeFloat:
        {
            if (m_uniformValue == nullptr)
            {
                m_uniformValue = new float();
            }
            memcpy(m_uniformValue, value, sizeof(float));

            return;
        }

        case EDataType::eTypeVector2:
        {
            if (m_uniformValue == nullptr)
            {
                m_uniformValue = new core::Vector2D();
            }
            memcpy(m_uniformValue, value, sizeof(core::Vector2D));

            return;
        }

        case EDataType::eTypeVector3:
        {
            if (m_uniformValue == nullptr)
            {
                m_uniformValue = new core::Vector3D();
            }
            memcpy(m_uniformValue, value, sizeof(core::Vector3D));

            return;
        }

        case EDataType::eTypeVector4:
        {
            if (m_uniformValue == nullptr)
            {
                m_uniformValue = new core::Vector4D();
            }
            memcpy(m_uniformValue, value, sizeof(core::Vector4D));

            return;
        }

        case EDataType::eTypeMatrix3:
        {
            if (m_uniformValue == nullptr)
            {
                m_uniformValue = new core::Matrix3D();
            }
            memcpy(m_uniformValue, value, sizeof(core::Matrix3D));

            return;
        }

        case EDataType::eTypeMatrix4:
        {
            if (m_uniformValue == nullptr)
            {
                m_uniformValue = new core::Matrix4D();
            }
            memcpy(m_uniformValue, value, sizeof(core::Matrix4D));

            return;
        }
    }

    LOG_ERROR("CShaderUniform: Type Data not exist");
}

void CShaderUniform::deallocMemory()
{
    if (m_uniformValue != nullptr)
    {
        free(m_uniformValue);
        m_uniformValue = nullptr;
    }
}

CShaderUniform::EDataType CShaderUniform::getUniformType() const
{
    return m_uniformType;
}

void* CShaderUniform::getUniforValue() const
{
    return m_uniformValue;
}

EUniformData CShaderUniform::getUniformData() const
{
    return m_uniformData;
}

s32 CShaderUniform::getUniformID(const s32 index) const
{
    return m_id;
}

const std::string& CShaderUniform::getUniformName() const
{
    return m_attribute;
}

void CShaderUniform::setUniforID(s32 id)
{
    m_id = id;
}
