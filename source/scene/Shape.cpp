#include "Shape.h"
#include "Engine.h"
#include "utils/Logger.h"
#include "renderer/Geometry.h"


using namespace v3d;
using namespace scene;
using namespace renderer;

CShape::CShape()
#ifdef _DEBUG
    : m_debug(nullptr)
#endif
{
    m_nodeType = ENodeType::eShape;
    LOG_INFO("Create node type: %s", getNodeNameByType(m_nodeType).c_str());

    CRendereble::setMaterial(std::make_shared<CMaterial>());
}

void CShape::init()
{
    const RenderTechniquePtr& technique = CRendereble::getMaterial()->getRenderTechique();
    if (!technique)
    {
        LOG_ERROR("CShape: Do not exist RenderTechique");
        ASSERT(false && "CShape: Do not exist RenderTechique");
        return;
    }

    CRendereble::setGeometry(RENDERER->makeSharedGeometry(technique));
    CRendereble::setRenderJob(std::make_shared<CRenderJob>(CRendereble::getMaterial(), CRendereble::getGeometry(), CNode::getAbsTransform()));
#ifdef _DEBUG
    m_debug = RENDERER->makeDebugDraw(CRendereble::getGeometry());
    m_debug->setDebugFlag(EDebugGeometryFlag::eGeometryFlagNone);
#endif
}

CShape::~CShape()
{
    CRendereble::getGeometry()->free();
#ifdef _DEBUG
    m_debug->free();
#endif
}

EShapeType CShape::getShapeType() const
{
    return m_shapeType;
}

SVertexData& CShape::getGeometryData()
{
    ASSERT(CRendereble::getGeometry() && "CShape: Geomery data nullptr");
    return CRendereble::getGeometry()->getData();
}

CGeometry::EDrawMode CShape::getGeometryDrawMode() const
{
    ASSERT(CRendereble::getGeometry() && "CShape: Geomery data nullptr");
    return CRendereble::getGeometry()->getDrawMode();
}

void CShape::setGeometryDrawMode(CGeometry::EDrawMode mode)
{
    ASSERT(CRendereble::getGeometry() && "CShape: Geomery data nullptr");
    CRendereble::getGeometry()->setDrawMode(mode);
}

void CShape::update(s32 time)
{
    if (!CNode::isVisible() || !m_initialiazed)
    {
        return;
    }

    CNode::updateTransform();
    CRendereble::getRenderJob()->setTransform(CNode::getAbsTransform());
}

void CShape::render()
{
    if (!CNode::isVisible() || !m_initialiazed)
    {
        return;
    }

    RENDERER->draw(CRendereble::getRenderJob());

#ifdef _DEBUG
    if (RENDERER->isDebugMode())
    {
        m_debug->draw();
    }
#endif
}
