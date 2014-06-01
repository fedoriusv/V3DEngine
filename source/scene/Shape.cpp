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
{
    m_material = std::make_shared<CMaterial>();
}

void CShape::init()
{
    RenderTechiquePtr techique = m_material->getRenderTechique();
    if (!techique)
    {
        LOG_ERROR("CShape: Do not exist RenderTechique");
        ASSERT(false && "CShape: Do not exist RenderTechique");
        return;
    }

    m_geometry = v3d::CEngine::getInstance()->getRenderer()->makeSharedGeometry(techique);
}

CShape::~CShape()
{
}

EShapeType CShape::getShapeType() const
{
	return m_shapeType;
}

void CShape::setMaterial(renderer::MaterialPtr material)
{
	m_material = material;
}

renderer::MaterialPtr CShape::getMaterial() const
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