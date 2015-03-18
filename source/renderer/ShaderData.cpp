#include "ShaderData.h"

using namespace v3d;
using namespace v3d::renderer;

//////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string CShaderData::s_dataType[EDataType::eDataTypeCount] = {
    "",
    "int",
    "float",
    "vec2",
    "vec3",
    "vec4",
    "mat3",
    "mat4"
};

const std::string& CShaderData::getDataNameByType(EDataType type)
{
    return s_dataType[type];
}

EDataType CShaderData::getDataTypeByName(const std::string& name)
{
    for (int i = 0; i < EDataType::eDataTypeCount; ++i)
    {
        if (s_dataType[i].compare(name) == 0)
        {
            return (EDataType)i;
        }
    }

    return EDataType::eTypeNone;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

CShaderData::CShaderData()
{
}

CShaderData::~CShaderData()
{
    m_attributeList.clear();
    m_uniformList.clear();
    m_samplerList.clear();
}

bool CShaderData::isExistUniform(const std::string& name)
{
    UniformList::const_iterator iter = m_uniformList.find(name);
    if (iter != m_uniformList.end())
    {
        return true;
    }

    return false;
}

bool CShaderData::isExistUniform(CShaderUniform::EUniformData type)
{
    UniformList::const_iterator iter = m_uniformList.find(CShaderUniform::s_uniformName[type]);
    if (iter != m_uniformList.end())
    {
        return true;
    }

    return false;
}

bool CShaderData::isExistAttribute(const std::string& name)
{
    AttributeList::const_iterator iter = std::find_if(m_attributeList.begin(), m_attributeList.end(), [name](const AttributePtr& attribute) -> bool
    {
        if (attribute->getAttribute() == name)
        {
            return true;
        }

        return false;
    });

    return (iter != m_attributeList.end());

}

void CShaderData::setUniformInt(const std::string& name, const s32 value)
{
    if (isExistUniform(name))
    {
        m_uniformList[name]->setUniform(EDataType::eTypeInt, name, (void*)(&value));
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(EDataType::eTypeInt, name, (void*)(&value));
        m_uniformList[name] = uniform;
    }
}

void CShaderData::setUniformFloat(const std::string& name, const f32 value)
{
    if (isExistUniform(name))
    {
        m_uniformList[name]->setUniform(EDataType::eTypeFloat, name, (void*)(&value));
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(EDataType::eTypeFloat, name, (void*)(&value));
        m_uniformList[name] = uniform;
    }
}

void CShaderData::setUniformVector2(const std::string& name, const core::Vector2D& vector)
{
    if (isExistUniform(name))
    {
        m_uniformList[name]->setUniform(EDataType::eTypeVector2, name, (void*)(&vector));
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(EDataType::eTypeVector2, name, (void*)(&vector));
        m_uniformList[name] = uniform;
    }
}

void CShaderData::setUniformVector3(const std::string& name, const core::Vector3D& vector)
{
    if (isExistUniform(name))
    {
        m_uniformList[name]->setUniform(EDataType::eTypeVector3, name, (void*)(&vector));
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(EDataType::eTypeVector3, name, (void*)(&vector));
        m_uniformList[name] = uniform;
    }
}

void CShaderData::setUniformVector4(const std::string& name, const core::Vector4D& vector)
{
    if (isExistUniform(name))
    {
        m_uniformList[name]->setUniform(EDataType::eTypeVector4, name, (void*)(&vector));
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(EDataType::eTypeVector4, name, (void*)(&vector));
        m_uniformList[name] = uniform;
    }
}

void CShaderData::setUniformMatrix3(const std::string& name, const core::Matrix3D& matrix)
{
    if (isExistUniform(name))
    {
        m_uniformList[name]->setUniform(EDataType::eTypeMatrix3, name, (void*)(&matrix));
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(EDataType::eTypeMatrix3, name, (void*)(&matrix));
        m_uniformList[name] = uniform;
    }
}

void CShaderData::setUniformMatrix4(const std::string& name, const core::Matrix4D& matrix)
{
    if (isExistUniform(name))
    {
        m_uniformList[name]->setUniform(EDataType::eTypeMatrix4, name, (void*)(&matrix));
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(EDataType::eTypeMatrix4, name, (void*)(&matrix));
        m_uniformList[name] = uniform;
    }
}

void CShaderData::addUniform(const UniformPtr& uniform)
{
    const std::string& name = uniform->getAttribute();
    if (!isExistUniform(name))
    {
        m_uniformList[name] = uniform;
    }
}

void CShaderData::addAttribute(const AttributePtr& attribute)
{
    if (!isExistAttribute(attribute->getAttribute()))
    {
        m_attributeList.push_back(attribute);
    }
}

const AttributeList& CShaderData::getAttributeList() const
{
    return m_attributeList;
}

const UniformList& CShaderData::getUniformList() const
{
    return m_uniformList;
}

const SamplerList& CShaderData::getSamplerList() const
{
    return m_samplerList;
}

AttributeList& CShaderData::getAttributeList()
{
    return m_attributeList;
}

UniformList& CShaderData::getUniformList()
{
    return m_uniformList;
}

SamplerList& CShaderData::getSamplerList()
{
    return m_samplerList;
}

bool CShaderData::isExistSampler(const std::string& name)
{
    SamplerList::const_iterator iter = std::find_if(m_samplerList.begin(), m_samplerList.end(), [name](const SamplerPtr& sampler) -> bool
    {
        if (sampler->getAttribute() == name)
        {
            return true;
        }

        return false;
    });

    return (iter != m_samplerList.end());
}

void CShaderData::addSampler(const SamplerPtr& sampler)
{
    if (!isExistSampler(sampler->getAttribute()))
    {
        m_samplerList.push_back(sampler);
    }
}
