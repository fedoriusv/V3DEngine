#include "ShaderData.h"

using namespace v3d;
using namespace v3d::renderer;

//////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string CShaderData::s_shaderDataType[EShaderDataType::eDataTypeCount] = {
    "",
    "int",
    "float",
    "vec2",
    "vec3",
    "vec4",
    "mat3",
    "mat4"
};

const std::string& CShaderData::getShaderDataNameByType(EShaderDataType type)
{
    return s_shaderDataType[type];
}

const EShaderDataType CShaderData::getShaderDataTypeByName(const std::string& name)
{
    for (int i = 0; i < EShaderDataType::eDataTypeCount; ++i)
    {
        if (s_shaderDataType[i].compare(name) == 0)
        {
            return (EShaderDataType)i;
        }
    }

    return EShaderDataType::eDataNone;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

CShaderData::CShaderData()
{
}

CShaderData::~CShaderData()
{
    m_attributeList.clear();
    m_uniformList.clear();
}

bool CShaderData::isExistUniform(const std::string& name)
{
    UniformList::iterator iter = m_uniformList.begin();

    while (iter != m_uniformList.end())
    {
        if ((*iter).first.compare(name) == 0)
        {
            return true;
        }
        ++iter;
    }
    return false;
}

bool CShaderData::isExistAttribute(const std::string& name)
{
    AttributeList::iterator iter = m_attributeList.begin();

    while (iter != m_attributeList.end())
    {
        if ((*iter).first.compare(name) == 0)
        {
            return true;
        }
        ++iter;
    }
    return false;
}

void CShaderData::setUniformInt(const std::string& name, const u32 value)
{
    if (isExistAttribute(name))
    {
        m_uniformList[name]->setUniform(eDataInt, name, (void*)(&value));
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(eDataInt, name, (void*)(&value));
        m_uniformList[name] = uniform;
    }
}

void CShaderData::setUniformFloat(const std::string& name, const f32 value)
{
    if (isExistAttribute(name))
    {
        m_uniformList[name]->setUniform(eDataFloat, name, (void*)(&value));
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(eDataFloat, name, (void*)(&value));
        m_uniformList[name] = uniform;
    }
}

void CShaderData::setUniformVector2(const std::string& name, const core::Vector2D& vector)
{
    if (isExistAttribute(name))
    {
        m_uniformList[name]->setUniform(eDataVector2, name, (void*)(&vector));
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(eDataVector2, name, (void*)(&vector));
        m_uniformList[name] = uniform;
    }
}

void CShaderData::setUniformVector3(const std::string& name, const core::Vector3D& vector)
{
    if (isExistAttribute(name))
    {
        m_uniformList[name]->setUniform(eDataVector3, name, (void*)(&vector));
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(eDataVector3, name, (void*)(&vector));
        m_uniformList[name] = uniform;
    }
}

void CShaderData::setUniformVector4(const std::string& name, const core::Vector4D& vector)
{
    if (isExistAttribute(name))
    {
        m_uniformList[name]->setUniform(eDataVector4, name, (void*)(&vector));
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(eDataVector4, name, (void*)(&vector));
        m_uniformList[name] = uniform;
    }
}

void CShaderData::setUniformMatrix3(const std::string& name, const core::Matrix3D& matrix)
{
    if (isExistAttribute(name))
    {
        m_uniformList[name]->setUniform(eDataMatrix3, name, (void*)(&matrix));
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(eDataMatrix3, name, (void*)(&matrix));
        m_uniformList[name] = uniform;
    }
}

void CShaderData::setUniformMatrix4(const std::string& name, const core::Vector4D& matrix)
{
    if (isExistAttribute(name))
    {
        m_uniformList[name]->setUniform(eDataMatrix4, name, (void*)(&matrix));
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(eDataMatrix4, name, (void*)(&matrix));
        m_uniformList[name] = uniform;
    }
}

void CShaderData::addDefaultUniform(const std::string& name, EShaderDataType type, EDefaultUniformData data)
{
    if (isExistAttribute(name))
    {
        m_uniformList[name]->setUniform(type, name, nullptr, data);
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(type, name, nullptr, data);
        m_uniformList[name] = uniform;
    }
}

void CShaderData::addAttribute(const std::string& name, EShaderAttribute type)
{
    if (isExistAttribute(name))
    {
        m_attributeList[name]->setAttribute(type, name);
    }
    else
    {
        AttributePtr uniform = std::make_shared<CShaderAttribute>();

        uniform->setAttribute(type, name);
        m_attributeList[name] = uniform;
    }
}

const AttributeList& CShaderData::getAttributeList() const
{
    return m_attributeList;
}

bool CShaderData::isExistSampler(const std::string& name)
{
    return false;
}

void CShaderData::addSampler(const std::string& name)
{
}