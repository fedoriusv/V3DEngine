#include "Model.h"
#include "utils/Logger.h"
#include "Engine.h"

using namespace v3d;
using namespace v3d::scene;
using namespace v3d::renderer;

CModel::CModel()
: m_data(nullptr)
{
    m_nodeType = ENodeType::eModel;
    LOG_INFO("Create node type: %s", getNodeNameByType(m_nodeType).c_str());
}

CModel::~CModel()
{
    m_meshes.clear();
}

void CModel::addMesh(const renderer::MeshPtr& mesh)
{
    if (mesh)
    {
        m_meshes.push_back(mesh);
    }
}

const renderer::MeshPtr& CModel::getMesh(u32 id) const
{
    ASSERT(m_meshes.size() > id || "Mesh list out range");
    return m_meshes.at(id);
}

u32 CModel::getMeshCount() const
{
    return m_meshes.size();
}

void CModel::setModelData(const resources::ModelDataPtr& data)
{
    m_data = data;
}

const resources::ModelDataPtr& CModel::getModelData() const
{
    return m_data;
}

void CModel::render()
{
    if (!m_visible || !m_initialiazed)
    {
        return;
    }

    //m_renderJob->setTransform(CNode::getAbsTransform());
}

void CModel::update(f64 time)
{
    if (!m_visible || !m_initialiazed)
    {
        return;
    }


}

void CModel::init()
{
    if (!m_data)
    {
        LOG_ERROR("CModel::init: Empty model data");
        return;
    }

    //TODO:
}
