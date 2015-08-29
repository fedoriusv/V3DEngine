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

    CRenderable::setMaterial(std::make_shared<CMaterial>());
    CRenderable::getMaterial()->setTexture(0, texture);
}

CBillboard::~CBillboard()
{
    CRenderable::getGeometry()->free();
}

void CBillboard::render()
{
    if (!CNode::isVisible() || !m_initialiazed)
    {
        return;
    }

    RENDERER->draw(CRenderable::getRenderJob());
}

void CBillboard::update(s32 dt)
{
    if (!CNode::isVisible() || !m_initialiazed)
    {
        return;
    }

    CNode::update(dt);
    CRenderable::getRenderJob()->setTransform(CNode::getAbsTransform());
}

void CBillboard::init()
{
    const RenderTechniquePtr& technique = CRenderable::getMaterial()->getRenderTechique();
    if (!technique)
    {
        LOG_ERROR("CBillboard: Do not exist RenderTechique");
        ASSERT(false && "CBillboard: Do not exist RenderTechique");
        return;
    }

    CRenderable::setGeometry(RENDERER->makeSharedGeometry(technique));
    CRenderable::setRenderJob(std::make_shared<CRenderJob>(CRenderable::getMaterial(), CRenderable::getGeometry(), CNode::getAbsTransform()));


    CBillboard::build();
    CRenderable::getGeometry()->setDrawMode(CGeometry::ePoints);

    CRenderable::getGeometry()->init();
    m_initialiazed = true;
}

void CBillboard::build()
{
    SVertexData& data = CRenderable::getGeometry()->getData();
    data._vertices =
    {
        {0.0f, 0.0f, 0.0f}
    };
}