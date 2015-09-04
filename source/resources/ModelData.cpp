#include "ModelData.h"
#include "utils/Logger.h"
#include "stream/StreamManager.h"

using namespace v3d;
using namespace core;
using namespace resources;
using namespace renderer;

CModelData::CModelData()
    : m_countNodes(0U)
    , m_id(-1)
    , m_name("")
{
}

CModelData::~CModelData()
{
}

void CModelData::init(const stream::IStreamPtr& stream)
{
    CResource::setStream(stream);
}

u32 CModelData::getCountNodes() const
{
    return m_countNodes;
}

const std::string& CModelData::getName() const
{
    return m_name;
}

s32 CModelData::getID() const
{
    return m_id;
}

const std::vector<CModelData::SNodeData>& CModelData::getNodesList() const
{
    return m_nodesList;
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

        u32 version;
        stream->read(version);
    }

    return false;
}

