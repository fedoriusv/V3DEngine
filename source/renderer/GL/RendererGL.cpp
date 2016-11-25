#include "RendererGL.h"
#include "utils/Logger.h"
#include "context/DeviceContext.h"

#ifdef _OPENGL_RENDER_
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

RendererGL::RendererGL(const ContextPtr context)
    : IRenderer(context, false)
    , m_isLocked(false)
{
    m_defaultRenderTarget = makeSharedRenderTarget();
    RendererGL::setCurrentRenderTarget(m_defaultRenderTarget);
}

RendererGL::~RendererGL()
{
}

//void RendererGL::init()
//{
//    LOG_INFO("OpenGL Render Init");
//
//    glViewport(0,0, m_context->getWindowSize().width, m_context->getWindowSize().height);
//    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//
//    m_context->setVSync(false);
//
//    glEnable(GL_MULTISAMPLE);
//
//    CRenderStateGL::winding(eWindingCCW);
//    CRenderStateGL::culling(true);
//    CRenderStateGL::cullface(eFaceBack);
//
//    CRenderStateGL::depthWrite(true);
//    CRenderStateGL::depthFunc(eCmpLequal);
//    CRenderStateGL::depthTest(true);
//    glClearDepth(1.0);
//
//    //glEnable(GL_DEPTH_CLAMP);
//    //glDepthRange(-1.0, 1.0);
//
//    CRenderStateGL::blendFunc(eBlendSrcAlpha, eBlendInvSrcAlpha);
//
//    ASSERT(m_context->getTextureUnitsCount() > 0, "Texture units not supported");
//    CTextureGL::bindTextureUnit(0);
//
//    glDisable(GL_DITHER);
//
//#ifdef _DEBUG
//    m_context->checkForErrors("CRendererGL: Render Init");
//#endif
//}

void RendererGL::preRender(bool clear)
{
    if (isLocked())
    {
        return;
    }

    if (clear)
    {
        const core::Vector4D& color = IRenderer::getCurrentRenderTarget()->getColorValue();
        glClearColor(color[0], color[1], color[2], color[3]);

        f32 depth = m_currentRenderTarget->getDepthValue();
        glClearDepthf(depth);

        /*glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);*/
    }

    m_isLocked = true;
}

void RendererGL::postRender()
{
    if (!isLocked())
    {
        return;
    }

    m_frameIndex++;
    m_isLocked = false;

#ifdef _DEBUG
    m_context.lock()->checkForErrors();
#endif
    //m_context.lock()->flush();
}


platform::ERenderType RendererGL::getRenderType() const
{
    return platform::ERenderType::eRenderOpenGL;
}

void RendererGL::immediateInit()
{
}

void RendererGL::immediaterBeginFrame()
{
    if (RendererGL::isLocked())
    {
        return;
    }

    m_isLocked = true;
}

void RendererGL::immediateEndFrame()
{
    if (!RendererGL::isLocked())
    {
        return;
    }

    m_isLocked = false;
}

void RendererGL::immediatePresentFrame()
{
    m_frameIndex++;
    m_context.lock()->present();
}

void RendererGL::immediateDraw()
{
    //TODO: need mode, range, type, data, instansingCount
   /* bool indexed = false;
    u32 instansingCount = 0;
    if (indexed)
    {
        if (instansingCount > 0)
        {
            glDrawElementsInstanced();
        }
        else
        {
            glDrawElements();
        }
    }
    else
    {
        if (instansingCount > 0)
        {
            glDrawArraysInstanced();
        }
        else
        {
            glDrawArrays();
        }
    }*/
}

ShaderPtr RendererGL::makeSharedShader()
{
    return std::make_shared<CShaderGL>();
}

ShaderProgramPtr RendererGL::makeSharedProgram()
{
    return std::make_shared<CShaderProgramGL>();
}

GeometryPtr RendererGL::makeSharedGeometry(const RenderTechnique* technique)
{
    return std::make_shared<CGeometryGL>(technique);
}

RenderStatePtr RendererGL::makeSharedRenderState()
{
    return std::make_shared<CRenderStateGL>();
}

RenderTargetPtr RendererGL::makeSharedRenderTarget()
{
    return std::make_shared<CRenderTargetGL>();
}

GeometryTargetPtr RendererGL::makeSharedGeometryTarget()
{
    return std::make_shared<CGeometryTargetGL>();
}

bool RendererGL::isLocked() const
{
    return m_isLocked;
}

void RendererGL::resetTextures()
{
    CTextureGL::reset();
}

} //namespace gl
} //namespace renderer
} //namespace v3d

#endif //_OPENGL_RENDER_
