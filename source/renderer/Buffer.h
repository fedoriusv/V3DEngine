#ifndef _V3D_BUFFER_H_
#define _V3D_BUFFER_H_

#include "DataTypes.h"
#include "GeometryTypes.h"

namespace v3d
{
namespace renderer
{

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * Interface for buffer objects management.
    */
    class Buffer
    {
    public:

        explicit        Buffer(EBufferTarget target);
        virtual         ~Buffer();

        virtual void    bind() const = 0;
        virtual void    bindToTarget(EBufferTarget target, u32 offset, u32 size) const  = 0;

        virtual void    unbind() const  = 0;

        virtual void    set(EDataUsageType type, u32 size, const void* data)    = 0;
        virtual void    update(u32 offset, u32 size, const void* data)          = 0;
        virtual void    read(u32 offset, u32 size, void* data) const            = 0;

        virtual void*   map(u32 access) = 0;
        virtual bool    unmap() = 0;

        EBufferTarget   getTarget() const;

    protected:

        EBufferTarget   m_target;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_V3D_BUFFER_H_
