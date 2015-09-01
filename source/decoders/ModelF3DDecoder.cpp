#include "ModelF3DDecoder.h"
#include "resources/ModelData.h"
#include "stream/StreamManager.h"
#include "utils/Logger.h"

using namespace v3d;
using namespace decoders;
using namespace resources;
using namespace stream;

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
    if (!stream)
    {
        LOG_ERROR("CModelF3DDecoder::decode: Empty stream");
        return nullptr;
    }

    u32 size = stream->size();
    u8* data = new u8[size];
    stream->read(data, sizeof(u8), size);

    ModelDataPtr model = std::make_shared<CModelData>();

    stream::IStreamPtr mem = CStreamManager::createMemoryStream(data, size);
    model->init(mem);

    return model;
}
