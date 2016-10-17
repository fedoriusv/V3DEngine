#include "StreamManager.h"
#include "FileStream.h"
#include "MemoryStream.h"

namespace v3d
{
namespace stream
{

FileStreamPtr StreamManager::createFileStream(const std::string& file, FileStream::EOpenMode mode)
{
    return std::make_shared<FileStream>(file, mode);
}

MemoryStreamPtr StreamManager::createMemoryStream(const void* data, const u32 size)
{
    return std::make_shared<MemoryStream>(data, size);
}

} //namespace stream
} //namespace v3d
