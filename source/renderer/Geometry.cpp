#include "Geometry.h"

using namespace f3d;
using namespace f3d::renderer;

CGeometry::CGeometry(const AttributeList& attributes)
	: m_drawMode(EDrawMode::eTriangleStrip)
    , m_attributes(attributes)
{
}

CGeometry::~CGeometry()
{
}