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
    , m_modelMatrix(core::Matrix4D())
    , m_normalMatrix(core::Matrix4D())
#ifdef _DEBUG
    , m_debugMode(false)
#endif
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

void CRenderer::reshape(u32 width, u32 height)
{
    if (height == 0)
    {
        height = 1;
    }
    m_viewportSize.width = width;
    m_viewportSize.height = height;

    f32 aspectRatio = (f32)m_viewportSize.width / (f32)m_viewportSize.height;
    m_projectionMatrix = core::buildProjectionMatrixPerspective(45.0f, aspectRatio, 0.5f, 100.0f);

}

void CRenderer::updateCamera(const core::Vector3D& pos, const core::Vector3D& target, const core::Vector3D& up)
{
    m_viewMatrix = core::buildLookAtMatrix(pos, target, up);
    m_viewMatrix.makeTransposed();
    m_viewPosition = pos;

}

void CRenderer::updateTransform(const core::Matrix4D& transform)
{
    m_modelMatrix = transform;

    m_modelMatrix.getInverse(m_normalMatrix);
    m_modelMatrix.makeTransposed();

}

const core::Dimension2D& CRenderer::getViewportSize() const
{
    return m_viewportSize;
}

#ifdef _DEBUG
void CRenderer::setDebugMode(bool active)
{
    m_debugMode = active;
}

bool CRenderer::isDebugMode() const
{
    return m_debugMode;
}
#endif
