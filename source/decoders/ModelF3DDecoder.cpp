#include "ModelF3DDecoder.h"
#include "stream/StreamManager.h"
#include "utils/Logger.h"
#include "scene/Model.h"
#include "scene/Mesh.h"

using namespace v3d;
using namespace decoders;
using namespace scene;
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

stream::CResource* CModelF3DDecoder::decode(const IStreamPtr& stream)
{
    if (!stream)
    {
        LOG_ERROR("CModelF3DDecoder::decode: Empty stream");
        return nullptr;
    }

    if (stream->size() > 0)
    {
        stream->seekBeg(0);

        MemoryStreamPtr data = CStreamManager::createMemoryStream(nullptr, stream->size());
        stream->read(data->getData(), sizeof(u8), stream->size());

        s32 version;
        data->read(version);

        if (version == F3D_MODEL_LOADER_VERSION)
        {
            return CModelF3DDecoder::decode100(data);
        }
        else
        {
            LOG_ERROR("CModelF3DDecoder::decode: unknown save version %d", version);
            return nullptr;
        }
    }

    LOG_ERROR("CModelF3DDecoder::decode: Empty stream size");
    return nullptr;
}

stream::CResource* CModelF3DDecoder::decode100(const stream::IStreamPtr& stream)
{
    CModel* model = new CModel();
    s32 id;
    stream->read(id);

    std::string name;
    stream->read(name);

    IStreamPtr data = CStreamManager::createMemoryStream();
    data->seekBeg(0);

    data->write(id);
    data->write(name);

    model->init(data);

    u32 countNodes;
    stream->read(countNodes);

    std::vector<s32> parentList;
    for (u32 nodeIdx = 0; nodeIdx < countNodes; ++nodeIdx)
    {
        s32 nodetype;
        stream->read(nodetype);

        s32 parrentIdx;
        stream->read(parrentIdx);
        parentList.push_back(parrentIdx);

        u32 subStreamSize;
        stream->read(subStreamSize);

        MemoryStreamPtr subStream = stream::CStreamManager::createMemoryStream(nullptr, subStreamSize);
        stream->read(subStream->getData(), sizeof(u8), subStreamSize);

        CNode* node = nullptr;
        switch (nodetype)
        {
            case eMesh:
            {
                node = new CMesh();
                static_cast<CMesh*>(node)->init(subStream);
            }
                break;

            case eLight:
            case eCamera:
            default:
                break;
        }

        model->addNode(node);
    }

    for (u32 nodeIdx = 0; nodeIdx < countNodes; ++nodeIdx)
    {
        CNode* node = model->getNode(nodeIdx);
        
        if (parentList[nodeIdx] >= 0)
        {
            node->setParent(model->getNode(parentList[nodeIdx]));
        }
        else
        {
            node->setParent(model);
        }
    }

    return model;
}
