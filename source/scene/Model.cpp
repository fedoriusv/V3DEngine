#include "Model.h"
#include "Mesh.h"
#include "utils/Logger.h"
#include "scene/ModelManager.h"

using namespace v3d;
using namespace scene;
using namespace resources;

CModel::CModel(const std::string& file)
: m_file(file)
{
    m_nodeType = ENodeType::eModel;
    LOG_INFO("CModel: Create node type: %s, name: %s", getNodeNameByType(m_nodeType).c_str(), file.c_str());
}

CModel::~CModel()
{
    //TODO: maybe need delete ptr
    m_nodesList.clear();
}

void CModel::init()
{
    if (m_initialiazed)
    {
        return;
    }

    if (m_file.empty())
    {
        LOG_ERROR("CModel::init: Empty file name");
        return;
    }

    const ModelDataPtr data = CModelManager::getInstance()->load(m_file);
    if (!data)
    {
        LOG_ERROR("CModel::init: Empty model data");
        return;
    }

    const std::vector<CModelData::SNodeData>& nodesDataList = data->getNodesList();
    for (auto& nodeData : nodesDataList)
    {
        CNode* node = nullptr;
        switch (nodeData._type)
        {
            case eMesh:
            {
                node = new CMesh();
                static_cast<CMesh*>(node)->init(nodeData._stream);
            }
                break;

            case eLight:
            case eCamera:
            default:
                break;
        }

        if (node)
        {
            m_nodesList.push_back(node);
        }
    }

    //TODO: apply parents

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
