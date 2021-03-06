#ifndef _V3D_REF_COUNTED_H_
#define _V3D_REF_COUNTED_H_

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
        virtual     ~CRefCounted();

        s32         getCount() const;

    private:

        void        grab() const;
        void        drop() const;

        friend void intrusivePtrAddRef(const CRefCounted* obj);
        friend void intrusivePtrRelease(const CRefCounted* obj);

        mutable s32 m_refCount;
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

#endif //_V3D_REF_COUNTED_H_
