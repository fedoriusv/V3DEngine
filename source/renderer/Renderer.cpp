#include "renderer/Renderer.h"
#include "context/DriverContext.h"

using namespace v3d;
using namespace v3d::renderer;

CRenderer::CRenderer(const DriverContextPtr& context)
	: m_context(context)
	, m_backColor(core::Vector3D(0))
    , m_projectionMatrix(core::Matrix4D())
    , m_viewMatrix(core::Matrix4D())
    , m_viewPosition(core::Vector3D(0.0f))
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

void CRenderer::checkForErrors(const std::string& location)
{
	m_context->checkForErrors(location);
}

void CRenderer::updateCamera(const core::Vector3D& pos, const core::Vector3D& target, const core::Vector3D& up)
{
    m_viewMatrix = core::buildLookAtMatrix(pos, target, up);
    m_viewMatrix.makeTransposed();
    m_viewPosition = pos;
}
