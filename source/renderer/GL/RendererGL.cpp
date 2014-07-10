#include "RendererGL.h"
#include "ShaderGL.h"
#include "ShaderProgramGL.h"
#include "GeomertyGL.h"
#include "TextureGL.h"
#include "RenderStateGL.h"
#include "DebugGeometryGL.h"
#include "DebugLightGL.h"

#include "GL/glew.h"

#include "utils/Logger.h"
#include "context/DriverContext.h"

using namespace v3d;
using namespace v3d::renderer;

CRendererGL::CRendererGL(const DriverContextPtr& context)
	: CRenderer(context)
{
	m_viewportSize = m_context->getViewport();
}

CRendererGL::~CRendererGL()
{
}

void CRendererGL::init()
{

    LOG_INFO("OpenGL Render Init");

	reshape(m_viewportSize.width, m_viewportSize.height);
	glClearColor(m_backColor[0], m_backColor[1], m_backColor[2], 0.0f);

    m_context->setVSync(false);

    glEnable(GL_MULTISAMPLE);

    //glDisable(GL_MULTISAMPLE);

    //glEnable(GL_DEPTH);
    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0f);
    //glDepthFunc(GL_LEQUAL);
    //glShadeModel(GL_SMOOTH);


    //glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ////


#ifdef _DEBUG
	m_context->checkForErrors("Render Init");
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
    CRenderer::reshape(width, height);

    glViewport(0, 0, width, height);
}

ShaderPtr CRendererGL::makeSharedShader()
{
    return std::make_shared<CShaderGL>();
}

ShaderProgramPtr CRendererGL::makeSharedProgram(const ShaderDataPtr& data)
{
    return std::make_shared<CShaderProgramGL>(data);
}

GeometryPtr CRendererGL::makeSharedGeometry(const RenderTechniquePtr& technique)
{
    return std::make_shared<CGeometryGL>(technique);
}

TexturePtr CRendererGL::makeSharedTexture()
{
    return std::make_shared<CTextureGL>();
}

RenderStatePtr CRendererGL::makeSharedRenderState()
{
    return std::make_shared<CRenderStateGL>();
}

#ifdef _DEBUG
DebugGeometryPtr CRendererGL::makeDebugDraw(const GeometryPtr& geometry)
{
    return std::make_shared<CDebugGeometryGL>(geometry);
}

DebugLightPtr CRendererGL::makeDebugLight(const Vector3D& position, const scene::SLightData& data)
{
    return std::make_shared<CDebugLightGL>(position, data);
}
#endif
