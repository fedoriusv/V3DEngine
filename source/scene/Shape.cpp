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

    platform::EDriverType type = v3d::CEngine::getInstance()->getPlatform()->getDriverType();

    switch (type)
    {
        case platform::EDriverType::eDriverOpenGL:
        {
            m_geometry = std::make_shared<CGeometryGL>();
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

void CShape::init()
{
    ///TODO:!!!
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