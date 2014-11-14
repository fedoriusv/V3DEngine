#ifndef _V3D_MEMORY_STREAM_H_
#define _V3D_MEMORY_STREAM_H_

#include "IStream.h"

namespace v3d
{
namespace stream
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class MemoryStream : public IStream
    {
    public:

        MemoryStream();
        MemoryStream(const void* data, const u32 size);
        ~MemoryStream();

        void	clear();
        void	allocate(u32 size);

        void    close()                                                  override;

        u32     read(void* buffer, const u32 size, const u32 count = 1)  override;
        u32     read(u8& value)                                          override;
        u32     read(s8& value)                                          override;
        u32     read(u16& value)                                         override;
        u32     read(s16& value)                                         override;
        u32     read(u32& value)                                         override;
        u32     read(s32& value)                                         override;
        u32     read(u64& value)                                         override;
        u32     read(s64& value)                                         override;
        u32     read(f32& value)                                         override;
        u32     read(f64& value)                                         override;
        u32     read(f80& value)                                         override;
        u32     read(bool& value)                                        override;
        u32     read(std::string& value)                                 override;

        u32     write(void* buffer, const u32 size, const u32 count = 1) override;
        u32     write(const u8 value)                                    override;
        u32     write(const s8 value)                                    override;
        u32     write(const u16 value)                                   override;
        u32     write(const s16 value)                                   override;
        u32     write(const u32 value)                                   override;
        u32     write(const s32 value)                                   override;
        u32     write(const u64 value)                                   override;
        u32     write(const s64 value)                                   override;
        u32     write(const f32 value)                                   override;
        u32     write(const f64 value)                                   override;
        u32     write(const f80 value)                                   override;
        u32     write(const bool value)                                  override;
        u32     write(const std::string& value)                          override;

        void    seekBeg(const u32 offset)                                override;
        void    seekEnd(const u32 offset)                                override;
        void    seekCur(const u32 offset)                                override;
        u32     tell()                                                   override;
        u32     size()                                                   override;

        u8*     getData() const;

    private:

        bool    checkSize(u32 size);

        u8*     m_stream;
        u32     m_length;
        u32     m_allocated;
        u32     m_pos;
         
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<MemoryStream> MemoryStreamPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_MEMORY_STREAM_H_
