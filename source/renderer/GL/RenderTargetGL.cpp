#include "RenderTargetGL.h"
#include "scene/TextureManager.h"
#include "Engine.h"
#include "utils/Logger.h"

#ifdef _OPENGL_DRIVER_
#include "RenderStateGL.h"
#include "RenderBufferGL.h"
#include "TextureGL.h"
#include "GL/glew.h"

#ifndef _PLATFORM_WIN_
#   pragma clang diagnostic ignored "-Wswitch"
#endif  //_PLATFORM_WIN_

namespace v3d
{
namespace renderer
{

using namespace scene;
using namespace core;
using namespace gl;

GLenum ERenderbufferTargetGL[] =
{
    GL_DRAW_FRAMEBUFFER,
    GL_READ_FRAMEBUFFER
};

u32 CRenderTargetGL::s_currentFBO[] = { 0 };

CRenderTargetGL::CRenderTargetGL()
    : m_frameBufferId(0U)

    , m_lastFrameIndex(1U)

    , m_hasClearColor(true)
    , m_hasClearDepth(true)
    , m_hasClearStencil(false)

    , m_initialized(false)
{
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint*)&m_frameBufferId);
    LOG_DEBUG("CRenderTargetGL: CRenderTargetGL constructor %x", this);
}

CRenderTargetGL::~CRenderTargetGL()
{
    CRenderTargetGL::destroy();
    m_attachBuffers.clear();

    ASSERT(m_frameBufferId == 0, "Framebuffer doesn't deleted");

    LOG_DEBUG("CRenderTargetGL: CRenderTargetGL destructor %x", this);
}

bool CRenderTargetGL::hasClearColorTarget() const
{
    return m_hasClearColor;
}

bool CRenderTargetGL::hasClearDepthTarget() const
{
    return m_hasClearDepth;
}

bool CRenderTargetGL::hasClearStencilTarget() const
{
    return m_hasClearStencil;
}

void CRenderTargetGL::bind()
{
    bool chaned = CRenderTargetGL::bindFramebuffer(m_frameBufferId);
    if (chaned)
    {
        RENDERER->setCurrentRenderTarget(shared_from_this());
        const Rect32& rect = getViewport();
        glViewport(0, 0, rect.getWidth(), rect.getHeight());

        if (!m_attachBuffers.empty())
        {
            glDrawBuffers((GLsizei)m_attachBuffers.size(), &m_attachBuffers[0]);
        }
        else
        {
            glDrawBuffer(m_frameBufferId ? GL_NONE : GL_BACK);
        }
    }

    u32 frameIdx = RENDERER->getFrameIndex();
    if (chaned || frameIdx != m_lastFrameIndex)
    {
        m_lastFrameIndex = frameIdx;

        u32 flags = 0;
        if (CRenderTargetGL::hasClearDepthTarget())
        {
            flags = CRenderTarget::getClearDepthBuffer() ? GL_DEPTH_BUFFER_BIT : 0;
            CRenderStateGL::depthWrite(true);

            glClearDepthf(CRenderTarget::getClearDepth());
        }
        
        if (CRenderTargetGL::hasClearStencilTarget())
        {
            flags = CRenderTarget::getClearStencilBuffer() ? GL_STENCIL_BUFFER_BIT : 0;
            CRenderStateGL::stencilWrite(true);

            //glClearStencil();
        }

        if (CRenderTargetGL::hasClearColorTarget() && CRenderTarget::getClearColorBuffer())
        {
            flags |= GL_COLOR_BUFFER_BIT;

            const core::Vector4D& color = CRenderTarget::getClearColor();
            glClearColor(color[0], color[1], color[2], color[3]);
        }

        if (flags)
        {
            glClear(flags);
        }
    }

    RENDERER->checkForErrors("CRenderTargetGL: Create render target Error");

}

void CRenderTargetGL::unbind()
{
    bool chaned = CRenderTargetGL::bindFramebuffer(0);
    if (chaned)
    {
        glReadBuffer(GL_BACK);
        glDrawBuffer(GL_BACK);

        const RenderTargetPtr& target = RENDERER->getDefaultRenderTarget();
        const Rect32& rect = target->getViewport();
        glViewport(0, 0, rect.getWidth(), rect.getHeight());

        RENDERER->setCurrentRenderTarget(target);
    }

    //TODO: test blit renderbuffers
    /*if (m_frameBufferID != 0 && m_attachBuffers.size() > 0)
    {
        CRenderTargetGL::copyToRenderbuffer(RENDERER->getDefaultRenderTarget());
    }*/
}

bool CRenderTargetGL::create()
{
    if (m_initialized)
    {
        return true;
    }

    const Rect32& rect = getViewport();
    u32 width = rect.getWidth();
    u32 height = rect.getHeight();
    u32 x = rect.getLeftX();
    u32 y = rect.getTopY();
    if (width == 0)
    {
        width = WINDOW->getSize().width;
    }

    if (height == 0)
    {
        height = WINDOW->getSize().height;
    }

    /*width = core::getSmallestPowerOf2(width);
    height = core::getSmallestPowerOf2(height);*/

    GLint originalFBO = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &originalFBO);

    CRenderTarget::setViewport(Rect32(x, y, x + width, y + height));

    glGenFramebuffers(1, &m_frameBufferId);
    CRenderTargetGL::bindFramebuffer(m_frameBufferId);

    for (auto& attach : m_attachmentsList)
    {
        switch (attach._output)
        {
            case eTextureOutput:
                CRenderTargetGL::createRenderToTexture(attach, rect);
                break;

            case eRenderOutput:
                CRenderTargetGL::createRenderToRenderbuffer(attach, rect);
                break;
        }
    }

    GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch (result)
    {
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        LOG_ERROR("CRenderTargetGL: Some attachments are incomplete");
        break;

    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        LOG_ERROR("CRenderTargetGL: Some attachments are missing");
        break;

    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        LOG_ERROR("CRenderTargetGL: Draw buffer is not set");
        break;

    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
        LOG_ERROR("CRenderTargetGL: Read buffer is not set");
        break;

    case GL_FRAMEBUFFER_UNSUPPORTED:
        LOG_ERROR("CRenderTargetGL: Framebuffer objects are not supported");
        break;

    case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
        LOG_ERROR("CRenderTargetGL:  Some attachments is not the same for all attached renderbuffers");
        break;

    case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
        LOG_ERROR("CRenderTargetGL:  Some framebuffer attachment is layered");
        break;
    };

    ASSERT((result == GL_FRAMEBUFFER_COMPLETE), "CRenderTarget: Create render target Error");
    RENDERER->checkForErrors("CRenderTargetGL: Create render target Error");

    if (originalFBO > 0)
    {
        CRenderTargetGL::bindFramebuffer(originalFBO);
    }

    m_initialized = true;

    return result == GL_FRAMEBUFFER_COMPLETE;
}

void CRenderTargetGL::destroy()
{
    if (!m_initialized)
    {
        return;
    }

    for (auto& attach : m_attachmentsList)
    {
        switch (attach._attachmentType)
        {
        case eColorAttach:
            if (attach._output == eTextureOutput)
            {
                framebufferTexture2D(GL_COLOR_ATTACHMENT0 + attach._index, GL_TEXTURE_2D, 0);
            }
            else if (attach._output == eRenderOutput)
            {
                framebufferRenderbuffer(GL_COLOR_ATTACHMENT0 + attach._index, 0);
            }
            break;

        case eDepthAttach:
            if (attach._output == eTextureOutput)
            {
                framebufferTexture2D(GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0);
            }
            else if (attach._output == eRenderOutput)
            {
                framebufferRenderbuffer(GL_DEPTH_ATTACHMENT, 0);
            }
            break;

        case eStencilAttach:
            if (attach._output == eTextureOutput)
            {
                framebufferTexture2D(GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, 0);
            }
            else if (attach._output == eRenderOutput)
            {
                framebufferRenderbuffer(GL_STENCIL_ATTACHMENT, 0);
            }
            break;
        }
    }
    //TODO: Have gl error GL_INVALID_OPERATION after call glFramebufferTexture2D

    if (m_frameBufferId > 0)
    {
#ifdef _DEBUG_GL
        ASSERT(glIsFramebuffer(m_frameBufferId), "Invalid Index FBO");
#endif //_DEBUG_GL
        glDeleteFramebuffers(1, &m_frameBufferId);
        m_frameBufferId = 0;
    }
}

void CRenderTargetGL::createRenderToRenderbuffer(SAttachments& attach, const Rect32& rect)
{
    std::function<GLenum(u32)> internalFormat = [](u32 format) -> GLenum
    {
        switch (format)
        {
        case 8888:
            return GL_RGBA8;
        case 888:
            return GL_RGB8;
        case 565:
           return GL_RGB565;
        case 4444:
            return GL_RGBA4;
        case 16:
            return GL_DEPTH_COMPONENT16;
        case 24:
            return GL_DEPTH_COMPONENT24;
        case 32:
            return GL_DEPTH_COMPONENT32F;
        case 8:
            return GL_STENCIL_INDEX8;
        default:
            LOG_WARNING("CRenderTarget: Color format unknown %d. Set defaut: 8888", format);
            return GL_RGBA8;
        };

        return GL_RGBA8;
    };

    u32 samplerSize = DRIVER_CONTEXT->getSamplesCount();

    switch (attach._attachmentType)
    {
        case eColorAttach:
        {
            GLint maxColorAttachments;
            glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColorAttachments);

            if (attach._index >= (u32)maxColorAttachments)
            {
                LOG_ERROR("CRenderTarget: Range out Color attachment max %d index %d", maxColorAttachments, attach._index);
                ASSERT(false, "CRenderTarget: Range out Color attachment");
                return;
            }

            if (!attach._active)
            {
                m_attachBuffers.push_back(GL_NONE);
                return;
            }

            m_attachBuffers.push_back(GL_COLOR_ATTACHMENT0 + attach._index);

            ASSERT(!attach._buffer, "Renderbuffer already exist");
            attach._buffer = new CRenderBufferGL(attach._format, attach._type, m_MSAA ? samplerSize : 0);
            attach._buffer->setSize(Dimension2D(rect.getWidth(), rect.getHeight()));

            attach._buffer->bind();
            CRenderTargetGL::framebufferRenderbuffer(GL_COLOR_ATTACHMENT0 + attach._index, static_cast<CRenderBufferGL*>(attach._buffer)->getRenderBufferID());
            attach._buffer->unbind();

            RENDERER->checkForErrors("CRenderTargetGL: Color attachment error");
        }
            break;

        case eDepthAttach:
        {
            if (!attach._active)
            {
                return;
            }

            ASSERT(!attach._buffer, "Renderbuffer already exist");
            attach._buffer = new CRenderBufferGL(attach._format, attach._type, m_MSAA ? samplerSize : 0);
            attach._buffer->setSize(Dimension2D(rect.getWidth(), rect.getHeight()));

            attach._buffer->bind();
            CRenderTargetGL::framebufferRenderbuffer(GL_DEPTH_ATTACHMENT, static_cast<CRenderBufferGL*>(attach._buffer)->getRenderBufferID());
            attach._buffer->unbind();

            RENDERER->checkForErrors("CRenderTargetGL: Depth attachment error");
        }
            break;

        case eStencilAttach:
        {
            if (!attach._active)
            {
                return;
            }

            ASSERT(!attach._buffer, "Renderbuffer already exist");
            attach._buffer = new CRenderBufferGL(attach._format, attach._type, m_MSAA ? samplerSize : 0);
            attach._buffer->setSize(Dimension2D(rect.getWidth(), rect.getHeight()));

            attach._buffer->bind();
            CRenderTargetGL::framebufferRenderbuffer(GL_STENCIL_ATTACHMENT, static_cast<CRenderBufferGL*>(attach._buffer)->getRenderBufferID());
            attach._buffer->unbind();

            RENDERER->checkForErrors("CRenderTargetGL: Stencil attachment error");
        }
            break;

        default:
        {
            LOG_ERROR("CRenderTarget: Not supported attach type %d", attach._attachmentType);
            ASSERT(false, "CRenderTarget: Not supported attach");
        }
    }
}

void CRenderTargetGL::createRenderToTexture(SAttachments& attach, const Rect32& rect)
{
    switch (attach._attachmentType)
    {
        case eColorAttach:
        {
            GLint maxColorAttachments;
            glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColorAttachments);

            if (attach._index >= (u32)maxColorAttachments)
            {
                LOG_ERROR("CRenderTarget: Range out Color attachment max %d index %d", maxColorAttachments, attach._index);
                ASSERT(false, "CRenderTarget: Range out Color attachment");
                return;
            }

            if (m_attachBuffers.size() < attach._index + 1)
            {
                m_attachBuffers.resize(attach._index + 1, GL_NONE);
            }

            if (!attach._active)
            {
                m_attachBuffers[attach._index] = GL_NONE;
                return;
            }
            else
            {
                m_attachBuffers[attach._index] = GL_COLOR_ATTACHMENT0 + attach._index;
            }

            if (m_MSAA)
            {
                attach._texture = CTextureManager::getInstance()->createTexture2DMSAA(Dimension2D(rect.getWidth(), rect.getHeight()), attach._format, attach._type);
                CRenderTargetGL::framebufferTexture2D(GL_COLOR_ATTACHMENT0 + attach._index, GL_TEXTURE_2D_MULTISAMPLE, static_cast<CTextureGL*>(attach._texture)->getTextureID());
            }
            else
            {
                attach._texture = CTextureManager::getInstance()->createTexture2DFromData(Dimension2D(rect.getWidth(), rect.getHeight()), attach._format, attach._type, nullptr, 8);
                CRenderTargetGL::framebufferTexture2D(GL_COLOR_ATTACHMENT0 + attach._index, GL_TEXTURE_2D, static_cast<CTextureGL*>(attach._texture)->getTextureID());
            }

            RENDERER->checkForErrors("CRenderTargetGL: Color attachment error");
        }
            break;

        case eDepthAttach:
        {
            if (!attach._active)
            {
                return;
            }

            ASSERT(attach._format == EImageFormat::eDepthComponent, "Depth must have only depth format");
            if (m_MSAA)
            {
                attach._texture = CTextureManager::getInstance()->createTexture2DMSAA(Dimension2D(rect.getWidth(), rect.getHeight()), eDepthComponent, attach._type);
                CRenderTargetGL::framebufferTexture2D(GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, static_cast<CTextureGL*>(attach._texture)->getTextureID());
            }
            else
            {
                attach._texture = CTextureManager::getInstance()->createTexture2DFromData(Dimension2D(rect.getWidth(), rect.getHeight()), eDepthComponent, attach._type, nullptr, 8);
                CRenderTargetGL::framebufferTexture2D(GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, static_cast<CTextureGL*>(attach._texture)->getTextureID());
            }

            RENDERER->checkForErrors("CRenderTargetGL: Depth attachment error");
        }
            break;

        case eStencilAttach:
        {
            if (!attach._active)
            {
                return;
            }

            //TODO: will need to rework
            if (m_MSAA)
            {
                attach._texture = CTextureManager::getInstance()->createTexture2DMSAA(Dimension2D(rect.getWidth(), rect.getHeight()), eDepthComponent, attach._type);
                CRenderTargetGL::framebufferTexture2D(GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, static_cast<CTextureGL*>(attach._texture)->getTextureID());
            }
            else
            {
                attach._texture = CTextureManager::getInstance()->createTexture2DFromData(Dimension2D(rect.getWidth(), rect.getHeight()), eDepthComponent, attach._type, nullptr, 8);
                CRenderTargetGL::framebufferTexture2D(GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, static_cast<CTextureGL*>(attach._texture)->getTextureID());
            }

            RENDERER->checkForErrors("CRenderTargetGL: Stencil attachment error");
        }
            break;

        case eDepthStencilAttach:
        {
            if (!attach._active)
            {
                return;
            }

            if (m_MSAA)
            {
                attach._texture = CTextureManager::getInstance()->createTexture2DMSAA(Dimension2D(rect.getWidth(), rect.getHeight()), eDepthComponent, attach._type);
                CRenderTargetGL::framebufferTexture2D(GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, static_cast<CTextureGL*>(attach._texture)->getTextureID());
            }
            else
            {
                attach._texture = CTextureManager::getInstance()->createTexture2DFromData(Dimension2D(rect.getWidth(), rect.getHeight()), eDepthComponent, attach._type, nullptr, 8);
                CRenderTargetGL::framebufferTexture2D(GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, static_cast<CTextureGL*>(attach._texture)->getTextureID());
            }

            RENDERER->checkForErrors("CRenderTargetGL: DepthStencil attachment error");
        }
        break;

        default:
        {
            LOG_ERROR("CRenderTarget: Not supported attach type %d", attach._attachmentType);
            ASSERT(false, "CRenderTarget: Not supported attach");
        }
    }
}

void CRenderTargetGL::copyToRenderbuffer(const RenderTargetPtr& dst)
{
    
    u32 dstBuffer = std::static_pointer_cast<CRenderTargetGL>(dst)->m_frameBufferId;
    CRenderTargetGL::bindFramebuffer(dstBuffer);

    CRenderTargetGL::bindFramebuffer(m_frameBufferId, eFBTargetRead);
    for (u32 i = 0; i < m_attachBuffers.size(); ++i)
    {
        glReadBuffer(m_attachBuffers[i]);

        s32 x = m_viewport.getLeftX();
        s32 y = m_viewport.getTopY();
        s32 width = m_viewport.getWidth();
        s32 height = m_viewport.getHeight();
        s32 invHeight = dst->getViewport().getHeight() - y;

        CRenderTargetGL::blitFramebuffer(Rect32(0, 0, width, height), Rect32(x, invHeight - height, width + x, invHeight),
            GL_COLOR_BUFFER_BIT, m_MSAA ? GL_NEAREST : GL_LINEAR);

        RENDERER->checkForErrors("CRenderTargetGL: Copy to renderbuffer error");
    }
}

bool CRenderTargetGL::bindFramebuffer(u32 buffer, EFramebufferTarget target)
{
    if (s_currentFBO[target] != buffer)
    {
        glBindFramebuffer(ERenderbufferTargetGL[target], buffer);
        ASSERT((glIsFramebuffer(buffer) || buffer == 0), "Invalid FBO index");
        s_currentFBO[target] = buffer;

        return true;
    }

    return false;
}

void CRenderTargetGL::framebufferTexture2D(s32 attachment, s32 target, u32 texture)
{
    ASSERT((glIsTexture(texture) || texture == 0), "Invalid Index Texture");
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, target, texture, 0);
}

void CRenderTargetGL::framebufferRenderbuffer(s32 attachment, u32 buffer)
{
    ASSERT((glIsRenderbuffer(buffer) || buffer == 0), "Invalid Index Renderbuffer");
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, buffer);
}

void CRenderTargetGL::blitFramebuffer(const Rect32& src, const Rect32& dst, u32 mask, u32 filter)
{
    glBlitFramebuffer(src.getLeftX(), src.getBottomY(), src.getRightX(), src.getTopY(),
        dst.getLeftX(), dst.getBottomY(), dst.getRightX(), dst.getTopY(), mask, filter);
}

} //namespace renderer
} //namespace v3d

#endif //_OPENGL_DRIVER_
