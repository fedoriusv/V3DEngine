#include "ModelData.h"
#include "utils/Logger.h"

using namespace v3d;
using namespace v3d::resources;

CModelData::CModelData()
: m_countMeshes(0U)
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

stream::ResourcePtr CModelData::getMeshResource(u32 index) const
{
    return nullptr;
}