#include "Geometry.h"

using namespace v3d;
using namespace v3d::renderer;

CGeometry::CGeometry(const RenderTechiquePtr& techique)
	: m_drawMode(EDrawMode::eTriangleStrip)
    , m_geometyType(EGeomertyType::eGeomertyStatic)
    , m_techique(techique)
{
}

CGeometry::~CGeometry()
{
}

SVertexData& CGeometry::getData()
{
    return m_data;
}

EDrawMode CGeometry::getDrawMode() const
{
    return m_drawMode;
}

void CGeometry::setDrawMode(EDrawMode mode)
{
    m_drawMode = mode;
}