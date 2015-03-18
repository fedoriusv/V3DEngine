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
    "mesh.benormals",
    "mesh.tangets",
    "mesh.texture0",
    "mesh.texture1",
    "mesh.texture2",
    "mesh.texture3",
};


const std::string& CShaderAttribute::getAttributeNameByType(EShaderAttribute type)
{
    return s_attributeName[type];
}

const CShaderAttribute::EShaderAttribute CShaderAttribute::getAttributeTypeByName(const std::string& name)
{
    for (int i = 0; i < eAttributeCount; ++i)
    {
        if (s_attributeName[i].compare(name) == 0)
        {
            return (EShaderAttribute)i;
        }
    }

    return eAttributeNone;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////


CShaderAttribute::CShaderAttribute()
    : m_type(eAttributeNone)
    , m_attribute("")
{
}

CShaderAttribute::~CShaderAttribute()
{
}

CShaderAttribute& CShaderAttribute::operator=(const CShaderAttribute& other)
{
    if (this == &other)
    {
        return *this;
    }

    m_attribute = other.m_attribute;
    m_type = other.m_type;

    return *this;
}

void CShaderAttribute::setAttribute(const std::string& attribute)
{
    m_attribute = attribute;
}

const std::string& CShaderAttribute::getAttribute() const
{
    return m_attribute;
}

void CShaderAttribute::setType(EShaderAttribute type)
{
    m_type = type;
}

CShaderAttribute::EShaderAttribute CShaderAttribute::getType() const
{
    return m_type;
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
    CShaderAttribute::setAttribute(varName);

    const std::string varVal = root->Attribute("val");
    if (varVal.empty())
    {
        LOG_ERROR("CRenderPass: Cannot find attribute val '%s'", varName.c_str());
        return false;
    }

    EShaderAttribute attribureType = CShaderAttribute::getAttributeTypeByName(varVal);
    CShaderAttribute::setType(attribureType);

    if (attribureType == eAttributeNone)
    {
        LOG_ERROR("CRenderPass:Invalid attribute type '%s'", varName.c_str());
        return false;
    }

    return true;
}