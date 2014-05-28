#include "ShaderAttribute.h"

using namespace f3d;
using namespace f3d::renderer;

//////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string CShaderAttribute::s_shaderAttributeName[EShaderAttribute::eAttributeCount] = {

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


const std::string& CShaderAttribute::getShaderAttributeNameByType(EShaderAttribute type)
{
    return s_shaderAttributeName[type];
}

const EShaderAttribute CShaderAttribute::getShaderAttributeTypeByName(const std::string& name)
{
    for (int i = 0; i < eAttributeCount; ++i)
    {
        if (s_shaderAttributeName[i].compare(name) == 0)
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
    m_type = EObjectType::eTypeShaderAttribute;
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

EShaderAttribute CShaderAttribute::getAttributeType() const
{
    return m_typeAttr;
}