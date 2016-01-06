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

        virtual void    bind() = 0;
        virtual void    bindToBuffer(const Buffer* buffer, u32 offset, u32 size) = 0;

        virtual void    unbind() = 0;

        virtual void    setData(EDataUsageType type, u32 size, void* data) = 0;
        virtual void    updateData(u32 offset, u32 size, void* data) = 0;

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
