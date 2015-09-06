#include "RendererGL.h"
#include "utils/Logger.h"
#include "context/DriverContext.h"

#ifdef _OPENGL_DRIVER_
#include "ShaderGL.h"
#include "ShaderProgramGL.h"
#include "GeometryGL.h"
#include "TextureGL.h"
#include "RenderStateGL.h"
#include "RenderTargetGL.h"

#include "GL/glew.h"

using namespace v3d;
using namespace renderer;

CRendererGL::CRendererGL(const DriverContextPtr& context)
    : CRenderer(context)
    , m_isLocked(false)
{
    m_defaultRenderTarget = makeSharedRenderTarget();
    CRenderer::setCurrentRenderTarget(m_defaultRenderTarget);
}

CRendererGL::~CRendererGL()
{
}

void CRendererGL::init()
{
    LOG_INFO("OpenGL Render Init");

    glViewport(0,0, m_context->getWindowSize().width, m_context->getWindowSize().height);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    m_context->setVSync(false);

    glEnable(GL_MULTISAMPLE);

    CRenderStateGL::winding(eWindingCW);
    CRenderStateGL::culling(true);
    CRenderStateGL::cullface(eFaceBack);

    CRenderStateGL::depthWrite(true);
    CRenderStateGL::depthFunc(eCmpLequal);
    CRenderStateGL::depthTest(true);
    glClearDepth(1.0);

    glEnable(GL_DEPTH_CLAMP);
    glDepthRange(-1.0, 1.0);

    CRenderStateGL::blendFunc(eBlendSrcAlpha, eBlendInvSrcAlpha);

    ASSERT(m_context->getTextureUnits() > 0 && "Texture units not supported");
    CTextureGL::activeTextureLayer(0);

    glDisable(GL_DITHER);

#ifdef _DEBUG
    m_context->checkForErrors("CRendererGL: Render Init");
#endif
}

void CRendererGL::preRender()
{
    if (isLocked())
    {
        return;
    }

    /*glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);*/

    m_isLocked = true;
}

void CRendererGL::postRender()
{
    if (!isLocked())
    {
        return;
    }

    m_frameIndex++;
    m_isLocked = false;

#ifdef _DEBUG
    m_context->checkForErrors();
#endif
    m_context->flushBuffers();
}

ShaderPtr CRendererGL::makeSharedShader()
{
    return std::make_shared<CShaderGL>();
}

ShaderProgramPtr CRendererGL::makeSharedProgram()
{
    return std::make_shared<CShaderProgramGL>();
}

GeometryPtr CRendererGL::makeSharedGeometry(const CRenderTechnique* technique)
{
    return std::make_shared<CGeometryGL>(technique);
}

CTexture* CRendererGL::makeSharedTexture()
{
    return new CTextureGL();
}

RenderStatePtr CRendererGL::makeSharedRenderState()
{
    return std::make_shared<CRenderStateGL>();
}

RenderTargetPtr CRendererGL::makeSharedRenderTarget()
{
    return std::make_shared<CRenderTargetGL>();
}

bool CRendererGL::isLocked() const
{
    return m_isLocked;
}

void CRendererGL::resetTexture()
{
    CTextureGL::reset();
}

#endif //_OPENGL_DRIVER_
