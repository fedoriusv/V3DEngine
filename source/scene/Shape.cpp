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

    Renderable::setMaterial(new CMaterial());
}

void CShape::init()
{
    const CRenderTechnique* technique = Renderable::getRenderTechique();
    if (!technique)
    {
        LOG_ERROR("CShape: RenderTechique doesn't exist for shape [%s]", CNode::getName().c_str());
        ASSERT(false, "CShape: RenderTechique doesn't exist");
        return;
    }

    Renderable::setGeometry(RENDERER->makeSharedGeometry(technique));
    Renderable::setRenderJob(std::make_shared<CRenderJob>(this, CNode::getAbsTransform()));
}

CShape::~CShape()
{
    LOG_INFO("Delete node type: %s", getNodeNameByType(m_nodeType).c_str());
    Renderable::getGeometry()->free();
}

EShapeType CShape::getShapeType() const
{
    return m_shapeType;
}

SVertexData& CShape::getGeometryData()
{
    ASSERT(Renderable::getGeometry(), "CShape: Geomery data nullptr");
    return Renderable::getGeometry()->getData();
}

EPrimitivesMode CShape::getGeometryDrawMode() const
{
    ASSERT(Renderable::getGeometry(), "CShape: Geomery data nullptr");
    return Renderable::getGeometry()->getDrawMode();
}

void CShape::setGeometryDrawMode(EPrimitivesMode mode)
{
    ASSERT(Renderable::getGeometry(), "CShape: Geomery data nullptr");
    Renderable::getGeometry()->setDrawMode(mode);
}

void CShape::update(s32 dt)
{
    if (!CNode::isVisible() || !m_initialiazed)
    {
        return;
    }

    CNode::update(dt);
    Renderable::getRenderJob()->setTransform(CNode::getAbsTransform());
}

void CShape::render()
{
    if (!CNode::isVisible() || !m_initialiazed)
    {
        return;
    }

    Renderable::render();
}
