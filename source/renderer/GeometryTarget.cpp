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

    ASSERT(!_buffer, "Buffer Lvalue exist");
    _buffer = data._buffer;

    _result = data._result;

    return *this;
}

CGeometryTarget::CGeometryTarget()
    : ITarget()
    , m_separated(false)
    , m_mode(EPrimitivesTopology::eTriangles)
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

void CGeometryTarget::addBuffer(const std::string& name, EDataType type, u32 amount)
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
        data._size = amount;
        
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

    if (root->Attribute("type"))
    {
        if (root->Attribute("type") == "interleaved")
        {
            m_separated = false;
        }
        else if (root->Attribute("type") == "separated")
        {
            m_separated = true;
        }
    }

    if (root->Attribute("primitive"))
    {
        std::string mode = root->Attribute("primitive");
        m_mode = RenderState::getPrimitivesTopologyByString(mode);
    }

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
        EDataType type = DataType::getDataTypeByString(varElement->Attribute("type"));
        if (type == EDataType::eTypeNone)
        {
            LOG_ERROR("CGeometryTarget: Attribute '%s' have invalid type", name.c_str());
            return false;
        }

        s32 size = varElement->IntAttribute("size");
        if (size <= 0)
        {
            LOG_ERROR("CGeometryTarget: Attribute '%s' have not size output", name.c_str());
            return false;
        }
        CGeometryTarget::addBuffer(name, type, size);

        varElement = varElement->NextSiblingElement("var");
    }

    return true;
}

} //namespace v3d
} //namespace renderer
