#include "ShaderData.h"

using namespace v3d;
using namespace v3d::renderer;

//////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string CShaderData::s_dataType[CShaderUniform::eTypeCount] = {
    "",
    "int",
    "float",
    "vec2",
    "vec3",
    "vec4",
    "mat3",
    "mat4"
};

const std::string& CShaderData::getDataNameByType(CShaderUniform::EDataType type)
{
    return s_dataType[type];
}

const CShaderUniform::EDataType CShaderData::getDataTypeByName(const std::string& name)
{
    for (int i = 0; i < CShaderUniform::eTypeCount; ++i)
    {
        if (s_dataType[i].compare(name) == 0)
        {
            return (CShaderUniform::EDataType)i;
        }
    }

    return CShaderUniform::eTypeNone;
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
    UniformList::const_iterator iter = m_uniformList.find(name);
    if (iter != m_uniformList.end())
    {
        return true;
    }

    return false;
}

bool CShaderData::isExistUniform(EUniformData type)
{
    UniformList::const_iterator iter = m_defaultUniformList.find(CShaderUniform::s_uniformName[type]);
    if (iter != m_defaultUniformList.end())
    {
        return true;
    }

    return false;
}

bool CShaderData::isExistAttribute(const std::string& name)
{
    AttributeList::const_iterator iter = m_attributeList.find(name);
    if (iter != m_attributeList.end())
    {
        return true;
    }

    return false;

}

void CShaderData::addUniformInt(const std::string& name, const u32 value)
{
    if (isExistUniform(name))
    {
        m_uniformList[name]->setUniform(CShaderUniform::eTypeInt, name, (void*)(&value));
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(CShaderUniform::eTypeInt, name, (void*)(&value));
        m_uniformList[name] = uniform;
    }
}

void CShaderData::addUniformFloat(const std::string& name, const f32 value)
{
    if (isExistUniform(name))
    {
        m_uniformList[name]->setUniform(CShaderUniform::eTypeFloat, name, (void*)(&value));
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(CShaderUniform::eTypeFloat, name, (void*)(&value));
        m_uniformList[name] = uniform;
    }
}

void CShaderData::addUniformVector2(const std::string& name, const core::Vector2D& vector)
{
    if (isExistUniform(name))
    {
        m_uniformList[name]->setUniform(CShaderUniform::eTypeVector2, name, (void*)(&vector));
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(CShaderUniform::eTypeVector2, name, (void*)(&vector));
        m_uniformList[name] = uniform;
    }
}

void CShaderData::addUniformVector3(const std::string& name, const core::Vector3D& vector)
{
    if (isExistUniform(name))
    {
        m_uniformList[name]->setUniform(CShaderUniform::eTypeVector3, name, (void*)(&vector));
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(CShaderUniform::eTypeVector3, name, (void*)(&vector));
        m_uniformList[name] = uniform;
    }
}

void CShaderData::addUniformVector4(const std::string& name, const core::Vector4D& vector)
{
    if (isExistUniform(name))
    {
        m_uniformList[name]->setUniform(CShaderUniform::eTypeVector4, name, (void*)(&vector));
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(CShaderUniform::eTypeVector4, name, (void*)(&vector));
        m_uniformList[name] = uniform;
    }
}

void CShaderData::addUniformMatrix3(const std::string& name, const core::Matrix3D& matrix)
{
    if (isExistUniform(name))
    {
        m_uniformList[name]->setUniform(CShaderUniform::eTypeMatrix3, name, (void*)(&matrix));
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(CShaderUniform::eTypeMatrix3, name, (void*)(&matrix));
        m_uniformList[name] = uniform;
    }
}

void CShaderData::addUniformMatrix4(const std::string& name, const core::Matrix4D& matrix)
{
    if (isExistUniform(name))
    {
        m_uniformList[name]->setUniform(CShaderUniform::eTypeMatrix4, name, (void*)(&matrix));
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(CShaderUniform::eTypeMatrix4, name, (void*)(&matrix));
        m_uniformList[name] = uniform;
    }
}

void CShaderData::addUniform(const std::string& name, CShaderUniform::EDataType type)
{
    if (isExistUniform(name))
    {
        m_uniformList[name]->setUniform(type, name, nullptr);
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(type, name, nullptr);
        m_uniformList[name] = uniform;
    }
}

void CShaderData::addDefaultUniform(const std::string& name, EUniformData data)
{
    if (isExistUniform(data))
    {
        m_defaultUniformList[name]->setUniform(name, data);
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(name, data);
        m_defaultUniformList[name] = uniform;
    }
}

void CShaderData::addAttribute(const std::string& name, CShaderAttribute::EShaderAttribute type)
{
    if (isExistAttribute(name))
    {
        m_attributeList[name]->setAttribute(type, name);
    }
    else
    {
        AttributePtr attribute = std::make_shared<CShaderAttribute>();

        attribute->setAttribute(type, name);
        m_attributeList[name] = attribute;
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

const UniformList& CShaderData::getDefaultUniformList() const
{
    return m_defaultUniformList;
}

const SamplerList& CShaderData::getSamplerList() const
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

void CShaderData::addSampler(const std::string& name)
{
    if (!isExistSampler(name))
    {
        SamplerPtr sampler = std::make_shared<CShaderSampler>();
        sampler->setSampler(name);

        m_samplerList.push_back(sampler);
    }
}
