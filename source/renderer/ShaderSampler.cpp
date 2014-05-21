#include "ShaderSampler.h"

using namespace f3d;
using namespace f3d::renderer;

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