#include "StreamManager.h"
#include "FileStream.h"

using namespace f3d;
using namespace f3d::stream;

CStreamManager::CStreamManager()
{
}

CStreamManager::~CStreamManager()
{
}

IStream* CStreamManager::createFileStream(const std::string& file)
{
    return new FileStream(file);
}