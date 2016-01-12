#include "GeometryTarget.h"
#include "Buffer.h"
#include "utils/Logger.h"
#include "Engine.h"

#include "tinyxml2.h"

namespace v3d
{
namespace renderer
{

CGeometryTarget::SBufferData::SBufferData()
    : _name("")
    , _type(EDataType::eTypeNone)
    , _size(0U)
    , _buffer(nullptr)

    , _result(0)
{
}

CGeometryTarget::SBufferData::~SBufferData()
{
    if (_buffer)
    {
        delete _buffer;
        _buffer = nullptr;
    }
}

CGeometryTarget::SBufferData& CGeometryTarget::SBufferData::operator=(const SBufferData& data)
{
    if (this == &data)
    {
        return *this;
    }

    _name = data._name;
    _type = data._type;
    _size = data._size;
    _buffer = data._buffer; //TODO need owner

    _result = data._result;

    return *this;
}

CGeometryTarget::CGeometryTarget()
    : ITarget()
{
    m_targetType = ITarget::ETagetType::eGeometryTarget;
    m_name = "";
}

CGeometryTarget::~CGeometryTarget()
{
    for (auto& buff : m_bufferStrings)
    {
        delete[] buff;
        buff = nullptr;
    }
    m_bufferStrings.clear();
}

const std::vector<const c8*>& CGeometryTarget::getBufferStrings() const
{
    return m_bufferStrings;
}

const CGeometryTarget::SBufferData* CGeometryTarget::getBuffer(const std::string& name)
{
    auto findPred = [name](const SBufferData& item) -> bool
    {
        if (item._name == name)
        {
            return true;
        }

        return false;
    };

    std::vector<SBufferData>::const_iterator iter = std::find_if(m_bufferList.cbegin(), m_bufferList.cend(), findPred);
    if (iter != m_bufferList.cend())
    {
        return &(*iter);
    }

    return nullptr;
}

void CGeometryTarget::addBuffer(const std::string& name, EDataType type)
{
    auto findPred = [name](const SBufferData& item) -> bool
    {
        if (item._name == name)
        {
            return true;
        }

        return false;
    };

    if (std::find_if(m_bufferList.cbegin(), m_bufferList.cend(), findPred) == m_bufferList.cend())
    {
        SBufferData data;
        data._name = name;
        data._type = type;
        
        m_bufferList.push_back(data);

        c8* buff = new c8[name.size() + 1];
        memcpy(buff, name.data(), name.size() + 1);
        m_bufferStrings.push_back(buff);
    }

}

bool CGeometryTarget::parse(const tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("CGeometryTarget: Not exist xml element <transformfeedback>");
        return false;
    }

    if (!root->Attribute("name"))
    {
        LOG_ERROR("CGeometryTarget: Transformfeedback have not name");
        return false;
    }
    m_name = root->Attribute("name");

    const tinyxml2::XMLElement* varElement = root->FirstChildElement("var");
    while (varElement)
    {
        if (!varElement->Attribute("name"))
        {
            LOG_ERROR("CGeometryTarget: Attribute have not name");
            return false;
        }
        std::string name = varElement->Attribute("name");

        if (!varElement->Attribute("type"))
        {
            LOG_ERROR("CGeometryTarget: Attribute '%s' have not type", name.c_str());
            return false;
        }
        EDataType type = CDataType::getDataTypeByString(varElement->Attribute("type"));
        if (type == EDataType::eTypeNone)
        {
            LOG_ERROR("CGeometryTarget: Attribute '%s' have invalid type", name.c_str());
            return false;
        }

        //TODO: add separate mode, amount output, draw mode, Flush force mode

        CGeometryTarget::addBuffer(name, type);

        varElement = varElement->NextSiblingElement("var");
    }

    return true;
}

} //namespace v3d
} //namespace renderer
