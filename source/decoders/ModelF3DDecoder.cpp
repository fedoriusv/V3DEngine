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

    if (stream->size() > 0)
    {
        stream->seekBeg(0);

        u32 version;
        stream->read(version);

        if (version <= F3D_MODEL_LOADER_VERSION)
        {
            return CModelF3DDecoder::decode100(stream);
        }
        else
        {
            LOG_ERROR("CModelF3DDecoder::decode: unknown svae version %d", version);
            return nullptr;
        }
    }

    LOG_ERROR("CModelF3DDecoder::decode: Empty stream size");
    return nullptr;
}

stream::ResourcePtr CModelF3DDecoder::decode100(const stream::IStreamPtr& stream)
{
    ModelDataPtr model = std::make_shared<CModelData>();

    stream::IStreamPtr mem = CStreamManager::createMemoryStream(data, size);
    model->init(mem);

    return model;
    stream->read(m_id);
    stream->read(m_name);

    stream->read(m_countNodes);
    for (u32 nodeIdx = 0; nodeIdx < m_countNodes; ++nodeIdx)
    {
        s32 nodetype;
        stream->read(nodetype);

        u32 subStreamSize;
        stream->read(subStreamSize);

        stream::IStreamPtr subStream = stream::CStreamManager::createMemoryStream(nullptr, subStreamSize);
        stream->read(subStream);

        SNodeData data(subStream, (scene::ENodeType)nodetype);
        m_nodesList.push_back(data);
    }

    return true;
}
