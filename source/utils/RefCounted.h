#pragma once

#include "common.h"

namespace v3d
{
namespace utils
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CRefCounted
    {
    public:

        explicit    CRefCounted(s32 refCount = 0);
        s32         getCount() const;

    protected:

        virtual     ~CRefCounted();

    private:

        void        grab() const;
        void        drop() const;

        friend void intrusivePtrAddRef(const CRefCounted* obj);
        friend void intrusivePtrRelease(const CRefCounted* obj);

        mutable std::atomic<s32> m_refCount;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    inline void intrusivePtrAddRef(const CRefCounted* obj)
    {
        obj->grab();
    }

    inline void intrusivePtrRelease(const CRefCounted* obj)
    {
        obj->drop();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace utils
} //namespace v3d
