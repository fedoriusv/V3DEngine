#include "Shape.h"
#include "Engine.h"
#include "utils/Logger.h"
#include "renderer/Geometry.h"
#include "renderer/Material.h"


using namespace v3d;
using namespace scene;
using namespace renderer;

CShape::CShape()
{
    m_nodeType = ENodeType::eShape;
    LOG_INFO("Create node type: %s", getNodeNameByType(m_nodeType).c_str());

    CRenderable::setMaterial(new CMaterial());
}

void CShape::init()
{
    const CRenderTechnique* technique = CRenderable::getMaterial()->getRenderTechique();
    if (!technique)
    {
        LOG_ERROR("CShape: RenderTechique doesn't exist for shape [%s]", CNode::getName().c_str());
        ASSERT(false, "CShape: RenderTechique doesn't exist");
        return;
    }

    CRenderable::setGeometry(RENDERER->makeSharedGeometry(technique));
    CRenderable::setRenderJob(std::make_shared<CRenderJob>(CRenderable::getMaterial(), CRenderable::getGeometry(), CNode::getAbsTransform()));
}

CShape::~CShape()
{
    LOG_INFO("Delete node type: %s", getNodeNameByType(m_nodeType).c_str());
    CRenderable::getGeometry()->free();
}

EShapeType CShape::getShapeType() const
{
    return m_shapeType;
}

SVertexData& CShape::getGeometryData()
{
    ASSERT(CRenderable::getGeometry(), "CShape: Geomery data nullptr");
    return CRenderable::getGeometry()->getData();
}

EDrawMode CShape::getGeometryDrawMode() const
{
    ASSERT(CRenderable::getGeometry(), "CShape: Geomery data nullptr");
    return CRenderable::getGeometry()->getDrawMode();
}

void CShape::setGeometryDrawMode(EDrawMode mode)
{
    ASSERT(CRenderable::getGeometry(), "CShape: Geomery data nullptr");
    CRenderable::getGeometry()->setDrawMode(mode);
}

void CShape::update(s32 dt)
{
    if (!CNode::isVisible() || !m_initialiazed)
    {
        return;
    }

    CNode::update(dt);
    CRenderable::getRenderJob()->setTransform(CNode::getAbsTransform());
}

void CShape::render()
{
    if (!CNode::isVisible() || !m_initialiazed)
    {
        return;
    }

    CRenderable::render();
}
