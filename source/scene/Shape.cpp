#include "Shape.h"
#include "Engine.h"
#include "utils/Logger.h"
#include "renderer/Geometry.h"


using namespace v3d;
using namespace scene;
using namespace renderer;

CShape::CShape()
    : m_geometry(nullptr)
    , m_material(nullptr)
    , m_renderJob(nullptr)
#ifdef _DEBUG
    , m_debug(nullptr)
#endif
{
    m_nodeType = ENodeType::eShape;
    LOG_INFO("Create node type: %s", getNodeNameByType(m_nodeType).c_str());

    m_material = std::make_shared<CMaterial>();
}

void CShape::init()
{
    const RenderTechniquePtr& technique = m_material->getRenderTechique();
    if (!technique)
    {
        LOG_ERROR("CShape: Do not exist RenderTechique");
        ASSERT(false && "CShape: Do not exist RenderTechique");
        return;
    }

    m_geometry = RENDERER->makeSharedGeometry(technique);
    m_renderJob = std::make_shared<CRenderJob>(m_material, m_geometry, m_modelMatrix);
#ifdef _DEBUG
    m_debug = RENDERER->makeDebugDraw(m_geometry);
    m_debug->setDebugFlag(EDebugGeometryFlag::eGeometryFlagNone);
#endif
}

CShape::~CShape()
{
    m_geometry->free();
#ifdef _DEBUG
    m_debug->free();
#endif
}

EShapeType CShape::getShapeType() const
{
    return m_shapeType;
}

void CShape::setMaterial(const MaterialPtr& material)
{
    m_material = material;
}

const MaterialPtr& CShape::getMaterial() const
{
    return m_material;
}

const RenderJobPtr& CShape::getRenderJob() const
{
    return m_renderJob;
}

SVertexData& CShape::getGeometryData()
{
    return m_geometry->getData();
}

CGeometry::EDrawMode CShape::getGeometryDrawMode() const
{
    return m_geometry->getDrawMode();
}

void CShape::setGeometryDrawMode(CGeometry::EDrawMode mode)
{
    m_geometry->setDrawMode(mode);
}

void CShape::update(s32 time)
{
    if (!m_visible || !m_initialiazed)
    {
        return;
    }

    m_renderJob->setTransform(CNode::getAbsTransform());
}

void CShape::render()
{
    if (!m_visible || !m_initialiazed)
    {
        return;
    }

    RENDERER->draw(m_renderJob);
    //m_needUpdate = false;

#ifdef _DEBUG
    if (RENDERER->isDebugMode())
    {
        m_debug->draw();
    }
#endif
}
