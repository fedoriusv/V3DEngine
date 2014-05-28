#include "Shape.h"
#include "Engine.h"
#include "renderer/GL/GeomertyGL.h"

using namespace f3d;
using namespace f3d::scene;
using namespace f3d::renderer;

CShape::CShape()
    : m_geometry(nullptr)
    , m_material(nullptr)
{
}

void CShape::init()
{
	platform::EDriverType type = f3d::CEngine::getInstance()->getPlatform()->getDriverType();
	
    m_material = std::make_shared<CMaterial>();

	switch (type)
	{
		case platform::EDriverType::eDriverOpenGL:
		{
            //m_material->getRenderTechique()->getRenderPass(0)->
            //m_geometry = std::make_shared<CGeometryGL>(CGeometryGL());
		}
		break;

		case platform::EDriverType::eDriverDirect3D:
		{
			//m_geometry = std::make_shared<CGeometry>(CGeometryGL());
		}
		break;

		default:
		{
			LOG_ERROR("Can`t Choose driver type");
		}
	}
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
	return m_geometry->m_data;
}

EDrawMode CShape::getGeometryDrawMode() const
{
	return m_geometry->m_drawMode;
}

void CShape::setGeometryDrawMode(EDrawMode mode)
{
	m_geometry->m_drawMode = mode;
}