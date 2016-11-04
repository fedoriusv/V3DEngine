#pragma once

#include "common.h"

namespace v3d
{
namespace renderer
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    class Buffer;

    /**
    * ConstantBuffer.
    * Client thread
    */
    class ConstantBuffer
    {
    public:

        explicit ConstantBuffer(u32 size);
        ConstantBuffer(const ConstantBuffer&) = delete;
        ConstantBuffer& operator=(const ConstantBuffer&) = delete;
        ~ConstantBuffer();

        void*   map(u32 size, u32 offset); //thread unsafe
        void    unmap(); //thread unsafe

        void    update(u32 size, u32 offset, const void* data);
        void    read(u32 size, u32 offset, void* const data);

        void    flush();

    private:

        bool        m_mapped;

        std::mutex  m_mutex;
        Buffer*     m_buffer;

        u8*         m_data;
        u32         m_size;
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d
