#include "ConstantBuffer.h"
#include "Buffer.h"

namespace v3d
{
namespace renderer
{

ConstantBuffer::ConstantBuffer(u32 size)
    : m_mapped(false)
    , m_buffer(nullptr)
    , m_data(nullptr)
    , m_size(size)
{
    ASSERT(size > 0, "invalid size");
    m_data = reinterpret_cast<u8*>(malloc(size));
    memset(m_data, 0, m_size);

    m_buffer = new Buffer(eUniformsBuffer, eWriteDynamic, m_size, m_data);
}

ConstantBuffer::~ConstantBuffer()
{
    free(m_data);
    m_data = nullptr;

    delete m_buffer;
}

void* ConstantBuffer::map(u32 size, u32 offset)
{
    if (!m_mapped)
    {
        m_mapped = true;

        ASSERT(size + offset <= m_size, "invalid offset");
        u8* ptr = m_data + offset;
        return ptr;
    }

    return nullptr;
}

void ConstantBuffer::unmap()
{
    ASSERT(!m_mapped, "already unmapped");
    if (m_mapped)
    {
        m_mapped = false;
    }
}

void ConstantBuffer::update(u32 size, u32 offset, const void* data)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    void* dstData = ConstantBuffer::map(size, offset);
    ASSERT(dstData, "dstData is nullptr");
    if (dstData)
    {
        memcpy(dstData, data, size);
        ConstantBuffer::unmap();
    }
}

void ConstantBuffer::read(u32 size, u32 offset, void* const data)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    void* scrData = ConstantBuffer::map(size, offset);
    ASSERT(scrData, "dstData is nullptr");
    if (scrData)
    {
        memcpy(data, scrData, size);
        ConstantBuffer::unmap();
    }
}

void ConstantBuffer::flush()
{
    std::lock_guard<std::mutex> lock(m_mutex);

    if (m_mapped)
    {
        ASSERT(false, "data mapped");
    }

    m_buffer->update(0, m_size, m_data);
}

} //namespace renderer
} //namespace v3d
