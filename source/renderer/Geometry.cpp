#include "Geometry.h"

using namespace f3d;
using namespace f3d::renderer;

CGeometry::CGeometry(const ShaderDataPtr& shaderData)
	: m_drawMode(EDrawMode::eTriangleStrip)
    , m_shaderData(shaderData)
{
}

CGeometry::~CGeometry()
{
}