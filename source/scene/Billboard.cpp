#include "Billboard.h"
#include "Engine.h"
#include "utils/Logger.h"

using namespace v3d;
using namespace scene;
using namespace renderer;

CBillboard::CBillboard(const std::string& texture)
{
    m_nodeType = ENodeType::eBillboard;
    LOG_INFO("Create node type: %s", getNodeNameByType(m_nodeType).c_str());

    CRendereble::setMaterial(std::make_shared<CMaterial>());
    CRendereble::getMaterial()->setTexture(0, texture);
}

CBillboard::~CBillboard()
{
    CRendereble::getGeometry()->free();
}

void CBillboard::render()
{
    if (!CNode::isVisible() || !m_initialiazed)
    {
        return;
    }

    RENDERER->draw(CRendereble::getRenderJob());
}

void CBillboard::update(s32 time)
{
    if (!CNode::isVisible() || !m_initialiazed)
    {
        return;
    }

    CNode::updateTransform();
    CRendereble::getRenderJob()->setTransform(CNode::getAbsTransform());
}

void CBillboard::init()
{
    const RenderTechniquePtr& technique = CRendereble::getMaterial()->getRenderTechique();
    if (!technique)
    {
        LOG_ERROR("CBillboard: Do not exist RenderTechique");
        ASSERT(false && "CBillboard: Do not exist RenderTechique");
        return;
    }

    CRendereble::setGeometry(RENDERER->makeSharedGeometry(technique));
    CRendereble::setRenderJob(std::make_shared<CRenderJob>(CRendereble::getMaterial(), CRendereble::getGeometry(), CNode::getAbsTransform()));

    CBillboard::build();

    m_initialiazed = true;
}

void CBillboard::build()
{
    CRendereble::getGeometry()->setDrawMode(CGeometry::ePoints);

    const f32 vertex[][3] =
    {
        0.0f, 0.0f, 0.0f,
    };

    SVertexData& data = CRendereble::getGeometry()->getData();
    data.malloc(1, 0);
    CRendereble::getGeometry()->copyToVertices(vertex, 1);

    CRendereble::getGeometry()->init();
}