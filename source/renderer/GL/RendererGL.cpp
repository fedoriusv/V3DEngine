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
#include "GeometryTargetGL.h"

#include "GL/glew.h"

namespace v3d
{
namespace renderer
{
namespace gl
{

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

    CRenderStateGL::winding(eWindingCCW);
    CRenderStateGL::culling(true);
    CRenderStateGL::cullface(eFaceBack);

    CRenderStateGL::depthWrite(true);
    CRenderStateGL::depthFunc(eCmpLequal);
    CRenderStateGL::depthTest(true);
    glClearDepth(1.0);

    //glEnable(GL_DEPTH_CLAMP);
    //glDepthRange(-1.0, 1.0);

    CRenderStateGL::blendFunc(eBlendSrcAlpha, eBlendInvSrcAlpha);

    ASSERT(m_context->getTextureUnitsCount() > 0, "Texture units not supported");
    CTextureGL::bindTextureUnit(0);

    glDisable(GL_DITHER);

#ifdef _DEBUG
    m_context->checkForErrors("CRendererGL: Render Init");
#endif
}

void CRendererGL::preRender(bool clear)
{
    if (isLocked())
    {
        return;
    }

    if (clear)
    {
        const core::Vector4D& color = CRenderer::getCurrentRenderTarget()->getColorValue();
        glClearColor(color[0], color[1], color[2], color[3]);

        f32 depth = m_currentRenderTarget->getDepthValue();
        glClearDepthf(depth);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

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

RenderStatePtr CRendererGL::makeSharedRenderState()
{
    return std::make_shared<CRenderStateGL>();
}

RenderTargetPtr CRendererGL::makeSharedRenderTarget()
{
    return std::make_shared<CRenderTargetGL>();
}

GeometryTargetPtr CRendererGL::makeSharedGeometryTarget()
{
    return std::make_shared<CGeometryTargetGL>();
}

bool CRendererGL::isLocked() const
{
    return m_isLocked;
}

void CRendererGL::resetTextures()
{
    CTextureGL::reset();
}

} //namespace gl
} //namespace renderer
} //namespace v3d

#endif //_OPENGL_DRIVER_
