#include "ModelF3DDecoder.h"

using namespace v3d;
using namespace v3d::resources;

CModelF3DDecoder::CModelF3DDecoder()
    : CResourceDecoder()
{
}

CModelF3DDecoder::CModelF3DDecoder(std::initializer_list<std::string> supportedExtensions)
    : CResourceDecoder(supportedExtensions)
{
}

CModelF3DDecoder::~CModelF3DDecoder()
{
}

stream::ResourcePtr CModelF3DDecoder::decode(const stream::IStreamPtr& stream)
{
    return nullptr;
}
