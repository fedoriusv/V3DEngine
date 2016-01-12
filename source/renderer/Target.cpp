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

} //namespace renderer
} //namespace v3d
