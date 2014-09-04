#include "StreamManager.h"
#include "FileStream.h"
#include "MemoryStream.h"

using namespace v3d;
using namespace v3d::stream;

CStreamManager::CStreamManager()
{
}

CStreamManager::~CStreamManager()
{
}

FileStream* CStreamManager::createFileStream(const std::string& file)
{
    return new FileStream(file);
}

MemoryStream* CStreamManager::createMemoryStream(const void* data, const u32 size)
{
    return new MemoryStream(data, size);
}