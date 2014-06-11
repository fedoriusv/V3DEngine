#ifndef F3D_STREAM_H_
#define F3D_STREAM_H_

#include "common.h"

namespace v3d
{
namespace stream
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

	class IStream
	{
	public:
		IStream(){};
		virtual         ~IStream(){};

		template <class T>
		u32             read(T& value);

		virtual u32     read(void* buffer, const u32 size, const u32 count = 1) = 0;
        virtual void    close()                                                 = 0;

		virtual u32     read(u8& value)                 = 0;
		virtual u32     read(s8& value)                 = 0;
		virtual u32     read(u16& value)                = 0;
		virtual u32     read(s16& value)                = 0;
		virtual u32     read(u32& value)                = 0;
		virtual u32     read(s32& value)                = 0;
		virtual u32     read(u64& value)                = 0;
		virtual u32     read(s64& value)                = 0;
		virtual u32     read(f32& value)                = 0;
		virtual u32     read(f64& value)                = 0;
		virtual u32     read(f80& value)                = 0;
		virtual u32     read(bool& value)               = 0;
		virtual u32     read(std::string& value)        = 0;

		template<class T>
		u32             write(T& value);

		virtual u32     write(void* buffer, const u32 size, const u32 count = 1) = 0;

		virtual u32     write(const u8 value)           = 0;
		virtual u32     write(const s8 value)           = 0;
		virtual u32     write(const u16 value)          = 0;
		virtual u32     write(const s16 value)          = 0;
		virtual u32     write(const u32 value)          = 0;
		virtual u32     write(const s32 value)          = 0;
		virtual u32     write(const u64 value)          = 0;
		virtual u32     write(const s64 value)          = 0;
		virtual u32     write(const f32 value)          = 0;
		virtual u32     write(const f64 value)          = 0;
		virtual u32     write(const f80 value)          = 0;
		virtual u32     write(const bool value)         = 0;
		virtual u32     write(const std::string& value) = 0;

		virtual void    seekBeg(const u32 offset)       = 0;
        virtual void    seekEnd(const u32 offset)       = 0;
		virtual void    seekCur(const u32 offset)       = 0;

		virtual u32     tell()                          = 0;
		virtual u32     size()                          = 0;

		virtual void*   map(const u32 size)             = 0;
		virtual void    unmap()                         = 0;

		virtual std::string	getName() const             = 0;
	};

	template <class T>
	virtual u32 IStream::read(T& value)
	{
		const u32 ret = read(&read, sizeof(T), 1);
		return ret;
	}

	template <class T>
	virtual u32 IStream::write(T& value)
	{
		const u32 ret = write(&read, sizeof(T), 1);
		return ret;
	}

	template <class T>
	IStream& operator >> (IStream& stream, T& value)
	{
		stream.read(value);
		return stream;
	}

	template <class T>
	IStream& operator << (IStream& stream, const T& value)
	{
		stream.write(value);
		return stream;
	}

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //F3D_STREAM_H_
