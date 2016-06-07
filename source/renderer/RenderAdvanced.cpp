#include "RenderAdvanced.h"
#include "utils/Logger.h"

#include "tinyxml2.h"

namespace v3d
{
namespace renderer
{

CRenderAdvanced::CRenderAdvanced()
    : m_instanced(0U)
    , m_patches(0U)
    , m_mode(EPrimitivesMode::ePrimitivesNone)
{
}

CRenderAdvanced::~CRenderAdvanced()
{
}

void CRenderAdvanced::setCountPatches(u32 patches)
{
    m_patches = patches;
}

void CRenderAdvanced::setPatchLevel(f32 inner, f32 outer)
{
    m_patchInnerLevel = inner;
    m_patchOuterLevel = outer;
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

u32 CRenderAdvanced::getCountPatches() const
{
    return m_patches;
}

f32 CRenderAdvanced::getPatchInnerLevel() const
{
    return m_patchInnerLevel;
}

f32 CRenderAdvanced::getPatchOuterLevel() const
{
    return m_patchOuterLevel;
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
        EPrimitivesMode mode = GeometryType::getPrimitivesModeByString(primitive);
        if (mode == EPrimitivesMode::ePrimitivesNone)
        {
            LOG_WARNING("CRenderAdvanced: Invalid primitive mode");
        }

        CRenderAdvanced::setPrimitiveMode(mode);
    }

    const tinyxml2::XMLElement* drawintervalElement = root->FirstChildElement("drawinterval");
    if (drawintervalElement)
    {
        u32 begin = drawintervalElement->UnsignedAttribute("begin");
        u32 amount = drawintervalElement->UnsignedAttribute("amount");

        CRenderAdvanced::setDrawInterval(begin, amount);
    }

    const tinyxml2::XMLElement* patchesElement = root->FirstChildElement("patches");
    if (patchesElement)
    {
        u32 patches = patchesElement->UnsignedAttribute("val");
        if (patches > 0)
        {
            if (CRenderAdvanced::getPrimitiveMode() != EPrimitivesMode::ePatches)
            {
                LOG_WARNING("If used patches draw mode must be 'ePatches'");
                CRenderAdvanced::setPrimitiveMode(EPrimitivesMode::ePatches);
            }
        }

        CRenderAdvanced::setCountPatches(patches);

        f32 inner = patchesElement->FloatAttribute("inner");
        f32 outer = patchesElement->FloatAttribute("outer");
        CRenderAdvanced::setPatchLevel(inner, outer);
    }

    return true;
}

} //namespace renderer
} //namespace v3d
