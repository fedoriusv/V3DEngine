#include "RefCounted.h"

namespace v3d
{
namespace utils
{

CRefCounted::CRefCounted(s32 refCount)
    : m_refCount(refCount)
{
}

CRefCounted::~CRefCounted()
{
}

s32 CRefCounted::getCount() const
{
    return m_refCount;
}


void CRefCounted::grab() const
{
    ++m_refCount;
}

void CRefCounted::drop() const
{
    --m_refCount;
    if (m_refCount == 0)
    {
        delete this;
    }
}

} //namespace utils
} //namespace v3d
