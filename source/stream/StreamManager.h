#pragma once

#include "FileStream.h"
#include "MemoryStream.h"

namespace v3d
{
namespace stream
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class StreamManager
    {
    public:

        static FileStreamPtr    createFileStream(const std::string& file, FileStream::EOpenMode mode = FileStream::e_in);
        static MemoryStreamPtr  createMemoryStream(const void* data = nullptr, const u32 size = 0);

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace stream
} //namespace v3d
