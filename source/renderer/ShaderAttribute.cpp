#include "ShaderAttribute.h"

using namespace v3d;
using namespace v3d::renderer;

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
    : m_typeAttr(EShaderAttribute::eAttributeNone)
    , m_attribute("")
{
}

CShaderAttribute::~CShaderAttribute()
{
}

void CShaderAttribute::setAttribute(EShaderAttribute type, const std::string& attribute)
{
    m_typeAttr = type;
    m_attribute = attribute;
}

const std::string& CShaderAttribute::getAttributeName() const
{
    return m_attribute;
}

CShaderAttribute::EShaderAttribute CShaderAttribute::getAttributeType() const
{
    return m_typeAttr;
}