#pragma once

#include "utils/RefCounted.h"
#include "utils/IntrusivePtr.h"
#include "DataTypes.h"
#include "GeometryTypes.h"


namespace v3d
{
namespace renderer
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EBufferTarget
    {
        eStagingBuffer = -1,

        eVertexBuffer,
        eIndexBuffer,
        eStorageBuffer,
        eUniformsBuffer,

        eBufferTargetCount
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    /*enum EDataUsageType
    {
        eWriteStatic,
        eWriteDynamic,
        eReadStatic,
        eReadDynamic,
        eCopyStatic,
        eCopyDynamic,

        eDataUsageTypeCount
    };*/

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class Buffer;
    class RenderThread;


    using BufferPtr = utils::TIntrusivePtr<Buffer>;

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * Buffer resource.
    * Client thread. Provide transfer data to render thread
    */
    class Buffer : public utils::CRefCounted //, public utils::TCloneable<BufferPtr>
    {
    public:

        Buffer(EBufferTarget target, EDataUsageType type, u32 size, const void* data);

        Buffer(const Buffer&) = delete;
        Buffer&                 operator=(const Buffer&) = delete;

        virtual                 ~Buffer();

        virtual void            bind() const;
        virtual void            bindToTarget(EBufferTarget target, u32 offset, u32 size) const;

        virtual void            unbind() const;

        virtual void            update(u32 offset, u32 size, const void* data);
        virtual void            read(u32 offset, u32 size, void* const data);

        virtual void* const     map(u32 offset, u32 size);
        virtual bool            unmap(u32 offset, u32 size);
        virtual bool            unmap();

        virtual EBufferTarget   getTarget() const;

        //BufferPtr               clone() const override;

    protected:

        Buffer();

        friend                  RenderThread;
        virtual bool            create(u32 size = 0, const void* data = nullptr);
        virtual void            destroy();

    private:

        Buffer* const           m_impl;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d
