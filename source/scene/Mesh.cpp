#include "Mesh.h"
#include "utils/Logger.h"
#include "scene/ModelManager.h"
#include "Engine.h"

using namespace v3d;
using namespace scene;
using namespace renderer;

CMesh::CMesh(const std::string& file)
: m_file(file)
{
    m_nodeType = ENodeType::eMesh;
    LOG_INFO("CMesh: Create node type: %s, name: %s", getNodeNameByType(m_nodeType).c_str(), file.c_str());
}

CMesh::~CMesh()
{
}

void CMesh::render()
{
    if (!CNode::isVisible() || !m_initialiazed)
    {
        return;
    }

    RENDERER->draw(CRenderable::getRenderJob());
}

void CMesh::update(s32 dt)
{
    if (!CNode::isVisible() || !m_initialiazed)
    {
        return;
    }

    CNode::update(dt);
    CRenderable::getRenderJob()->setTransform(CNode::getAbsTransform());
}

void CMesh::init()
{
    if (m_file.empty())
    {
        LOG_ERROR("CMesh::init: Empty file name");
        return;
    }

    const RenderTechniquePtr& technique = CRenderable::getMaterial()->getRenderTechique();
    if (!technique)
    {
        LOG_ERROR("CMesh: Do not exist RenderTechique");
        ASSERT(false && "CMesh: Do not exist RenderTechique");
        return;
    }

    const resources::ModelDataPtr data = CModelManager::getInstance()->load(m_file);
    if (!data)
    {
        LOG_ERROR("CMesh::init: Empty model data");
        return;
    }

    CNode::setID(data->getID());
    CNode::setName(data->getName());

    for (u32 i = 0; i < data->getCountMeshes(); ++i)
    {
        stream::ResourcePtr resourceMesh = data->readMeshResource(technique);
        if (!resourceMesh)
        {
            LOG_WARNING("CMesh::init: Streaming mesh N [%d] error read file [%s]", i, data->getResourseName().c_str());
            continue;
        }

        if (!resourceMesh->load())
        {
            LOG_WARNING("CMesh::load: Streaming mesh N [%d] error load [%s]", i, data->getResourseName().c_str());
            continue;
        }

        //MeshPtr mesh = std::static_pointer_cast<CMesh>(resourceMesh);
        //mesh->init();

        //CModel::addMesh(mesh);
    }

    m_initialiazed = true;
}
