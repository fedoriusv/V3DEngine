#include "ShaderSampler.h"

using namespace v3d;
using namespace v3d::renderer;

CShaderSampler::CShaderSampler()
    : m_attribute("")
{
}

CShaderSampler::~CShaderSampler()
{
}

void CShaderSampler::setSampler(const std::string& attribute)
{
    m_attribute = attribute;
}

const std::string& CShaderSampler::getAttribute() const
{
    return m_attribute;
}