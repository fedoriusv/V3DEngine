#include "Buffer.h"

namespace v3d
{
namespace renderer
{

Buffer::Buffer(EBufferTarget target)
    : m_target(target)
{
}

Buffer::~Buffer()
{
}

EBufferTarget Buffer::getTarget() const
{
    return m_target;
}

} //namespace renderer
} //namespace v3d
