#include "ModelF3DDecoder.h"
#include "resources/ModelData.h"
#include "utils/Logger.h"

using namespace v3d;
using namespace v3d::decoders;
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
    if (!stream)
    {
        LOG_ERROR("CModelF3DDecoder::decode: Empty stream");
        return nullptr;
    }

    ModelDataPtr data = std::make_shared<CModelData>();

    data->init(stream);

    return data;
}
