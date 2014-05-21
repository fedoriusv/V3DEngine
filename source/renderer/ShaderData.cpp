#include "ShaderData.h"

using namespace f3d;
using namespace f3d::renderer;

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
        m_uniformList[name]->setUniform(eUniformInt, name, (void*)(&value));
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(eUniformInt, name, (void*)(&value));
        m_uniformList[name] = uniform;
    }
}

void CShaderData::setUniformFloat(const std::string& name, const f32 value)
{
    if (isExistAttribute(name))
    {
        m_uniformList[name]->setUniform(eUniformInt, name, (void*)(&value));
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(eUniformInt, name, (void*)(&value));
        m_uniformList[name] = uniform;
    }
}

void CShaderData::setUniformVector2(const std::string& name, const core::Vector2D& vector)
{
    if (isExistAttribute(name))
    {
        m_uniformList[name]->setUniform(eUniformInt, name, (void*)(&vector));
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(eUniformInt, name, (void*)(&vector));
        m_uniformList[name] = uniform;
    }
}

void CShaderData::setUniformVector3(const std::string& name, const core::Vector3D& vector)
{
    if (isExistAttribute(name))
    {
        m_uniformList[name]->setUniform(eUniformInt, name, (void*)(&vector));
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(eUniformInt, name, (void*)(&vector));
        m_uniformList[name] = uniform;
    }
}

void CShaderData::setUniformVector4(const std::string& name, const core::Vector4D& vector)
{
    if (isExistAttribute(name))
    {
        m_uniformList[name]->setUniform(eUniformInt, name, (void*)(&vector));
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(eUniformInt, name, (void*)(&vector));
        m_uniformList[name] = uniform;
    }
}

void CShaderData::setUniformMatrix3(const std::string& name, const core::Matrix3D& matrix)
{
    if (isExistAttribute(name))
    {
        m_uniformList[name]->setUniform(eUniformInt, name, (void*)(&matrix));
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(eUniformInt, name, (void*)(&matrix));
        m_uniformList[name] = uniform;
    }
}

void CShaderData::setUniformMatrix4(const std::string& name, const core::Vector4D& matrix)
{
    if (isExistAttribute(name))
    {
        m_uniformList[name]->setUniform(eUniformInt, name, (void*)(&matrix));
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(eUniformInt, name, (void*)(&matrix));
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
        m_attributeList[name]->addAttribute(type, name);
    }
    else
    {
        AttributePtr uniform = std::make_shared<CShaderAttribute>();

        uniform->addAttribute(type, name);
        m_attributeList[name] = uniform;
    }
}

bool CShaderData::isExistSampler(const std::string& name)
{
    return false;
}

void CShaderData::addSampler(const std::string& name)
{
}