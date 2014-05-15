#include "Geometry.h"

using namespace f3d;
using namespace f3d::renderer;

CGeometry::CGeometry()
	: m_drawMode(EDrawMode::eTriangleStrip)
{
}

CGeometry::~CGeometry()
{
}

SVertexData& CGeometry::getData()
{
	return m_data;
}