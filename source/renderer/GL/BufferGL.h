#pragma once

#ifdef _OPENGL_RENDER_
#include "renderer/Buffer.h"

namespace v3d
{
namespace renderer
{
namespace gl
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * Module for buffer objects management. GL render only.
    */
    class BufferGL : public Buffer
    {
    public:

        explicit    BufferGL(EBufferTarget target);
                    ~BufferGL();

        void        bind() const                                                            override;
        void        bindToTarget(EBufferTarget target, u32 offset = 0, u32 size = 0) const  override;

        void        unbind() const                                                          override;

        void        set(EDataUsageType type, u32 size, const void* data)                    override;
        void        update(u32 offset, u32 size, const void* data)                          override;
        void        read(u32 offset, u32 size, void* data) const                            override;

        void*       map(u32 access)                                                         override;
        bool        unmap()                                                                 override;

        u32         getID() const;

    private:

        u32         m_bufferID;
        bool        m_lock;

    private:

        static u32  s_currentBuffer[EBufferTarget::eBufferTargetCount];
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace gl
} //namespace renderer
} //namespace v3d

#endif //_OPENGL_RENDER_
