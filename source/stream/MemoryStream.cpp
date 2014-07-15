#include "MemoryStream.h"

using namespace v3d;
using namespace v3d::stream;

MempryStream::MempryStream(const void* data, const u32 size)
    : m_stream((c8*)data)
    , m_length(0)
    , m_allocated(size)
    , m_pos(0)
{
}

MempryStream::~MempryStream()
{
    if (m_allocated)
    {
        delete[] m_stream;
        m_stream = nullptr;
    }
}

void MempryStream::close()
{
}

u32 MempryStream::read(void* buffer, const u32 size, const u32 count)
{
    ASSERT(m_pos + size * count <= m_length);

    memcpy(buffer, m_stream + m_pos, size * count);
    m_pos += size * count;

    return m_pos;
}

u32 MempryStream::read(u8& value)
{
    ASSERT(m_length - m_pos >= sizeof(u8));

    value = m_stream[m_pos++];

    return m_pos;
}

u32 MempryStream::read(s8& value)
{
    ASSERT(m_length - m_pos >= sizeof(s8));

    value = m_stream[m_pos++];

    return m_pos;
}

u32 MempryStream::read(u16& value)
{
    ASSERT(m_length - m_pos >= sizeof(u16));

    value = (m_stream[m_pos++] & 0xFF) << 8;
    value |= m_stream[m_pos++] & 0xFF;

    return m_pos;
}

u32 MempryStream::read(s16& value)
{
    ASSERT(m_length - m_pos >= sizeof(s16));

    value = (m_stream[m_pos++] & 0xFF) << 8;
    value |= m_stream[m_pos++] & 0xFF;

    return m_pos;
}

u32 MempryStream::read(u32& value)
{
    ASSERT(m_length - m_pos >= sizeof(u32));

    value = (m_stream[m_pos++] & 0xFF) << 24;
    value |= (m_stream[m_pos++] & 0xFF) << 16;
    value |= (m_stream[m_pos++] & 0xFF) << 8;
    value |= (m_stream[m_pos++] & 0xFF);

    return m_pos;
}

u32 MempryStream::read(s32& value)
{
    ASSERT(m_length - m_pos >= sizeof(u32));

    value = (m_stream[m_pos++] & 0xFF) << 24;
    value |= (m_stream[m_pos++] & 0xFF) << 16;
    value |= (m_stream[m_pos++] & 0xFF) << 8;
    value |= (m_stream[m_pos++] & 0xFF);

    return m_pos;
}

u32 MempryStream::read(u64& value)
{
    ASSERT(m_length - m_pos >= sizeof(u64));

    value = (m_stream[m_pos++] & 0xFF) << 24;
    value |= (m_stream[m_pos++] & 0xFF) << 16;
    value |= (m_stream[m_pos++] & 0xFF) << 8;
    value |= (m_stream[m_pos++] & 0xFF);

    return m_pos;
}

u32 MempryStream::read(s64& value)
{
    ASSERT(m_length - m_pos >= sizeof(s64));

    value = (m_stream[m_pos++] & 0xFF) << 24;
    value |= (m_stream[m_pos++] & 0xFF) << 16;
    value |= (m_stream[m_pos++] & 0xFF) << 8;
    value |= (m_stream[m_pos++] & 0xFF);

    return m_pos;
}

u32 MempryStream::read(f32& value)
{
    ASSERT(m_length - m_pos >= sizeof(f32));

    s32& ival = *((s32*)&value);

    ival = (m_stream[m_pos++] & 0xFF) << 24;
    ival |= (m_stream[m_pos++] & 0xFF) << 16;
    ival |= (m_stream[m_pos++] & 0xFF) << 8;
    ival |= (m_stream[m_pos++] & 0xFF);

    return m_pos;
}

u32 MempryStream::read(f64& value)
{
    ASSERT(m_length - m_pos >= sizeof(f64));

    s32& ival = *((s32*)&value);

    ival = (m_stream[m_pos++] & 0xFF) << 24;
    ival |= (m_stream[m_pos++] & 0xFF) << 16;
    ival |= (m_stream[m_pos++] & 0xFF) << 8;
    ival |= (m_stream[m_pos++] & 0xFF);

    return m_pos;
}

u32 MempryStream::read(f80& value)
{
    ASSERT(m_length - m_pos >= sizeof(f80));

    s32& ival = *((s32*)&value);

    ival = (m_stream[m_pos++] & 0xFF) << 24;
    ival |= (m_stream[m_pos++] & 0xFF) << 16;
    ival |= (m_stream[m_pos++] & 0xFF) << 8;
    ival |= (m_stream[m_pos++] & 0xFF);

    return m_pos;
}

u32 MempryStream::read(bool& value)
{
    ASSERT(m_length - m_pos >= sizeof(bool));
    value = m_stream[m_pos++] != 0;

    return m_pos;
}

u32 MempryStream::read(std::string& value)
{
   //TODO:

    return m_pos;
}

u32 MempryStream::write(void* buffer, const u32 size, const u32 count)
{
    if (checkSize(size))
    {
        memcpy(m_stream + m_pos, buffer, size * count);
        m_pos += size * count;

        if (m_pos > m_length)
        {
            m_length = m_pos;
        }
    }

    return m_pos;
}

u32 MempryStream::write(const u8 value)
{
    if (checkSize(sizeof(u8)))
    {
        m_stream[m_pos++] = value;

        if (m_pos > m_length)
        {
            m_length = m_pos;
        }
    }

    return m_pos;
}

u32 MempryStream::write(const s8 value)
{
    if (checkSize(sizeof(s8)))
    {
        m_stream[m_pos++] = value;

        if (m_pos > m_length)
        {
            m_length = m_pos;
        }
    }

    return m_pos;
}

u32 MempryStream::write(const u16 value)
{
    if (checkSize(sizeof(u16)))
    {
        m_stream[m_pos++] = (value >> 8) & 0xFF;
        m_stream[m_pos++] = value & 0xFF;

        if (m_pos > m_length)
        {
            m_length = m_pos;
        }
    }

    return m_pos;
}

u32 MempryStream::write(const s16 value)
{
    if (checkSize(sizeof(s16)))
    {
        m_stream[m_pos++] = (value >> 8) & 0xFF;
        m_stream[m_pos++] = value & 0xFF;

        if (m_pos > m_length)
        {
            m_length = m_pos;
        }
    }

    return m_pos;
}

u32 MempryStream::write(const u32 value)
{
    if (checkSize(sizeof(u32)))
    {
        m_stream[m_pos++] = (value >> 24) & 0xFF;
        m_stream[m_pos++] = (value >> 16) & 0xFF;
        m_stream[m_pos++] = (value >> 8) & 0xFF;
        m_stream[m_pos++] = value & 0xFF;

        if (m_pos > m_length)
        {
            m_length = m_pos;
        }
    }

    return m_pos;
}

u32 MempryStream::write(const s32 value)
{
    if (checkSize(sizeof(s32)))
    {
        m_stream[m_pos++] = (value >> 24) & 0xFF;
        m_stream[m_pos++] = (value >> 16) & 0xFF;
        m_stream[m_pos++] = (value >> 8) & 0xFF;
        m_stream[m_pos++] = value & 0xFF;

        if (m_pos > m_length)
        {
            m_length = m_pos;
        }
    }

    return m_pos;
}

u32 MempryStream::write(const u64 value)
{
    if (checkSize(sizeof(u64)))
    {
        m_stream[m_pos++] = (value >> 24) & 0xFF;
        m_stream[m_pos++] = (value >> 16) & 0xFF;
        m_stream[m_pos++] = (value >> 8) & 0xFF;
        m_stream[m_pos++] = value & 0xFF;

        if (m_pos > m_length)
        {
            m_length = m_pos;
        }
    }

    return m_pos;
}

u32 MempryStream::write(const s64 value)
{
    if (checkSize(sizeof(s64)))
    {
        m_stream[m_pos++] = (value >> 24) & 0xFF;
        m_stream[m_pos++] = (value >> 16) & 0xFF;
        m_stream[m_pos++] = (value >> 8) & 0xFF;
        m_stream[m_pos++] = value & 0xFF;

        if (m_pos > m_length)
        {
            m_length = m_pos;
        }
    }

    return m_pos;
}

u32 MempryStream::write(const f32 value)
{
    if (checkSize(sizeof(f32)))
    {
        int& ival = *((int*)&value);

        m_stream[m_pos++] = (ival >> 24) & 0xFF;
        m_stream[m_pos++] = (ival >> 16) & 0xFF;
        m_stream[m_pos++] = (ival >> 8) & 0xFF;
        m_stream[m_pos++] = ival & 0xFF;

        if (m_pos > m_length)
        {
            m_length = m_pos;
        }
    }

    return m_pos;
}

u32 MempryStream::write(const f64 value)
{
    if (checkSize(sizeof(f64)))
    {
        int& ival = *((int*)&value);

        m_stream[m_pos++] = (ival >> 24) & 0xFF;
        m_stream[m_pos++] = (ival >> 16) & 0xFF;
        m_stream[m_pos++] = (ival >> 8) & 0xFF;
        m_stream[m_pos++] = ival & 0xFF;

        if (m_pos > m_length)
        {
            m_length = m_pos;
        }
    }

    return m_pos;
}

u32 MempryStream::write(const f80 value)
{
    if (checkSize(sizeof(f80)))
    {
        int& ival = *((int*)&value);

        m_stream[m_pos++] = (ival >> 24) & 0xFF;
        m_stream[m_pos++] = (ival >> 16) & 0xFF;
        m_stream[m_pos++] = (ival >> 8) & 0xFF;
        m_stream[m_pos++] = ival & 0xFF;

        if (m_pos > m_length)
        {
            m_length = m_pos;
        }
    }

    return m_pos;
}

u32 MempryStream::write(const bool value)
{
    if (checkSize(sizeof(bool)))
    {
        m_stream[m_pos++] = value;

        if (m_pos > m_length)
        {
            m_length = m_pos;
        }
    }

    return m_pos;
}

u32 MempryStream::write(const std::string& value)
{
    //TODO:

    return m_pos;
}

void MempryStream::seekBeg(const u32 offset)
{
    ASSERT(offset <= m_length);
    m_pos = offset;
}

void MempryStream::seekEnd(const u32 offset)
{
    ASSERT(offset <= m_length);
    m_pos = m_length + offset;
}

void MempryStream::seekCur(const u32 offset)
{
    ASSERT(offset <= m_length);
    m_pos += offset;
}

u32 MempryStream::tell()
{
    return m_pos;
}

u32 MempryStream::size()
{
    return m_length;
}

void* MempryStream::map(const u32 size)
{
    //TODO: 

    return m_stream;
}

void MempryStream::unmap()
{
    //TODO
}

void MempryStream::clear()
{
    MempryStream::seekBeg(0);
    m_length = 0;
}

void MempryStream::allocate(u32 size)
{
    if (m_stream)
    {
        delete[] m_stream;
        m_stream = nullptr;
    }

    m_allocated = size;
    m_stream = new char[m_allocated];

    MempryStream::seekBeg(0);
}

bool MempryStream::checkSize(int size)
{
    ASSERT(m_stream);
    if (m_allocated == 0)
    {
        return false;
    }

    if (m_pos + size > m_allocated)
    {
        c8* oldStream = m_stream;

        int newAllocated = 2 * (m_pos + size);
        m_stream = new c8[newAllocated];
        memcpy(m_stream, oldStream, m_allocated);

        m_allocated = newAllocated;

        delete[] oldStream;
        oldStream = nullptr;
    }

    return true;
}