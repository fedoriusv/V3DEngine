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

    Renderable::setMaterial(new CMaterial());
    Renderable::getMaterial()->setTexture(0, texture);
}

CBillboard::~CBillboard()
{
    Renderable::getGeometry()->free();
}

void CBillboard::render()
{
    if (!CNode::isVisible() || !m_initialiazed)
    {
        return;
    }

    Renderable::render();
}

void CBillboard::update(s32 dt)
{
    if (!CNode::isVisible() || !m_initialiazed)
    {
        return;
    }

    CNode::update(dt);
    Renderable::getRenderJob()->setTransform(CNode::getAbsTransform());
}

void CBillboard::init()
{
    if (m_initialiazed)
    {
        return;
    }

    const RenderTechnique* technique = Renderable::getRenderTechique();
    if (!technique)
    {
        LOG_ERROR("CBillboard: RenderTechique doesn't exist");
        ASSERT(false, "CBillboard: RenderTechique doesn't exist");
        return;
    }

//    Renderable::setGeometry(ENGINE_RENDERER->makeSharedGeometry(technique));
    Renderable::setRenderJob(std::make_shared<CRenderJob>(this, CNode::getAbsTransform()));


    CBillboard::build();
    Renderable::getGeometry()->setDrawMode(ePoints);

    Renderable::getGeometry()->init();
    m_initialiazed = true;
}

void CBillboard::build()
{
    SVertexData& data = Renderable::getGeometry()->getData();
    data._vertices =
    {
        {0.0f, 0.0f, 0.0f}
    };
}