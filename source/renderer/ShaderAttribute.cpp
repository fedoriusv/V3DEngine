#include "ShaderAttribute.h"
#include "utils/Logger.h"

#include "tinyxml2.h"

namespace v3d
{
namespace renderer
{

//////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string ShaderAttribute::s_attributeName[ShaderAttribute::eAttribCount] = 
{

    "mesh.position",
    "mesh.color",
    "mesh.normal",
    "mesh.binormal",
    "mesh.tangent",
    "mesh.texture0",
    "mesh.texture1",
    "mesh.texture2",
    "mesh.texture3",

    "partical.position",
    "partical.color",
    "partical.velocity",
    "particals.lifetime",
    "partical.size",
    "partical.type",

    "attachment.color",
};


const std::string& ShaderAttribute::getAttributeNameByValue(EShaderAttribute type)
{
    return s_attributeName[type];
}

const ShaderAttribute::EShaderAttribute ShaderAttribute::getValueByAttributeName(const std::string& name)
{
    for (int i = 0; i < EShaderAttribute::eAttribCount; ++i)
    {
        if (s_attributeName[i].compare(name) == 0)
        {
            return (EShaderAttribute)i;
        }
    }

    return EShaderAttribute::eAttribUser;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

ShaderAttribute::SUserData::SUserData()
    : _size(0U)
    , _count(0U)
    , _data(nullptr)
{
}

ShaderAttribute::SUserData::SUserData(u32 size, u32 count, const void* data)
{
    SUserData::copy(size, count, data);
}

ShaderAttribute::SUserData::~SUserData()
{
    SUserData::free();
}

ShaderAttribute::SUserData& ShaderAttribute::SUserData::operator=(const SUserData& other)
{
    if (this == &other)
    {
        return *this;
    }

    SUserData::free();
    SUserData::copy(other._size, other._count, other._data);

    return *this;
}

void ShaderAttribute::SUserData::copy(u32 size, u32 count, const void* data)
{
    if (count * size > 0 && data)
    {
        _size = size;
        _count = count;
        _data = malloc(count * size);

        memcpy(_data, data, count * size);
    }
}

void ShaderAttribute::SUserData::free()
{
    if (_data != nullptr)
    {
        ::free(_data);
        _data = nullptr;
    }

    _size = 0U;
    _count = 0U;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////


ShaderAttribute::ShaderAttribute()
    : m_name("")

    , m_type(ShaderDataType::EDataType::eUnknown)
    , m_channel(eAttribUser)

    , m_location(-1)

    , m_userData(nullptr)
{
}

ShaderAttribute::ShaderAttribute(const ShaderAttribute& attribute)
    : m_name(attribute.m_name)

    , m_type(attribute.m_type)
    , m_channel(attribute.m_channel)

    , m_location(attribute.m_location)

    , m_userData(nullptr)
{
    if (attribute.m_channel != EShaderAttribute::eAttribUser)
    {
        ShaderAttribute::setAttribute(attribute.m_name, attribute.m_channel);
    }
    else
    {
        if (attribute.m_userData)
        {
            ShaderAttribute::setAttribute(attribute.m_type, attribute.m_name, attribute.m_userData->_size, attribute.m_userData->_count, attribute.m_userData->_data);
        }
    }
}

ShaderAttribute& ShaderAttribute::operator=(const ShaderAttribute& attribute)
{
    if (this == &attribute)
    {
        return *this;
    }

    if (m_userData)
    {
        delete m_userData;
        m_userData = nullptr;
    }

    if (attribute.m_channel != EShaderAttribute::eAttribUser)
    {
        ShaderAttribute::setAttribute(attribute.m_name, attribute.m_channel);
    }
    else
    {
        if (attribute.m_userData)
        {
            ShaderAttribute::setAttribute(attribute.m_type, attribute.m_name, attribute.m_userData->_size, attribute.m_userData->_count, attribute.m_userData->_data);
        }
    }

    m_location = attribute.m_location;

    return *this;
}

ShaderAttribute::~ShaderAttribute()
{
    if (m_userData)
    {
        delete m_userData;
        m_userData = nullptr;
    }
}

const std::string& ShaderAttribute::getName() const
{
    return m_name;
}

void* ShaderAttribute::getUserData() const
{
    if (m_userData)
    {
        return m_userData->_data;
    }

    return nullptr;
}

u32 ShaderAttribute::getUserDataSize() const
{
    if (m_userData)
    {
        return m_userData->_size;
    }

    return 0;
}

u32 ShaderAttribute::getUserDataCount() const
{
    if (m_userData)
    {
        return m_userData->_count;
    }

    return 0;
}

ShaderAttribute::EShaderAttribute ShaderAttribute::getChannel() const
{
    return m_channel;
}

ShaderDataType::EDataType ShaderAttribute::getDataType() const
{
    return m_type;
}

u32 ShaderAttribute::getLocation() const
{
    return m_location;
}

bool ShaderAttribute::parse(const tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("ShaderAttribute: Not exist xml attributes element");
        return false;
    }

    const std::string varName = root->Attribute("name");
    if (varName.empty())
    {
        LOG_ERROR("ShaderAttribute: Cannot find attribute name");
        return false;
    }

    if (root->Attribute("val"))
    {
        const std::string varVal = root->Attribute("val");
        if (varVal.empty())
        {
            LOG_ERROR("RenderPass: Cannot find attribute val '%s'", varName.c_str());
            return false;
        }

        EShaderAttribute attribureData = ShaderAttribute::getValueByAttributeName(varVal);
        if (attribureData == EShaderAttribute::eAttribUser)
        {
            LOG_ERROR("RenderPass: Attribute type not found [%s]", varName.c_str());
            return false;
        }

        ShaderAttribute::setAttribute(varName, attribureData);
    }
    else
    {
        LOG_INFO("RenderPass: User attribute type '%s'", varName.c_str());

        if (!root->Attribute("type"))
        {
            LOG_ERROR("RenderPass: Cannot find attribute type in '%s'", varName.c_str());
            return false;
        }
        const std::string varType = root->Attribute("type");

        ShaderDataType::EDataType attributeType = ShaderDataType::getDataTypeByString(varType);
        if (attributeType == ShaderDataType::EDataType::eUnknown)
        {
            LOG_ERROR("RenderPass: Cannot find attribute type in '%s'", varName.c_str());
            return false;
        }

        u32 varDiv = root->UnsignedAttribute("div");
        ShaderAttribute::setAttribute(attributeType, varName, 0, 0, nullptr);
    }

    return true;
}

void ShaderAttribute::setAttribute(const std::string& name, EShaderAttribute data)
{
   /* m_name = name;
    m_channel = data;

    switch (m_channel)
    {
    case EShaderAttribute::eAttribVertexPosition:
    case EShaderAttribute::eAttribVertexNormal:
    case EShaderAttribute::eAttribVertexBinormal:
    case EShaderAttribute::eAttribVertexTangent:
    case EShaderAttribute::eAttribVertexColor:
    case EShaderAttribute::eAttribParticalPosition:
    case EShaderAttribute::eAttribParticalColor:
    case EShaderAttribute::eAttribParticalVelocity:
        m_type = ShaderDataType::eVector3f;
        break;

    case EShaderAttribute::eAttribVertexTexture0:
    case EShaderAttribute::eAttribVertexTexture1:
    case EShaderAttribute::eAttribVertexTexture2:
    case EShaderAttribute::eAttribVertexTexture3:
        m_type = ShaderDataType::eVector2f;
        break;

    case EShaderAttribute::eAttribParticalLifeTime:
    case EShaderAttribute::eAttribParticalSize:
        m_type = ShaderDataType::eFloat;
        break;

    case EShaderAttribute::eAttribParticalType:
        m_type = ShaderDataType::eInt;
        break;

    default:
        m_type = ShaderDataType::eUnknown;
        break;
    }*/
}

void ShaderAttribute::setAttribute(ShaderDataType::EDataType type, const std::string& name, u32 size, u32 count, const void* data)
{
    m_name = name;
    m_channel = eAttribUser;
    m_type = type;

    if (count > 0 && data != nullptr)
    {
        if (m_userData)
        {
            delete m_userData;
            m_userData = nullptr;
        }

        m_userData = new SUserData(size, count, data);
    }
}

} //namespace renderer
} //namespace v3d
