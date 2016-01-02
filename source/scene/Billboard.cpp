#include "Billboard.h"
#include "Engine.h"
#include "utils/Logger.h"
#include "renderer/Material.h"

using namespace v3d;
using namespace scene;
using namespace renderer;

CBillboard::CBillboard(const std::string& texture)
{
    m_nodeType = ENodeType::eBillboard;
    LOG_INFO("Create node type: %s", getNodeNameByType(m_nodeType).c_str());

    CRenderable::setMaterial(new CMaterial());
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

    CRenderable::render();
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
    if (m_initialiazed)
    {
        return;
    }

    const CRenderTechnique* technique = CRenderable::getMaterial()->getRenderTechique();
    if (!technique)
    {
        LOG_ERROR("CBillboard: RenderTechique doesn't exist");
        ASSERT(false, "CBillboard: RenderTechique doesn't exist");
        return;
    }

    CRenderable::setGeometry(RENDERER->makeSharedGeometry(technique));
    CRenderable::setRenderJob(std::make_shared<CRenderJob>(CRenderable::getMaterial(), CRenderable::getGeometry(), CNode::getAbsTransform()));


    CBillboard::build();
    CRenderable::getGeometry()->setDrawMode(ePoints);

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