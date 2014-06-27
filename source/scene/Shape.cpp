#include "Shape.h"
#include "Engine.h"
#include "utils/Logger.h"
#include "renderer/GL/GeomertyGL.h"


using namespace v3d;
using namespace v3d::scene;
using namespace v3d::renderer;

CShape::CShape()
    : m_geometry(nullptr)
    , m_material(nullptr)
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
    RenderTechniquePtr technique = m_material->getRenderTechique();
    if (!technique)
    {
        LOG_ERROR("CShape: Do not exist RenderTechique");
        ASSERT(false && "CShape: Do not exist RenderTechique");
        return;
    }

    m_geometry = RENDERER->makeSharedGeometry(technique);
    m_renderJob = std::make_shared<CRenderJob>(m_material, m_geometry, m_transform);
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

void CShape::setMaterial(const renderer::MaterialPtr& material)
{
	m_material = material;
}

const renderer::MaterialPtr& CShape::getMaterial() const
{
	return m_material;
}

SVertexData& CShape::getGeometryData()
{
    return m_geometry->getData();
}

EDrawMode CShape::getGeometryDrawMode() const
{
	return m_geometry->getDrawMode();
}

void CShape::setGeometryDrawMode(EDrawMode mode)
{
	m_geometry->setDrawMode(mode);
}

void CShape::update(f64 time)
{
    if (!m_visible || !m_initialiazed)
    {
        return;
    }

    m_renderJob->setTransform(m_transform);
}

void CShape::render()
{
    if (!m_visible || !m_initialiazed)
    {
        return;
    }

    RENDERER->draw(m_renderJob, m_needUpdate);
    //m_needUpdate = false;

#ifdef _DEBUG
    if (RENDERER->isDebugMode())
    {
        m_debug->draw();
    }
#endif
}
