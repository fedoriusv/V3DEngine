#include "renderer/GL/RendererGL.h"
#include "utils/Logger.h"
#include "context/DriverContext.h"
#include "GL/glew.h"

using namespace f3d;
using namespace f3d::renderer;

CRendererGL::CRendererGL(CDriverContext* context)
	: CRenderer(context)
	, m_projectionMatrix(core::Matrix4D())
	, m_viewMatrix(core::Matrix4D())
{
}

CRendererGL::~CRendererGL()
{
}

void CRendererGL::init()
{
	LOG_INFO("OpenGL Render Init")

	reshape(m_viewportSize.width, m_viewportSize.height);
	glClearColor(m_backColor[0], m_backColor[1], m_backColor[2], 0.0f);

#ifdef _DEBUG
	m_context->checkForErrors();
#endif
}

void CRendererGL::preRender()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(m_backColor[0], m_backColor[1], m_backColor[2], 0.0f);
}

void CRendererGL::postRender()
{
#ifdef _DEBUG
	m_context->checkForErrors();
#endif
	glFlush();
}

void CRendererGL::reshape(u32 width, u32 height)
{
	if (height == 0)
	{
		height = 1;
	}
	m_viewportSize.width  = width;
	m_viewportSize.height = height;

	glViewport(0, 0, m_viewportSize.width, m_viewportSize.height);

	GLfloat aspectRatio = (GLfloat)m_viewportSize.width / (GLfloat)m_viewportSize.height;
	m_projectionMatrix = core::buildProjectionMatrixPerspective(45.0f, aspectRatio, 0.1f, 100.0f);
}