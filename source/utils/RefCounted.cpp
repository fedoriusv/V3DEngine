#include "RefCounted.h"

using namespace v3d;
using namespace utils;

CRefCounted::CRefCounted(s32 refCount)
: m_refCount(refCount)
{
}

CRefCounted::~CRefCounted()
{
}

s32 CRefCounted::GetCount() const
{
    return m_refCount;
}


void CRefCounted::Grab() const
{
    ++m_refCount;
}

void CRefCounted::Drop() const
{
    --m_refCount;
    if (m_refCount == 0)
    {
        delete this;
    }
}
