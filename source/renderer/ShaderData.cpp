#include "ShaderData.h"

using namespace v3d;
using namespace v3d::renderer;

//////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string CShaderData::s_shaderDataType[EShaderDataType::eTypeCount] = {
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
    for (int i = 0; i < EShaderDataType::eTypeCount; ++i)
    {
        if (s_shaderDataType[i].compare(name) == 0)
        {
            return (EShaderDataType)i;
        }
    }

    return EShaderDataType::eNone;
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

bool CShaderData::isExistUniform(EDefaultUniformData type)
{
    UniformList::iterator iter = m_uniformList.begin();

    while (iter != m_uniformList.end())
    {
        if ((*iter).second->m_defaultData == type)
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
        m_uniformList[name]->setUniform(EShaderDataType::eInt, name, (void*)(&value));
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(EShaderDataType::eInt, name, (void*)(&value));
        m_uniformList[name] = uniform;
    }
}

void CShaderData::setUniformFloat(const std::string& name, const f32 value)
{
    if (isExistAttribute(name))
    {
        m_uniformList[name]->setUniform(EShaderDataType::eFloat, name, (void*)(&value));
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(EShaderDataType::eFloat, name, (void*)(&value));
        m_uniformList[name] = uniform;
    }
}

void CShaderData::setUniformVector2(const std::string& name, const core::Vector2D& vector)
{
    if (isExistAttribute(name))
    {
        m_uniformList[name]->setUniform(EShaderDataType::eVector2, name, (void*)(&vector));
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(EShaderDataType::eVector2, name, (void*)(&vector));
        m_uniformList[name] = uniform;
    }
}

void CShaderData::setUniformVector3(const std::string& name, const core::Vector3D& vector)
{
    if (isExistAttribute(name))
    {
        m_uniformList[name]->setUniform(EShaderDataType::eVector3, name, (void*)(&vector));
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(EShaderDataType::eVector3, name, (void*)(&vector));
        m_uniformList[name] = uniform;
    }
}

void CShaderData::setUniformVector4(const std::string& name, const core::Vector4D& vector)
{
    if (isExistAttribute(name))
    {
        m_uniformList[name]->setUniform(EShaderDataType::eVector4, name, (void*)(&vector));
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(EShaderDataType::eVector4, name, (void*)(&vector));
        m_uniformList[name] = uniform;
    }
}

void CShaderData::setUniformMatrix3(const std::string& name, const core::Matrix3D& matrix)
{
    if (isExistAttribute(name))
    {
        m_uniformList[name]->setUniform(EShaderDataType::eMatrix3, name, (void*)(&matrix));
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(EShaderDataType::eMatrix3, name, (void*)(&matrix));
        m_uniformList[name] = uniform;
    }
}

void CShaderData::setUniformMatrix4(const std::string& name, const core::Matrix4D& matrix)
{
    if (isExistAttribute(name))
    {
        m_uniformList[name]->setUniform(EShaderDataType::eMatrix4, name, (void*)(&matrix));
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(EShaderDataType::eMatrix4, name, (void*)(&matrix));
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
    //TODO:
}

const std::string CShaderData::findByUniformData(EDefaultUniformData type)
{
    UniformList::iterator iter = m_uniformList.begin();

    while (iter != m_uniformList.end())
    {
        if ((*iter).second->m_defaultData == type)
        {
            return (*iter).first;
        }
        ++iter;
    }
    return "";
}

void CShaderData::setUniformInt(EDefaultUniformData type, const u32 value)
{
    const std::string name = findByUniformData(type);
    if (!name.empty())
    {
        m_uniformList[name]->setUniform(EShaderDataType::eInt, name, (void*)(&value));
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(EShaderDataType::eInt, name, (void*)(&value));
        m_uniformList[name] = uniform;
    }
}

void CShaderData::setUniformFloat(EDefaultUniformData type, const f32 value)
{
    const std::string name = findByUniformData(type);
    if (!name.empty())
    {
        m_uniformList[name]->setUniform(EShaderDataType::eFloat, name, (void*)(&value));
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(EShaderDataType::eFloat, name, (void*)(&value));
        m_uniformList[name] = uniform;
    }
}

void CShaderData::setUniformVector2(EDefaultUniformData type, const core::Vector2D& vector)
{
    const std::string name = findByUniformData(type);
    if (!name.empty())
    {
        m_uniformList[name]->setUniform(EShaderDataType::eVector2, name, (void*)(&vector));
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(EShaderDataType::eVector2, name, (void*)(&vector));
        m_uniformList[name] = uniform;
    }
}

void CShaderData::setUniformVector3(EDefaultUniformData type, const core::Vector3D& vector)
{
    const std::string name = findByUniformData(type);
    if (!name.empty())
    {
        m_uniformList[name]->setUniform(EShaderDataType::eVector3, name, (void*)(&vector));
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(EShaderDataType::eVector3, name, (void*)(&vector));
        m_uniformList[name] = uniform;
    }
}

void CShaderData::setUniformVector4(EDefaultUniformData type, const core::Vector4D& vector)
{
    const std::string name = findByUniformData(type);
    if (!name.empty())
    {
        m_uniformList[name]->setUniform(EShaderDataType::eVector4, name, (void*)(&vector));
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(EShaderDataType::eVector4, name, (void*)(&vector));
        m_uniformList[name] = uniform;
    }
}

void CShaderData::setUniformMatrix3(EDefaultUniformData type, const core::Matrix3D& matrix)
{
    const std::string name = findByUniformData(type);
    if (!name.empty())
    {
        m_uniformList[name]->setUniform(EShaderDataType::eMatrix3, name, (void*)(&matrix));
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(EShaderDataType::eMatrix3, name, (void*)(&matrix));
        m_uniformList[name] = uniform;
    }
}

void CShaderData::setUniformMatrix4(EDefaultUniformData type, const core::Matrix4D& matrix)
{
    const std::string name = findByUniformData(type);
    if (!name.empty())
    {
        m_uniformList[name]->setUniform(EShaderDataType::eMatrix4, name, (void*)(&matrix));
    }
    else
    {
        UniformPtr uniform = std::make_shared<CShaderUniform>();

        uniform->setUniform(EShaderDataType::eMatrix4, name, (void*)(&matrix));
        m_uniformList[name] = uniform;
    }
}