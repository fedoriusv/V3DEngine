#include "Model.h"
#include "utils/Logger.h"
#include "ModelManager.h"
#include "scene/RenderTechniqueManager.h"
#include "Engine.h"

using namespace v3d;
using namespace v3d::scene;
using namespace v3d::renderer;

CModel::CModel(const std::string& file, const std::string& techique)
: m_data(nullptr)
, m_file(file)
, m_techique(techique)
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

void CModel::render()
{
    if (!m_visible || !m_initialiazed)
    {
        return;
    }

    //m_renderJob->setTransform(CNode::getAbsTransform());

    for (std::vector<MeshPtr>::const_iterator mesh = m_meshes.begin(); mesh < m_meshes.end(); ++mesh)
    {
        (*mesh)->render();
    }
}

void CModel::update(f64 time)
{
    if (!m_visible || !m_initialiazed)
    {
        return;
    }

    for (std::vector<MeshPtr>::const_iterator mesh = m_meshes.begin(); mesh < m_meshes.end(); ++mesh)
    {
        (*mesh)->update(time);
    }
}

void CModel::init()
{
    if (m_file.empty())
    {
        LOG_ERROR("CModel::init: Empty file name");
        return;
    }

    RenderTechniquePtr technique = scene::CRenderTechniqueManager::getInstance()->load(m_techique);
    if (!technique)
    {
        LOG_ERROR("CModel::init: Error read file [%s]", m_techique.c_str());
        return;
    }

    m_data = CModelManager::getInstance()->load(m_file);
    if (!m_data)
    {
        LOG_ERROR("CModel::init: Empty model data");
        return;
    }

    m_id = m_data->getID();
    m_name = m_data->getName();

    for (u32 i = 0; i < m_data->getCountMeshes(); ++i)
    {
        stream::ResourcePtr resourceMesh = m_data->readMeshResource(technique);
        if (!resourceMesh)
        {
            LOG_WARNING("CModel::init: Streaming mesh N [%d] error read file [%s]", i, m_data->getResourseName().c_str());
            continue;
        }

        if (!resourceMesh->load())
        {
            LOG_WARNING("CModelManager::load: Streaming mesh N [%d] error load [%s]", i, m_data->getResourseName().c_str());
            continue;
        }

        MeshPtr mesh = std::static_pointer_cast<CMesh>(resourceMesh);
        mesh->init();

        CModel::addMesh(mesh);
    }

    m_initialiazed = true;
}
