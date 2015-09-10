#include "Model.h"
#include "Mesh.h"
#include "utils/Logger.h"
#include "scene/ModelManager.h"
#include "scene/RenderTechniqueManager.h"

using namespace v3d;
using namespace scene;
using namespace renderer;

CModel::CModel()
{
    m_nodeType = ENodeType::eModel;
    LOG_INFO("CModel: Create node type: %s", getNodeNameByType(m_nodeType).c_str());
}

CModel::~CModel()
{
}

void CModel::init()
{
    if (m_initialiazed)
    {
        return;
    }

    for (NodeList::const_iterator iter = CNode::Begin(); iter < CNode::End(); ++iter)
    {
        CNode* node = (*iter);
        (*iter)->init();
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
        stream->seekBeg(0);

        stream->read(m_id);
        stream->read(m_name);

        return true;
    }

    return false;
}

bool CModel::setRenderTechniqueForAllMeshes(const std::string& file)
{
    const CRenderTechnique* technique = scene::CRenderTechniqueManager::getInstance()->load(file);
    if (!technique)
    {
        LOG_ERROR("CModel: Error read file [%s]", file.c_str());
        return false;
    }

    for (NodeList::const_iterator iter = CNode::Begin(); iter < CNode::End(); ++iter)
    {
        CNode* node = (*iter);
        switch (node->getNodeType())
        {
            case eMesh:
                static_cast<CMesh*>(node)->getMaterial()->setRenderTechnique(technique);
                break;

            default:
                break;
        }
    }

    return true;
}
