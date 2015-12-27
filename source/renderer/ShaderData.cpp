#include "ShaderData.h"

using namespace v3d;
using namespace renderer;

CShaderData::CShaderData()
: m_vertexFormatMask(0U)
{
}

CShaderData::~CShaderData()
{
    m_attributeList.clear();
    m_uniformList.clear();
    m_samplerList.clear();
    m_vertexFormatMask = 0U;
}

u32 CShaderData::getVertexFormatMask() const
{
    return m_vertexFormatMask;
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
    AttributeList::const_iterator iter = m_attributeList.find(name);
    if (iter != m_attributeList.end())
    {
        return true;
    }

    return false;
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

void CShaderData::setUniform(const std::string& name, const s32 value)
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

void CShaderData::setUniform(const std::string& name, const f32 value)
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

void CShaderData::setUniform(const std::string& name, const core::Vector2D& vector)
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

void CShaderData::setUniform(const std::string& name, const core::Vector3D& vector)
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

void CShaderData::setUniform(const std::string& name, const core::Vector4D& vector)
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

void CShaderData::setUniform(const std::string& name, const core::Matrix3D& matrix)
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

void CShaderData::setUniform(const std::string& name, const core::Matrix4D& matrix)
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

void CShaderData::setAttribute(const std::string& name, u32 divisor, const std::vector<s32>& data)
{
    if (isExistAttribute(name))
    {
        m_attributeList[name]->setAttribute(EDataType::eTypeInt, name, divisor, sizeof(s32), (u32)data.size(), data.data());
    }
    else
    {
        AttributePtr attribute = std::make_shared<CShaderAttribute>();

        attribute->setAttribute(EDataType::eTypeInt, name, divisor, sizeof(s32), (u32)data.size(), data.data());
        m_attributeList[name] = attribute;
    }
}

void CShaderData::setAttribute(const std::string& name, u32 divisor, const std::vector<f32>& data)
{
    if (isExistAttribute(name))
    {
        m_attributeList[name]->setAttribute(EDataType::eTypeFloat, name, divisor, sizeof(f32), (u32)data.size(), data.data());
    }
    else
    {
        AttributePtr attribute = std::make_shared<CShaderAttribute>();

        attribute->setAttribute(EDataType::eTypeFloat, name, divisor, sizeof(f32), (u32)data.size(), data.data());
        m_attributeList[name] = attribute;
    }
}

void CShaderData::setAttribute(const std::string& name, u32 divisor, const std::vector<core::Vector2D>& data)
{
    if (isExistAttribute(name))
    {
        m_attributeList[name]->setAttribute(EDataType::eTypeVector2, name, divisor, sizeof(core::Vector2D), (u32)data.size(), data.data());
    }
    else
    {
        AttributePtr attribute = std::make_shared<CShaderAttribute>();

        attribute->setAttribute(EDataType::eTypeVector2, name, divisor, sizeof(core::Vector2D), (u32)data.size(), data.data());
        m_attributeList[name] = attribute;
    }
}

void CShaderData::setAttribute(const std::string& name, u32 divisor, const std::vector<core::Vector3D>& data)
{
    if (isExistAttribute(name))
    {
        m_attributeList[name]->setAttribute(EDataType::eTypeVector3, name, divisor, sizeof(core::Vector3D), (u32)data.size(), data.data());
    }
    else
    {
        AttributePtr attribute = std::make_shared<CShaderAttribute>();

        attribute->setAttribute(EDataType::eTypeVector3, name, divisor, sizeof(core::Vector3D), (u32)data.size(), data.data());
        m_attributeList[name] = attribute;
    }
}

void CShaderData::setAttribute(const std::string& name, u32 divisor, const std::vector<core::Vector4D>& data)
{
    if (isExistAttribute(name))
    {
        m_attributeList[name]->setAttribute(EDataType::eTypeVector4, name, divisor, sizeof(core::Vector4D), (u32)data.size(), data.data());
    }
    else
    {
        AttributePtr attribute = std::make_shared<CShaderAttribute>();

        attribute->setAttribute(EDataType::eTypeVector4, name, divisor, sizeof(core::Vector4D), (u32)data.size(), data.data());
        m_attributeList[name] = attribute;
    }
}

void CShaderData::addUniform(const UniformPtr& uniform)
{
    const std::string& name = uniform->getName();
    if (!isExistUniform(name))
    {
        m_uniformList[name] = uniform;
    }
}

void CShaderData::addAttribute(const AttributePtr& attribute)
{
    const std::string& name = attribute->getName();
    if (!isExistAttribute(name))
    {
        m_attributeList[name] = attribute;
        m_vertexFormatMask |= 1 << (attribute->getType() + 1);
    }
}

void CShaderData::addSampler(const SamplerPtr& sampler)
{
    if (!isExistSampler(sampler->getAttribute()))
    {
        m_samplerList.push_back(sampler);
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
