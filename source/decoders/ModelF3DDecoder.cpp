#include "ModelF3DDecoder.h"
#include "stream/StreamManager.h"
#include "utils/Logger.h"
#include "scene/Model.h"
#include "scene/Mesh.h"
#include "renderer/Material.h"

using namespace v3d;
using namespace decoders;
using namespace scene;
using namespace stream;
using namespace renderer;

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

        f32 version;
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
    std::map<std::string, MemoryStreamPtr> materialList;

    u32 countMaterials = 0;
    stream->read(countMaterials);
    for (u32 index = 0; index < countMaterials; ++index)
    {
        u32 materialStreamSize;
        stream->read(materialStreamSize);

        MemoryStreamPtr materialStream = stream::CStreamManager::createMemoryStream(nullptr, materialStreamSize);
        stream->read(materialStream->getData(), sizeof(u8), materialStreamSize);
        
        materialStream->seekBeg(0);
        std::string materialName;
        materialStream->read(materialName);

        if (!materialName.empty())
        {
            materialList.insert(std::map<std::string, MemoryStreamPtr>::value_type(materialName, materialStream));
        }
    }

    //Geometry
    std::vector<s32> parentList;

    u32 countNodes;
    stream->read(countNodes);
    data->write(countNodes);
    for (u32 index = 0; index < countNodes; ++index)
    {
        s32 nodetype;
        stream->read(nodetype);

        s32 parrentIdx;
        stream->read(parrentIdx);
        parentList.push_back(parrentIdx);

        std::string materialName;
        stream->read(materialName);

        u32 nodeStreamSize;
        stream->read(nodeStreamSize);

        MemoryStreamPtr nodeStream = stream::CStreamManager::createMemoryStream(nullptr, nodeStreamSize);
        stream->read(nodeStream->getData(), sizeof(u8), nodeStreamSize);

        data->write(nodetype);

        switch (nodetype)
        {
            case eMesh:
            {
                data->write(nodeStream->size());
                data->write(nodeStream->getData(), sizeof(u8), nodeStream->size());

                data->write(materialName);
                if (!materialName.empty())
                {
                    MemoryStreamPtr& materialStream = materialList[materialName];
                    data->write(materialStream->size());
                    data->write(materialStream->getData(), sizeof(u8), materialStream->size());
                }
            }
                break;

            case eLight:
            case eCamera:
            default:
                break;
        }
    }

    //Parents
    data->write((u32)parentList.size());
    data->write(parentList.data(), sizeof(s32), (u32)parentList.size());

    parentList.clear();
    materialList.clear();

    model->init(data);

    return model;
}
