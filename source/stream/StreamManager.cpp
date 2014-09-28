#include "StreamManager.h"
#include "FileStream.h"
#include "MemoryStream.h"

using namespace v3d;
using namespace v3d::stream;

FileStreamPtr CStreamManager::createFileStream(const std::string& file, FileStream::EOpenMode mode)
{
    return std::make_shared<FileStream>(file, mode);
}

MemoryStreamPtr CStreamManager::createMemoryStream(const void* data, const u32 size)
{
    return std::make_shared<MemoryStream>(data, size);
}