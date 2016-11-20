#include "ShaderData.h"
#include "ShaderAttribute.h"

namespace v3d
{
namespace renderer
{

ShaderData::ShaderData()
    : m_vertexFormatMask(0U)
{
}

ShaderData::ShaderData(const ShaderData& data)
    : m_vertexFormatMask(data.m_vertexFormatMask)
{
    std::for_each(data.m_uniformList.cbegin(), data.m_uniformList.cend(), [this](const UniformPair& item)
    {
        ShaderUniform* uniform = new ShaderUniform(*item.second);
        m_uniformList.insert(UniformList::value_type(item.first, uniform));
    });

    std::for_each(data.m_attributeList.cbegin(), data.m_attributeList.cend(), [this](const AttributePair& item)
    {
        ShaderAttribute* attribute = new ShaderAttribute(*item.second);
        m_attributeList.insert(AttributeList::value_type(item.first, attribute));
    });

    std::for_each(data.m_fragDataList.cbegin(), data.m_fragDataList.cend(), [this](const AttributePair& item)
    {
        ShaderAttribute* attribute = new ShaderAttribute(*item.second);
        m_fragDataList.insert(AttributeList::value_type(item.first, attribute));
    });

    std::for_each(data.m_samplerList.cbegin(), data.m_samplerList.cend(), [this](const CShaderSampler* item)
    {
        CShaderSampler* sampler = new CShaderSampler(*item);
        m_samplerList.push_back(sampler);
    });
}

ShaderData& ShaderData::operator=(const ShaderData& data)
{
    if (&data == this)
    {
        return *this;
    }

    ShaderData::clear();

    std::for_each(data.m_uniformList.cbegin(), data.m_uniformList.cend(), [this](const UniformPair& item)
    {
        ShaderUniform* uniform = new ShaderUniform(*item.second);
        m_uniformList.insert(UniformList::value_type(item.first, uniform));
    });

    std::for_each(data.m_attributeList.cbegin(), data.m_attributeList.cend(), [this](const AttributePair& item)
    {
        ShaderAttribute* attribute = new ShaderAttribute(*item.second);
        m_attributeList.insert(AttributeList::value_type(item.first, attribute));
    });

    std::for_each(data.m_fragDataList.cbegin(), data.m_fragDataList.cend(), [this](const AttributePair& item)
    {
        ShaderAttribute* attribute = new ShaderAttribute(*item.second);
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

ShaderData::~ShaderData()
{
    ShaderData::clear();
}

u32 ShaderData::getVertexFormatMask() const
{
    return m_vertexFormatMask;
}

bool ShaderData::isExistUniform(const std::string& name)
{
    UniformList::const_iterator iter = m_uniformList.find(name);
    if (iter != m_uniformList.end())
    {
        return true;
    }

    return false;
}

bool ShaderData::isExistUniform(ShaderUniform::EUniformData type)
{
    UniformList::const_iterator iter = m_uniformList.find(ShaderUniform::s_uniformName[type]);
    if (iter != m_uniformList.end())
    {
        return true;
    }

    return false;
}

bool ShaderData::isExistAttribute(const std::string& name)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    AttributeList::const_iterator iter = m_attributeList.find(name);
    if (iter != m_attributeList.end())
    {
        return true;
    }

    return false;
}

bool ShaderData::isExistFragData(const std::string& name)
{
    AttributeList::const_iterator iter = m_fragDataList.find(name);
    if (iter != m_fragDataList.end())
    {
        return true;
    }

    return false;
}

bool ShaderData::isExistSampler(const std::string& name)
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

void ShaderData::setUniform(const std::string& name, const s32 value)
{
    if (isExistUniform(name))
    {
        //m_uniformList[name]->setUniform(EShaderDataType::eTypeInt, name, (void*)(&value));
    }
    else
    {
       /* ShaderUniform* uniform = new ShaderUniform();

        uniform->setUniform(EShaderDataType::eTypeInt, name, (void*)(&value));
        m_uniformList[name] = uniform;*/
    }
}

void ShaderData::setUniform(const std::string& name, const f32 value)
{
    if (isExistUniform(name))
    {
        //m_uniformList[name]->setUniform(EShaderDataType::eTypeFloat, name, (void*)(&value));
    }
    else
    {
        //ShaderUniform* uniform = new ShaderUniform();

        ////uniform->setUniform(EShaderDataType::eTypeFloat, name, (void*)(&value));
        //m_uniformList[name] = uniform;
    }
}

void ShaderData::setUniform(const std::string& name, const core::Vector2D& vector)
{
    if (isExistUniform(name))
    {
        //m_uniformList[name]->setUniform(EShaderDataType::eTypeVector2, name, (void*)(&vector));
    }
    else
    {
        //ShaderUniform* uniform = new ShaderUniform();

        ////uniform->setUniform(EShaderDataType::eTypeVector2, name, (void*)(&vector));
        //m_uniformList[name] = uniform;
    }
}

void ShaderData::setUniform(const std::string& name, const core::Vector3D& vector)
{
    if (isExistUniform(name))
    {
        //m_uniformList[name]->setUniform(EShaderDataType::eTypeVector3, name, (void*)(&vector));
    }
    else
    {
        //ShaderUniform* uniform = new ShaderUniform();

        ////uniform->setUniform(EShaderDataType::eTypeVector3, name, (void*)(&vector));
        //m_uniformList[name] = uniform;
    }
}

void ShaderData::setUniform(const std::string& name, const core::Vector4D& vector)
{
    if (isExistUniform(name))
    {
        //m_uniformList[name]->setUniform(EShaderDataType::eTypeVector4, name, (void*)(&vector));
    }
    else
    {
        //ShaderUniform* uniform = new ShaderUniform();

        ////uniform->setUniform(EShaderDataType::eTypeVector4, name, (void*)(&vector));
        //m_uniformList[name] = uniform;
    }
}

void ShaderData::setUniform(const std::string& name, const core::Matrix3D& matrix)
{
    if (isExistUniform(name))
    {
        //m_uniformList[name]->setUniform(EShaderDataType::eTypeMatrix3, name, (void*)(&matrix));
    }
    else
    {
        //ShaderUniform* uniform = new ShaderUniform();

        ////uniform->setUniform(EShaderDataType::eTypeMatrix3, name, (void*)(&matrix));
        //m_uniformList[name] = uniform;
    }
}

void ShaderData::setUniform(const std::string& name, const core::Matrix4D& matrix)
{
    if (isExistUniform(name))
    {
        //m_uniformList[name]->setUniform(EShaderDataType::eTypeMatrix4, name, (void*)(&matrix));
    }
    else
    {
        //ShaderUniform* uniform = new ShaderUniform();

        ////uniform->setUniform(EShaderDataType::eTypeMatrix4, name, (void*)(&matrix));
        //m_uniformList[name] = uniform;
    }
}

void ShaderData::setAttribute(const std::string& name, const std::vector<s32>& data)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    if (isExistAttribute(name))
    {
        //m_attributeList[name]->setAttribute(EShaderDataType::eTypeInt, name, sizeof(s32), (u32)data.size(), data.data());
    }
    else
    {
        ShaderAttribute* attribute = new ShaderAttribute();

        //attribute->setAttribute(EShaderDataType::eTypeInt, name, sizeof(s32), (u32)data.size(), data.data());
        m_attributeList[name] = attribute;
    }
}

void ShaderData::setAttribute(const std::string& name, const std::vector<f32>& data)
{
    if (isExistAttribute(name))
    {
        //m_attributeList[name]->setAttribute(EShaderDataType::eTypeFloat, name, divisor, sizeof(f32), (u32)data.size(), data.data());
    }
    else
    {
        ShaderAttribute* attribute = new ShaderAttribute();

        //attribute->setAttribute(EShaderDataType::eTypeFloat, name, divisor, sizeof(f32), (u32)data.size(), data.data());
        m_attributeList[name] = attribute;
    }
}

void ShaderData::setAttribute(const std::string& name, const std::vector<core::Vector2D>& data)
{
    if (isExistAttribute(name))
    {
        //m_attributeList[name]->setAttribute(EShaderDataType::eTypeVector2, name, divisor, sizeof(core::Vector2D), (u32)data.size(), data.data());
    }
    else
    {
        ShaderAttribute* attribute = new ShaderAttribute();

        //attribute->setAttribute(EShaderDataType::eTypeVector2, name, divisor, sizeof(core::Vector2D), (u32)data.size(), data.data());
        m_attributeList[name] = attribute;
    }
}

void ShaderData::setAttribute(const std::string& name, const std::vector<core::Vector3D>& data)
{
    if (isExistAttribute(name))
    {
        //m_attributeList[name]->setAttribute(EShaderDataType::eTypeVector3, name, divisor, sizeof(core::Vector3D), (u32)data.size(), data.data());
    }
    else
    {
        ShaderAttribute* attribute = new ShaderAttribute();

        //attribute->setAttribute(EShaderDataType::eTypeVector3, name, divisor, sizeof(core::Vector3D), (u32)data.size(), data.data());
        m_attributeList[name] = attribute;
    }
}

void ShaderData::setAttribute(const std::string& name, const std::vector<core::Vector4D>& data)
{
    if (isExistAttribute(name))
    {
        //m_attributeList[name]->setAttribute(EShaderDataType::eTypeVector4, name, divisor, sizeof(core::Vector4D), (u32)data.size(), data.data());
    }
    else
    {
        ShaderAttribute* attribute = new ShaderAttribute();

        //attribute->setAttribute(ShaderDataType::EShaderDataType::eTypeVector4, name, sizeof(core::Vector4D), (u32)data.size(), data.data());
        m_attributeList[name] = attribute;
    }
}

void ShaderData::addUniform(const ShaderUniform* uniform)
{
    const std::string& name = uniform->getName();
    if (!isExistUniform(name))
    {
        m_uniformList[name] = const_cast<ShaderUniform*>(uniform);
    }
}

void ShaderData::addAttribute(const ShaderAttribute* attribute)
{
    const std::string& name = attribute->getName();
    if (!isExistAttribute(name))
    {
        m_attributeList[name] = const_cast<ShaderAttribute*>(attribute);
        //m_vertexFormatMask |= 1 << (attribute->getType() + 1);
    }
}

void ShaderData::addSampler(const CShaderSampler* sampler)
{
    if (!isExistSampler(sampler->getName()))
    {
        m_samplerList.push_back(const_cast<CShaderSampler*>(sampler));
    }
}

void ShaderData::addFragData(const ShaderAttribute* fragData)
{
    const std::string& name = fragData->getName();
    if (!isExistFragData(name))
    {
        m_fragDataList[name] = const_cast<ShaderAttribute*>(fragData);
    }
}

void ShaderData::clear()
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

const AttributeList& ShaderData::getAttributeList() const
{
    return m_attributeList;
}

const AttributeList& ShaderData::getFragDataList() const
{
    return m_fragDataList;
}

const UniformList& ShaderData::getUniformList() const
{
    return m_uniformList;
}

const SamplerList& ShaderData::getSamplerList() const
{
    return m_samplerList;
}

AttributeList& ShaderData::getAttributeList()
{
    return m_attributeList;
}

AttributeList & ShaderData::getFragDataList()
{
    return m_fragDataList;
}

UniformList& ShaderData::getUniformList()
{
    return m_uniformList;
}

SamplerList& ShaderData::getSamplerList()
{
    return m_samplerList;
}

} //namespace renderer
} //namespace v3d
