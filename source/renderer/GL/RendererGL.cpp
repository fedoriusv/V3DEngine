#include "RendererGL.h"
#include "ShaderGL.h"
#include "ShaderProgramGL.h"
#include "GeometryGL.h"
#include "TextureGL.h"
#include "RenderStateGL.h"
#include "RenderTargetGL.h"
#include "DebugGeometryGL.h"
#include "DebugLightGL.h"

#include "GL/glew.h"

#include "utils/Logger.h"
#include "context/DriverContext.h"

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

    reshape(m_context->getWindowSize().width, m_context->getWindowSize().height);
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

    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &m_maxTextureUnits);
    ASSERT(m_maxTextureUnits > 0 && "Texture units not supported");

    glActiveTexture(GL_TEXTURE0);

    GLfloat m_maxAnisotropy;
    if (glewIsSupported("GL_EXT_texture_filter_anisotropic"))
    {
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &m_maxAnisotropy);
    }

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

ShaderProgramPtr CRendererGL::makeSharedProgram()
{
    return std::make_shared<CShaderProgramGL>();
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

RenderTargetPtr CRendererGL::makeSharedRenderTarget()
{
    return std::make_shared<CRenderTargetGL>();
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

bool CRendererGL::isLocked() const
{
    return m_isLocked;
}

void CRendererGL::resetTexture()
{
    CTextureGL::reset();
}
