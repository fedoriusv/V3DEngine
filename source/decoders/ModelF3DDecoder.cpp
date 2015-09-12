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

    //Material
    std::vector<MemoryStreamPtr> materialList;

    u32 countMaterials = 0;
    //stream->read(countMaterials);
    for (u32 index = 0; index < countMaterials; ++index)
    {
        //TODO:
    }

    //Geometry
    std::vector<s32> parentList;
    NodeList nodeList;

    u32 countNodes;
    stream->read(countNodes);
    for (u32 index = 0; index < countNodes; ++index)
    {
        s32 nodetype;
        stream->read(nodetype);

        s32 parrentIdx;
        stream->read(parrentIdx);
        parentList.push_back(parrentIdx);

        s32 materialIdx = 0;
        //stream->read(materialIdx);

        u32 subStreamMatSize = 0;
        if (materialIdx > 0)
        {
            subStreamMatSize = materialList[materialIdx]->size();
        }

        u32 subStreamGeoSize;
        stream->read(subStreamGeoSize);

        MemoryStreamPtr subStream = stream::CStreamManager::createMemoryStream(nullptr, subStreamGeoSize + subStreamMatSize);
        stream->read(subStream->getData(), sizeof(u8), subStreamGeoSize);

        if (subStreamMatSize > 0)
        {
            subStream->seekCur(subStreamGeoSize + 1);
            subStream->write(materialList[materialIdx]->getData(), sizeof(u8), subStreamMatSize);
        }

        s32 nodeIdx;
        if (subStream->size() > 0)
        {
            subStream->seekBeg(0);
            subStream->read(nodeIdx);
        }

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

        node->setID(nodeIdx);
        nodeList.push_back(node);
    }

    data->write((u32)nodeList.size());

    //Parents
    u32 index = 0;
    for (NodeList::iterator iter = nodeList.begin(); iter < nodeList.end(); ++iter)
    {
        CNode* node = (*iter);
        data->write(&node, sizeof(CNode*), 1);
        s32 parentIdx = parentList[index];
        if (parentIdx >= 0)
        {
            auto predHaveParent = [parentIdx](const CNode* node) -> bool
            {
                return parentIdx == node->getID();
            };

            auto parentIter = std::find_if(nodeList.begin(), nodeList.end(), predHaveParent);
            node->setParent(parentIter != nodeList.end() ? (*parentIter) : model);
        }
        else
        {
            node->setParent(model);
        }
        ++index;
    }

    parentList.clear();
    nodeList.clear();
    materialList.clear();

    model->init(data);

    return model;
}
