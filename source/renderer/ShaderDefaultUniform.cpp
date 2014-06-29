#include "ShaderDefaultUniform.h"

using namespace v3d;
using namespace v3d::renderer;

//////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string CShaderDefaultUniform::s_uniformName[EUniformData::eUniformsCount] = {

    "transform.projectionMatrix",
    "transform.modelMatrix",
    "transform.viewMatrix",
    "transform.normalMatrix",
    "transform.viewPosition",
    "transform.orthoMatrix",

    "material.ambient",
    "material.diffuse",
    "material.specular",
    "material.emission",
    "material.shininess",

    "lights.count",

    "light.position",
    "light.ambient",
    "light.diffuse",
    "light.specular",
    "light.direction",
    "light.attenuation",
    "light.radius",
};


const std::string& CShaderDefaultUniform::getNameByValue(EUniformData type)
{
    return s_uniformName[type];
}

const CShaderDefaultUniform::EUniformData CShaderDefaultUniform::getValueByName(const std::string& name)
{
    for (int i = 0; i < EUniformData::eUniformsCount; ++i)
    {
        if (s_uniformName[i].compare(name) == 0)
        {
            return (EUniformData)i;
        }
    }

    return EUniformData::eUniformUnknown;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

CShaderDefaultUniform::CShaderDefaultUniform()
: m_uniformType(EUniformData::eUniformUnknown)
, m_attribute("")
{
}

CShaderDefaultUniform::~CShaderDefaultUniform()
{
}

void CShaderDefaultUniform::setUniform(const std::string& attribute, EUniformData data)
{
    m_uniformType = data;
    m_attribute = attribute;
}

CShaderDefaultUniform::EUniformData CShaderDefaultUniform::getUniformType() const
{
    return m_uniformType;
}
