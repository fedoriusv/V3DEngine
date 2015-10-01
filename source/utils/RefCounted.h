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

        explicit CRefCounted(s32 refCount = 0);
        virtual ~CRefCounted();

        s32 GetCount() const;

    private:

        void Grab() const;
        void Drop() const;

        friend void intrusivePtrAddRef(const CRefCounted* obj);
        friend void intrusivePtrRelease(const CRefCounted* obj);

        mutable s32 m_refCount;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    inline void intrusivePtrAddRef(const CRefCounted* obj)
    {
        obj->Grab();
    }
    inline void intrusivePtrRelease(const CRefCounted* obj)
    {
        obj->Drop();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_REF_COUNTED_H_