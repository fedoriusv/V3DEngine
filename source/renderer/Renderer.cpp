#include "renderer/Renderer.h"
#include "context/DriverContext.h"

using namespace f3d;
using namespace f3d::renderer;

CRenderer::CRenderer(CDriverContext* context)
	: m_context(context)
	, m_backColor(core::Vector3D(0))
{
	m_viewportSize = context->getWindow()->getSize();
}

CRenderer::~CRenderer()
{
}

void CRenderer::setBackColor(const core::Vector3D& color)
{
	m_backColor = color;
}

const core::Vector3D& CRenderer::getBackColor() const
{
	return m_backColor;
}