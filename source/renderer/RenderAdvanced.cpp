#include "RenderAdvanced.h"
#include "utils/Logger.h"

#include "tinyxml2.h"

using namespace v3d;
using namespace renderer;

CRenderAdvanced::CRenderAdvanced()
    : m_instanced(0)
{
}

CRenderAdvanced::~CRenderAdvanced()
{
}

void CRenderAdvanced::setCountInstance(u32 instanced)
{
    m_instanced = instanced;
}

u32 CRenderAdvanced::getCountInstance() const
{
    return m_instanced;
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

    return true;
}