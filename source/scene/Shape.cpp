#include "Shape.h"
#include "Fedoria3D.h"
#include "renderer/GL/GeomertyGL.h"

using namespace f3d;
using namespace f3d::scene;
using namespace f3d::renderer;

CShape::CShape()
	: m_geometry(nullptr)
	, m_material(nullptr)
{
	platform::EDriverType type = f3d::CEngine::getInstance()->getPlatform()->getDriverType();
	switch (type)
	{
		case platform::EDriverType::eDriverOpenGL:
		{
			m_geometry = std::shared_ptr<CGeometry>(CGeometryGL())
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