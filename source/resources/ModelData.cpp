#include "ModelData.h"
#include "utils/Logger.h"
#include "renderer/Mesh.h"
#include "stream/StreamManager.h"

using namespace v3d;
using namespace v3d::resources;
using namespace v3d::renderer;

CModelData::CModelData()
    : m_countMeshes(0U)
    , m_name("")
    , m_id(-1)
{
}

CModelData::~CModelData()
{
}

void CModelData::init(const stream::IStreamPtr& stream)
{
    CResource::setStream(stream);
}

u32 CModelData::getCountMeshes() const
{
    return m_countMeshes;
}

const std::string& CModelData::getName() const
{
    return m_name;
}

s32 CModelData::getID() const
{
    return m_id;
}

bool CModelData::load()
{
    const stream::IStreamPtr& stream = CResource::getStream();
    if (!stream)
    {
        LOG_ERROR("CModelData::load: Empty Stream with name [%s]", CResource::getResourseName().c_str());
        return false;
    }

    if (stream->size() > 0)
    {

        stream->seekBeg(0);

        stream->read(m_id);
        stream->read(m_name);

        stream->read(m_countMeshes);

        return true;
    }

    return false;
}

stream::ResourcePtr CModelData::readMeshResource(const renderer::RenderTechniquePtr& technique) const
{
    const stream::IStreamPtr& stream = CResource::getStream();
    if (!stream)
    {
        LOG_ERROR("CModelData::getMeshResource: Empty Stream with name [%s]", CResource::getResourseName().c_str());
        return nullptr;
    }

    if (stream->size() > 0)
    {
        u32 size = 0;
        stream->read(size);
        u8* data = new u8[size];
        stream->read(data, sizeof(u8), size);

        MeshPtr mesh = std::make_shared<CMesh>(technique);

        stream::IStreamPtr mem = stream::CStreamManager::createMemoryStream(data, size);
        mesh->init(mem);

        return mesh;
    }

    return nullptr;
}