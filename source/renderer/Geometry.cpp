#include "Geometry.h"

using namespace v3d;
using namespace v3d::renderer;

CGeometry::CGeometry(const AttributeList& attributes)
	: m_drawMode(EDrawMode::eTriangleStrip)
    , m_attributes(attributes)
{
}

CGeometry::~CGeometry()
{
}