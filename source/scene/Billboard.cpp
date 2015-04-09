#include "Billboard.h"
#include "Engine.h"
#include "utils/Logger.h"

using namespace v3d;
using namespace scene;
using namespace renderer;

CBillboard::CBillboard(const std::string& texture)
: m_material(nullptr)
, m_geometry(nullptr)
, m_renderJob(nullptr)
{
    m_nodeType = ENodeType::eBillboard;
    LOG_INFO("Create node type: %s", getNodeNameByType(m_nodeType).c_str());

    m_material = std::make_shared<CMaterial>();
    m_material->setTexture(0, texture);
}

CBillboard::~CBillboard()
{
    m_geometry->free();
}

void CBillboard::setMaterial(const MaterialPtr& material)
{
    m_material = material;
}

const MaterialPtr& CBillboard::getMaterial() const
{
    return m_material;
}

const RenderJobPtr& CBillboard::getRenderJob() const
{
    return m_renderJob;
}

void CBillboard::render()
{
    if (!m_visible || !m_initialiazed)
    {
        return;
    }

    RENDERER->draw(m_renderJob);
}

void CBillboard::update(s32 time)
{
    if (!m_visible || !m_initialiazed)
    {
        return;
    }

    m_renderJob->setTransform(CNode::getAbsTransform());
}

void CBillboard::init()
{
    const RenderTechniquePtr& technique = m_material->getRenderTechique();
    if (!technique)
    {
        LOG_ERROR("CBillboard: Do not exist RenderTechique");
        ASSERT(false && "CBillboard: Do not exist RenderTechique");
        return;
    }

    m_geometry = RENDERER->makeSharedGeometry(technique);
    m_renderJob = std::make_shared<CRenderJob>(m_material, m_geometry, m_modelMatrix);

    CBillboard::build();

    m_initialiazed = true;
}

void CBillboard::build()
{
    m_geometry->setDrawMode(CGeometry::ePoints);

    const f32 vertex[][3] =
    {
        0.0f, 0.0f, 0.0f,
    };

    SVertexData& data = m_geometry->getData();
    data.malloc(1, 0);
    m_geometry->copyToVertices(vertex, 1);

    m_geometry->init();
}