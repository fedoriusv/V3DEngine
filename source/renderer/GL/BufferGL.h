#ifndef _V3D_BUFFER_GL_H_
#define _V3D_BUFFER_GL_H_

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

        void        bind()                                                              override;
        void        bindToBuffer(const Buffer* buffer, u32 offset = 0, u32 size = 0)    override;

        void        unbind()                                                            override;

        void        setData(EDataUsageType type, u32 size, void* data)                  override;
        void        updateData(u32 offset, u32 size, void* data)                        override;

        void*       map(u32 access)                                                     override;
        bool        unmap()                                                             override;

    private:

        u32         m_id;
        bool        m_lock;

    private:

        static u32  s_currentBuffer[EBufferTarget::eBufferTargetCount];
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace gl
} //namespace renderer
} //namespace v3d

#endif //_V3D_BUFFER_GL_H_
