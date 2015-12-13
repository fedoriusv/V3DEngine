#include "ShaderAttribute.h"
#include "utils/Logger.h"

#include "tinyxml2.h"

using namespace v3d;
using namespace renderer;

//////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string CShaderAttribute::s_attributeName[EShaderAttribute::eAttributeCount] = {

    "mesh.positions",
    "mesh.colors",
    "mesh.normals",
    "mesh.binormals",
    "mesh.tangents",
    "mesh.texture0",
    "mesh.texture1",
    "mesh.texture2",
    "mesh.texture3",
};


const std::string& CShaderAttribute::getNameByValue(EShaderAttribute type)
{
    return s_attributeName[type];
}

const CShaderAttribute::EShaderAttribute CShaderAttribute::getValueByName(const std::string& name)
{
    for (int i = 0; i < eAttributeCount; ++i)
    {
        if (s_attributeName[i].compare(name) == 0)
        {
            return (EShaderAttribute)i;
        }
    }

    return eAttributeUser;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

CShaderAttribute::SUserData::SUserData()
    : _size(0U)
    , _count(0U)
    , _data(nullptr)
{
}

CShaderAttribute::SUserData::SUserData(u32 size, u32 count, const void* data)
{
    SUserData::copy(size, count, data);
}

CShaderAttribute::SUserData::~SUserData()
{
    SUserData::free();
}

CShaderAttribute::SUserData& CShaderAttribute::SUserData::operator=(const SUserData& other)
{
    if (this == &other)
    {
        return *this;
    }

    SUserData::free();
    SUserData::copy(other._size, other._count, other._data);

    return *this;
}

void CShaderAttribute::SUserData::copy(u32 size, u32 count, const void* data)
{
    if (count * size > 0 && data)
    {
        _size = size;
        _count = count;
        _data = malloc(count * size);

        memcpy(_data, data, count * size);
    }
}

void CShaderAttribute::SUserData::free()
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


CShaderAttribute::CShaderAttribute()
    : m_type(eAttributeUser)
    , m_name("")
    , m_divisor(0U)
    , m_userData(nullptr)

    , m_id(-1)
{
}

CShaderAttribute::~CShaderAttribute()
{
    if (m_userData)
    {
        delete m_userData;
        m_userData = nullptr;
    }
}

void CShaderAttribute::setID(s32 id)
{
    m_id = id;
}

CShaderAttribute& CShaderAttribute::operator=(const CShaderAttribute& other)
{
    if (this == &other)
    {
        return *this;
    }

    m_name = other.m_name;
    m_type = other.m_type;
    m_divisor = other.m_divisor;
    m_userData = other.m_userData;

    return *this;
}

const std::string& CShaderAttribute::getName() const
{
    return m_name;
}

u32 CShaderAttribute::getDivisor() const
{
    return m_divisor;
}

void* CShaderAttribute::getUserData() const
{
    if (m_userData)
    {
        return m_userData->_data;
    }

    return nullptr;
}

u32 CShaderAttribute::getUserDataSize() const
{
    if (m_userData)
    {
        return m_userData->_size;
    }

    return 0;
}

u32 CShaderAttribute::getUserDataCount() const
{
    if (m_userData)
    {
        return m_userData->_count;
    }

    return 0;
}

CShaderAttribute::EShaderAttribute CShaderAttribute::getType() const
{
    return m_type;
}

u32 CShaderAttribute::getID() const
{
    return m_id;
}

bool CShaderAttribute::parse(const tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("CShaderAttribute: Not exist xml attributes element");
        return false;
    }

    const std::string varName = root->Attribute("name");
    if (varName.empty())
    {
        LOG_ERROR("CShaderAttribute: Cannot find attribute name");
        return false;
    }

    if (root->Attribute("val"))
    {
        const std::string varVal = root->Attribute("val");
        if (varVal.empty())
        {
            LOG_ERROR("CRenderPass: Cannot find attribute val '%s'", varName.c_str());
            return false;
        }

        EShaderAttribute attribureType = CShaderAttribute::getValueByName(varVal);
        if (attribureType == eAttributeUser)
        {
            LOG_ERROR("CRenderPass:Attribute type not found [%s]", varName.c_str());
            return false;
        }

        CShaderAttribute::setAttribute(varName, attribureType);
    }
    else
    {
        LOG_INFO("CRenderPass:User attribute type '%s'", varName.c_str());

        u32 varDiv = root->UnsignedAttribute("div");
        CShaderAttribute::setAttribute(varName, varDiv, 0, 0, nullptr);
    }

    return true;
}

void CShaderAttribute::setAttribute(const std::string & name, EShaderAttribute type)
{
    m_name = name;
    m_type = type;
}

void CShaderAttribute::setAttribute(const std::string & name, u32 divisor, u32 size, u32 count, const void* data)
{
    m_name = name;
    m_type = eAttributeUser;

    m_divisor = divisor;
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
