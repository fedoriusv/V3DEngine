#include "Target.h"

namespace v3d
{
namespace renderer
{

ITarget::ITarget()
    : m_name("")
{
}

ITarget::~ITarget()
{
}

const std::string& ITarget::getName() const
{
    return m_name;
}

ITarget::ETagetType ITarget::getTagetType() const
{
    return m_targetType;
}

void ITarget::setName(const std::string& name)
{
    m_name = name;
}

void ITarget::setTargetType(ETagetType type)
{
    m_targetType = type;
}

} //namespace renderer
} //namespace v3d
