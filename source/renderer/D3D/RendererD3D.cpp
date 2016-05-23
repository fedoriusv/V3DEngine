#include "RendererD3D.h"
#include "utils/Logger.h"

#ifdef _DIRECT3D_DRIVER_
#include "context/DriverContextD3D.h"
#include "renderer/D3D/TextureD3D.h"
#include "GeometryD3D.h"

#include <d3d11.h>

namespace v3d
{
namespace renderer
{
namespace d3d
{

CRendererD3D::CRendererD3D(const DriverContextPtr& context)
    : CRenderer(context)
    , m_isLocked(false)
{
    m_defaultRenderTarget = makeSharedRenderTarget();
    CRenderer::setCurrentRenderTarget(m_defaultRenderTarget);
}

CRendererD3D::~CRendererD3D()
{
}

void CRendererD3D::init()
{
    LOG_INFO("Direct3D Render Init");

    // Setup the viewport for rendering.
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)m_context->getWindowSize().width;
    vp.Height = (FLOAT)m_context->getWindowSize().height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0.0f;
    vp.TopLeftY = 0.0f;

    ID3D11DeviceContext* d3dContext = std::static_pointer_cast<CDriverContextD3D>(m_context)->getD3DContext();
    d3dContext->RSSetViewports(1, &vp);

    ID3D11RenderTargetView* d3dTargetView = std::static_pointer_cast<CDriverContextD3D>(m_context)->getD3DTargetView();
    FLOAT clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    d3dContext->ClearRenderTargetView(d3dTargetView, clearColor);

    m_context->setVSync(false);

    /*CRenderStateGL::winding(eWindingCW);
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

    CTextureGL::activeTextureLayer(0);

    glDisable(GL_DITHER);*/

#ifdef _DEBUG
    m_context->checkForErrors("CRendererD3D: Render Init");
#endif
}

void CRendererD3D::preRender(bool clear)
{
    if (isLocked())
    {
        return;
    }

    m_isLocked = true;
}

void CRendererD3D::postRender()
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

ShaderPtr CRendererD3D::makeSharedShader()
{
    return nullptr;
    //return std::make_shared<CShaderGL>();
}

ShaderProgramPtr CRendererD3D::makeSharedProgram()
{
    return nullptr;
    //return std::make_shared<CShaderProgramGL>();
}

GeometryPtr CRendererD3D::makeSharedGeometry(const CRenderTechnique* technique)
{
    return std::make_shared<CGeometryD3D>(technique);
}

RenderStatePtr CRendererD3D::makeSharedRenderState()
{
    return nullptr;
    //return std::make_shared<CRenderStateGL>();
}

RenderTargetPtr CRendererD3D::makeSharedRenderTarget()
{
    return nullptr;
    //return std::make_shared<CRenderTargetGL>();
}

GeometryTargetPtr CRendererD3D::makeSharedGeometryTarget()
{
    return nullptr;
}

bool CRendererD3D::isLocked() const
{
    return m_isLocked;
}

void CRendererD3D::resetTextures()
{
    //CTextureGL::reset();
}

} //namespace d3d
} //namespace renderer
} //namespace v3d

#endif //_DIRECT3D_DRIVER_
