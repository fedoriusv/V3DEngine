#include "ShaderData.h"
#include "ShaderAttribute.h"

namespace v3d
{
namespace renderer
{

CShaderData::CShaderData()
    : m_vertexFormatMask(0U)
{
}

CShaderData::CShaderData(const CShaderData& data)
    : m_vertexFormatMask(data.m_vertexFormatMask)
{
    std::for_each(data.m_uniformList.cbegin(), data.m_uniformList.cend(), [this](const UniformPair& item)
    {
        CShaderUniform* uniform = new CShaderUniform(*item.second);
        m_uniformList.insert(UniformList::value_type(item.first, uniform));
    });

    std::for_each(data.m_attributeList.cbegin(), data.m_attributeList.cend(), [this](const AttributePair& item)
    {
        CShaderAttribute* attribute = new CShaderAttribute(*item.second);
        m_attributeList.insert(AttributeList::value_type(item.first, attribute));
    });

    std::for_each(data.m_fragDataList.cbegin(), data.m_fragDataList.cend(), [this](const AttributePair& item)
    {
        CShaderAttribute* attribute = new CShaderAttribute(*item.second);
        m_fragDataList.insert(AttributeList::value_type(item.first, attribute));
    });

    std::for_each(data.m_samplerList.cbegin(), data.m_samplerList.cend(), [this](const CShaderSampler* item)
    {
        CShaderSampler* sampler = new CShaderSampler(*item);
        m_samplerList.push_back(sampler);
    });
}

CShaderData& CShaderData::operator=(const CShaderData& data)
{
    if (&data == this)
    {
        return *this;
    }

    CShaderData::clear();

    std::for_each(data.m_uniformList.cbegin(), data.m_uniformList.cend(), [this](const UniformPair& item)
    {
        CShaderUniform* uniform = new CShaderUniform(*item.second);
        m_uniformList.insert(UniformList::value_type(item.first, uniform));
    });

    std::for_each(data.m_attributeList.cbegin(), data.m_attributeList.cend(), [this](const AttributePair& item)
    {
        CShaderAttribute* attribute = new CShaderAttribute(*item.second);
        m_attributeList.insert(AttributeList::value_type(item.first, attribute));
    });

    std::for_each(data.m_fragDataList.cbegin(), data.m_fragDataList.cend(), [this](const AttributePair& item)
    {
        CShaderAttribute* attribute = new CShaderAttribute(*item.second);
        m_fragDataList.insert(AttributeList::value_type(item.first, attribute));
    });

    std::for_each(data.m_samplerList.cbegin(), data.m_samplerList.cend(), [this](const CShaderSampler* item)
    {
        CShaderSampler* sampler = new CShaderSampler(*item);
        m_samplerList.push_back(sampler);
    });

    m_vertexFormatMask = data.m_vertexFormatMask;

    return *this;
}

CShaderData::~CShaderData()
{
    CShaderData::clear();
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

bool CShaderData::isExistFragData(const std::string& name)
{
    AttributeList::const_iterator iter = m_fragDataList.find(name);
    if (iter != m_fragDataList.end())
    {
        return true;
    }

    return false;
}

bool CShaderData::isExistSampler(const std::string& name)
{
    SamplerList::const_iterator iter = std::find_if(m_samplerList.begin(), m_samplerList.end(), [name](const CShaderSampler* sampler) -> bool
    {
        if (sampler->getName() == name)
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
        CShaderUniform* uniform = new CShaderUniform();

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
        CShaderUniform* uniform = new CShaderUniform();

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
        CShaderUniform* uniform = new CShaderUniform();

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
        CShaderUniform* uniform = new CShaderUniform();

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
        CShaderUniform* uniform = new CShaderUniform();

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
        CShaderUniform* uniform = new CShaderUniform();

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
        CShaderUniform* uniform = new CShaderUniform();

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
        CShaderAttribute* attribute = new CShaderAttribute();

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
        CShaderAttribute* attribute = new CShaderAttribute();

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
        CShaderAttribute* attribute = new CShaderAttribute();

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
        CShaderAttribute* attribute = new CShaderAttribute();

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
        CShaderAttribute* attribute = new CShaderAttribute();

        attribute->setAttribute(EDataType::eTypeVector4, name, divisor, sizeof(core::Vector4D), (u32)data.size(), data.data());
        m_attributeList[name] = attribute;
    }
}

void CShaderData::addUniform(const CShaderUniform* uniform)
{
    const std::string& name = uniform->getName();
    if (!isExistUniform(name))
    {
        m_uniformList[name] = const_cast<CShaderUniform*>(uniform);
    }
}

void CShaderData::addAttribute(const CShaderAttribute* attribute)
{
    const std::string& name = attribute->getName();
    if (!isExistAttribute(name))
    {
        m_attributeList[name] = const_cast<CShaderAttribute*>(attribute);
        m_vertexFormatMask |= 1 << (attribute->getType() + 1);
    }
}

void CShaderData::addSampler(const CShaderSampler* sampler)
{
    if (!isExistSampler(sampler->getName()))
    {
        m_samplerList.push_back(const_cast<CShaderSampler*>(sampler));
    }
}

void CShaderData::addFragData(const CShaderAttribute* fragData)
{
    const std::string& name = fragData->getName();
    if (!isExistFragData(name))
    {
        m_fragDataList[name] = const_cast<CShaderAttribute*>(fragData);
    }
}

void CShaderData::clear()
{
    for (auto& item : m_uniformList)
    {
        delete item.second;
        item.second = nullptr;
    }
    m_uniformList.clear();

    for (auto& item : m_attributeList)
    {
        delete item.second;
        item.second = nullptr;
    }
    m_attributeList.clear();
    m_vertexFormatMask = 0U;

    for (auto& item : m_fragDataList)
    {
        delete item.second;
        item.second = nullptr;
    }
    m_fragDataList.clear();

    for (auto& item : m_samplerList)
    {
        delete item;;
        item = nullptr;
    }
    m_samplerList.clear();
}

const AttributeList& CShaderData::getAttributeList() const
{
    return m_attributeList;
}

const AttributeList& CShaderData::getFragDataList() const
{
    return m_fragDataList;
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

AttributeList & CShaderData::getFragDataList()
{
    return m_fragDataList;
}

UniformList& CShaderData::getUniformList()
{
    return m_uniformList;
}

SamplerList& CShaderData::getSamplerList()
{
    return m_samplerList;
}

} //namespace renderer
} //namespace v3d
