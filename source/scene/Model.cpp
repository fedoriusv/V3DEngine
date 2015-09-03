#include "Model.h"
#include "Mesh.h"
#include "utils/Logger.h"
#include "scene/ModelManager.h"

using namespace v3d;
using namespace scene;

CModel::CModel()
{
    m_nodeType = ENodeType::eModel;
    LOG_INFO("CModel: Create node type: %s", getNodeNameByType(m_nodeType).c_str());
}

CModel::~CModel()
{
    for (auto& it : m_nodesList)
    {
        delete it;
        it = nullptr;
    }
    m_nodesList.clear();
}

void CModel::addNode(CNode* node)
{
    if (node)
    {
        m_nodesList.push_back(node);
    }
}

CNode* CModel::getNode(u32 index)
{
    ASSERT(index < m_nodesList.size() && "CModel::getNode: invalid node index");
    return m_nodesList[index];
}

void CModel::init()
{
    if (m_initialiazed)
    {
        return;
    }

    for (std::vector<CNode*>::iterator node = m_nodesList.begin(); node < m_nodesList.end(); ++node)
    {
        (*node)->init();
    }

    m_initialiazed = true;
}

void CModel::update(s32 dt)
{
    if (!CNode::isVisible() || !m_initialiazed)
    {
        return;
    }

    CNode::update(dt);
}

void CModel::init(const stream::IStreamPtr& stream)
{
    CResource::setStream(stream);
}

bool CModel::load()
{
    const stream::IStreamPtr& stream = CResource::getStream();
    if (!stream)
    {
        LOG_ERROR("CModel: Empty Stream with name [%s] form File", CResource::getResourseName().c_str());
        return false;
    }

    if (stream->size() > 0)
    {
        stream->read(m_id);
        stream->read(m_name);
    }
}
