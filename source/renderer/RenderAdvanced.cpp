#include "RenderAdvanced.h"
#include "utils/Logger.h"

#include "tinyxml2.h"

namespace v3d
{
namespace renderer
{

CRenderAdvanced::CRenderAdvanced()
    : m_instanced(0)
    , m_mode(EPrimitivesMode::ePrimitivesNone)
{
}

CRenderAdvanced::~CRenderAdvanced()
{
}

void CRenderAdvanced::setCountInstance(u32 instanced)
{
    m_instanced = instanced;
}

void CRenderAdvanced::setPrimitiveMode(EPrimitivesMode mode)
{
    m_mode = mode;
}

void CRenderAdvanced::setDrawInterval(u32 offset, u32 amount)
{
    m_interval._begin = offset;
    m_interval._count = amount;
}

u32 CRenderAdvanced::getCountInstance() const
{
    return m_instanced;
}

EPrimitivesMode CRenderAdvanced::getPrimitiveMode() const
{
    return m_mode;
}

const CGeometry::SInterval& CRenderAdvanced::getDrawInterval() const
{
    return m_interval;
}

bool CRenderAdvanced::parse(const tinyxml2::XMLElement* root)
{
    if (!root)
    {
        LOG_ERROR("CRenderAdvanced: Not exist xml advanced element");
        return false;
    }

    const tinyxml2::XMLElement* instanceElement = root->FirstChildElement("instanced");
    if (instanceElement)
    {
        u32 inctance = instanceElement->UnsignedAttribute("val");
        CRenderAdvanced::setCountInstance(inctance);
    }

    const tinyxml2::XMLElement* primitiveElement = root->FirstChildElement("primitive");
    if (primitiveElement && primitiveElement->Attribute("val"))
    {
        std::string primitive = primitiveElement->Attribute("val");
        m_mode = GeometryType::getPrimitivesModeByString(primitive);
        if (m_mode == EPrimitivesMode::ePrimitivesNone)
        {
            LOG_WARNING("CRenderAdvanced: Invalid primitive mode");
        }

        CRenderAdvanced::setPrimitiveMode(m_mode);
    }

    const tinyxml2::XMLElement* drawintervalElement = root->FirstChildElement("drawinterval");
    if (drawintervalElement)
    {
        u32 begin = drawintervalElement->UnsignedAttribute("begin");
        u32 amount = drawintervalElement->UnsignedAttribute("amount");

        CRenderAdvanced::setDrawInterval(begin, amount);
    }

    return true;
}

} //namespace renderer
} //namespace v3d
