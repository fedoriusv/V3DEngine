#include "RenderTargetGL.h"
#include "scene/TextureManager.h"
#include "Engine.h"
#include "utils/Logger.h"

#ifdef _OPENGL_DRIVER_
#include "RenderStateGL.h"
#include "GL/glew.h"

#ifndef _PLATFORM_WIN_
#   pragma clang diagnostic ignored "-Wswitch"
#endif  //_PLATFORM_WIN_

using namespace v3d;
using namespace renderer;
using namespace scene;
using namespace core;

GLenum ERenderbufferTargetGL[] =
{
    GL_DRAW_FRAMEBUFFER,
    GL_READ_FRAMEBUFFER
};

u32 CRenderTargetGL::s_currentFBO[] = { 0 };
u32 CRenderTargetGL::s_currentRBO = 0;

CRenderTargetGL::CRenderTargetGL()
    : m_frameBufferID(0U)
    , m_renderBufferID(0U)

    , m_lastFrameIndex(1U)

    , m_hasClearColor(true)
    , m_hasClearDepth(true)
    , m_hasClearStencil(false)
{
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint*)&m_frameBufferID);
}

CRenderTargetGL::~CRenderTargetGL()
{
    CRenderTargetGL::destroy();
    m_attachBuffers.clear();
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
    bool chaned = CRenderTargetGL::bindFramebuffer(m_frameBufferID);
    if (chaned)
    {
        RENDERER->setCurrentRenderTarget(shared_from_this());
        const Rect32& rect = getViewport();
        glViewport(0, 0, rect.getWidth(), rect.getHeight());

        if (m_attachBuffers.size() > 0)
        {
            glDrawBuffers((GLsizei)m_attachBuffers.size(), &m_attachBuffers[0]);
        }
        else
        {
            glDrawBuffer(m_frameBufferID ? GL_NONE : GL_BACK);
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

        }

        if (CRenderTargetGL::hasClearStencilTarget())
        {
            flags = CRenderTarget::getClearStencilBuffer() ? GL_STENCIL_BUFFER_BIT : 0;
            CRenderStateGL::stencilWrite(true);
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
    /*bool chaned = CRenderTargetGL::bindFramebuffer(0);
    if (chaned)
    {
        glReadBuffer(GL_BACK);
        glDrawBuffer(GL_BACK);

        const RenderTargetPtr& target = RENDERER->getDefaultRenderTarget();
        const Rect& rect = target->getViewport();
        glViewport(0, 0, rect.getWidth(), rect.getHeight());

        RENDERER->setCurrentRenderTarget(target);
    }*/

    //TODO: test blit renderbuffers
    /*if (m_frameBufferID != 0 && m_attachBuffers.size() > 0)
    {
        CRenderTargetGL::copyToRenderbuffer(RENDERER->getDefaultRenderTarget());
    }*/
}

bool CRenderTargetGL::create()
{
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
    GLint originalRBO = 0;
    glGetIntegerv(GL_RENDERBUFFER_BINDING, &originalRBO);

    CRenderTarget::setViewport(Rect32(x, y, x + width, y + height));

    CRenderTargetGL::genFramebuffer(m_frameBufferID);
    CRenderTargetGL::bindFramebuffer(m_frameBufferID);

    for (auto& attach : m_attachmentsList)
    {
        switch (attach._output)
        {
            case eTextureOutput:
                CRenderTargetGL::createRenderToTexture(attach, rect);
                break;

            case eRenderOutput:
                CRenderTargetGL::createRenderbuffer(attach, rect);
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

    ASSERT((result == GL_FRAMEBUFFER_COMPLETE) && "CRenderTarget: Create render target Error");
    RENDERER->checkForErrors("CRenderTargetGL: Create render target Error");

    if (originalRBO >= 0)
    {
        CRenderTargetGL::bindRenderbuffer(originalRBO);
    }
    if (originalFBO >= 0)
    {
        CRenderTargetGL::bindFramebuffer(originalFBO);
    }

    return result == GL_FRAMEBUFFER_COMPLETE;
}

void CRenderTargetGL::destroy()
{
    for (auto& attach : m_attachmentsList)
    {
        switch (attach._type)
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

    CRenderTargetGL::deleteRenderbuffers(m_renderBufferID);
    CRenderTargetGL::deleteFramebuffers(m_frameBufferID);
    m_frameBufferID = 0;
    m_renderBufferID = 0;
}

void CRenderTargetGL::createRenderbuffer(SAttachments& attach, const Rect32& rect)
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

    u32 samplerSize = DRIVER_CONTEXT->getSamplersCount();

    switch (attach._type)
    {
        case eColorAttach:
        {
            GLint maxColorAttachments;
            glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColorAttachments);

            if (attach._index >= (u32)maxColorAttachments)
            {
                LOG_ERROR("CRenderTarget: Range out Color attachment max %d index %d", maxColorAttachments, attach._index);
                ASSERT("CRenderTarget: Range out Color attachment" && false);
                return;
            }
            m_attachBuffers.push_back(GL_COLOR_ATTACHMENT0 + attach._index);

            CRenderTargetGL::genRenderbuffer(attach._rboID);
            CRenderTargetGL::bindRenderbuffer(attach._rboID);
            CRenderTargetGL::renderbufferStorage(internalFormat(attach._format), rect, m_MSAA ? samplerSize : 0);
            CRenderTargetGL::bindRenderbuffer(0);

            CRenderTargetGL::framebufferRenderbuffer(GL_COLOR_ATTACHMENT0 + attach._index, attach._rboID);

            RENDERER->checkForErrors("CRenderTargetGL: Color attachment error");
        }
            break;

        case eDepthAttach:
        {
            if (!m_renderBufferID)
            {
                CRenderTargetGL::genRenderbuffer(m_renderBufferID);
            }
            attach._rboID = m_renderBufferID;
            CRenderTargetGL::bindRenderbuffer(attach._rboID);
            CRenderTargetGL::renderbufferStorage(internalFormat(attach._format), rect, m_MSAA ? samplerSize : 0);
            CRenderTargetGL::bindRenderbuffer(0);

            CRenderTargetGL::framebufferRenderbuffer(GL_DEPTH_ATTACHMENT, attach._rboID);

            RENDERER->checkForErrors("CRenderTargetGL: Depth attachment error");
        }
            break;

        case eStencilAttach:
        {
            if (!m_renderBufferID)
            {
                CRenderTargetGL::genRenderbuffer(m_renderBufferID);
            }
            attach._rboID = m_renderBufferID;

            CRenderTargetGL::bindRenderbuffer(attach._rboID);
            CRenderTargetGL::renderbufferStorage(internalFormat(attach._format), rect, m_MSAA ? samplerSize : 0);
            CRenderTargetGL::bindRenderbuffer(0);

            CRenderTargetGL::framebufferRenderbuffer(GL_STENCIL_ATTACHMENT, attach._rboID);

            RENDERER->checkForErrors("CRenderTargetGL: Stencil attachment error");
        }
            break;

        default:
        {
            LOG_ERROR("CRenderTarget: Not supported attach type %d", attach._type);
            ASSERT("CRenderTarget: Not supported attach" && false);
        }
    }
}

void CRenderTargetGL::createRenderToTexture(SAttachments& attach, const Rect32& rect)
{
    EImageFormat imageFormat = eRGBA;
    EImageType imageType = eUnsignedShort;
    std::function<void(u32)> formatColor = [&imageFormat, &imageType](u32 format)
    {
        switch (format)
        {
        case 8888:
            imageFormat = eRGBA;
            imageType = eUnsignedShort;
            break;

        case 888:
            imageFormat = eRGB;
            imageType = eUnsignedShort;
            break;

        case 565:
            imageFormat = eRGB;
            imageType = eUnsignedShort_565;
            break;

        case 4444:
            imageFormat = eRGBA;
            imageType = eUnsignedShort_4444;
            break;

        default:
            imageFormat = eRGBA;
            imageType = eUnsignedShort;
            LOG_WARNING("CRenderTarget: Color format unknown %d. Set defaut: 8888", format);
            break;
        }
    };

    switch (attach._type)
    {
        case eColorAttach:
        {
            GLint maxColorAttachments;
            glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColorAttachments);

            if (attach._index >= (u32)maxColorAttachments)
            {
                LOG_ERROR("CRenderTarget: Range out Color attachment max %d index %d", maxColorAttachments, attach._index);
                ASSERT("CRenderTarget: Range out Color attachment" && false);
                return;
            }
            m_attachBuffers.push_back(GL_COLOR_ATTACHMENT0 + attach._index);

            formatColor(attach._format);
            if (m_MSAA)
            {
                attach._texture = CTextureManager::getInstance()->createTexture2DMSAA(Dimension2D(rect.getWidth(), rect.getHeight()), imageFormat, imageType);
                CRenderTargetGL::framebufferTexture2D(GL_COLOR_ATTACHMENT0 + attach._index, GL_TEXTURE_2D_MULTISAMPLE, attach._texture->getTextureID());
            }
            else
            {
                attach._texture = CTextureManager::getInstance()->createTexture2DFromData(Dimension2D(rect.getWidth(), rect.getHeight()), imageFormat, imageType, nullptr);
                CRenderTargetGL::framebufferTexture2D(GL_COLOR_ATTACHMENT0 + attach._index, GL_TEXTURE_2D, attach._texture->getTextureID());
            }

            RENDERER->checkForErrors("CRenderTargetGL: Color attachment error");
        }
            break;

        case eDepthAttach:
        {
            if (m_MSAA)
            {
                attach._texture = CTextureManager::getInstance()->createTexture2DMSAA(Dimension2D(rect.getWidth(), rect.getHeight()), eDepthComponent, eUnsignedInt);
                CRenderTargetGL::framebufferTexture2D(GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, attach._texture->getTextureID());
            }
            else
            {
                attach._texture = CTextureManager::getInstance()->createTexture2DFromData(Dimension2D(rect.getWidth(), rect.getHeight()), eDepthComponent, eUnsignedInt, nullptr);
                CRenderTargetGL::framebufferTexture2D(GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, attach._texture->getTextureID());
            }

            RENDERER->checkForErrors("CRenderTargetGL: Depth attachment error");
        }
            break;

        case eStencilAttach:
        {
            //TODO: need rework
            if (m_MSAA)
            {
                attach._texture = CTextureManager::getInstance()->createTexture2DMSAA(Dimension2D(rect.getWidth(), rect.getHeight()), eDepthComponent, eUnsignedInt);
                CRenderTargetGL::framebufferTexture2D(GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, attach._texture->getTextureID());
            }
            else
            {
                attach._texture = CTextureManager::getInstance()->createTexture2DFromData(Dimension2D(rect.getWidth(), rect.getHeight()), eDepthComponent, eUnsignedInt, nullptr);
                CRenderTargetGL::framebufferTexture2D(GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, attach._texture->getTextureID());
            }

            RENDERER->checkForErrors("CRenderTargetGL: Stencil attachment error");
        }
            break;

        default:
        {
            LOG_ERROR("CRenderTarget: Not supported attach type %d", attach._type);
            ASSERT("CRenderTarget: Not supported attach" && false);
        }
    }
}

void CRenderTargetGL::copyToRenderbuffer(const RenderTargetPtr& dst)
{
    
    u32 dstBuffer = std::static_pointer_cast<CRenderTargetGL>(dst)->m_frameBufferID;
    CRenderTargetGL::bindFramebuffer(dstBuffer);

    CRenderTargetGL::bindFramebuffer(m_frameBufferID, eFBTargetRead);
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

void CRenderTargetGL::genFramebuffer(u32& buffer)
{
    glGenFramebuffers(1, &buffer);
}

bool CRenderTargetGL::bindFramebuffer(u32 buffer, EFramebufferTarget target)
{
    if (s_currentFBO[target] != buffer)
    {
        glBindFramebuffer(ERenderbufferTargetGL[target], buffer);
        ASSERT((glIsFramebuffer(buffer) || buffer == 0) && "Invalid FBO index");
        s_currentFBO[target] = buffer;

        return true;
    }

    return false;
}

void CRenderTargetGL::deleteFramebuffers(u32& buffer)
{
    if (buffer != 0)
    {
        ASSERT(glIsFramebuffer(buffer) && "Invalid Index FBO");
        glDeleteFramebuffers(1, &buffer);
    }
}

void CRenderTargetGL::genRenderbuffer(u32& buffer)
{
    glGenRenderbuffers(1, &buffer);
}

bool CRenderTargetGL::bindRenderbuffer(u32 buffer)
{
    if (s_currentRBO != buffer)
    {
        glBindRenderbuffer(GL_RENDERBUFFER, buffer);
        ASSERT((glIsRenderbuffer(buffer) || buffer == 0) && "Invalid RBO index");
        s_currentRBO = buffer;

        return true;
    }

    return false;
}

void CRenderTargetGL::deleteRenderbuffers(u32& buffer)
{
    if (buffer != 0)
    {
        ASSERT(glIsRenderbuffer(buffer) && "Invalid Index RBO");
        glDeleteRenderbuffers(1, &buffer);
    }
}

void CRenderTargetGL::framebufferTexture2D(s32 attachment, s32 target, u32 texture)
{
    ASSERT((glIsTexture(texture) || texture == 0) && "Invalid Index Texture");
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, target, texture, 0);
}

void CRenderTargetGL::framebufferRenderbuffer(s32 attachment, u32 buffer)
{
    ASSERT((glIsRenderbuffer(buffer) || buffer == 0) && "Invalid Index Renderbuffer");
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, buffer);
}

void CRenderTargetGL::blitFramebuffer(const Rect32& src, const Rect32& dst, u32 mask, u32 filter)
{
    glBlitFramebuffer(src.getLeftX(), src.getBottomY(), src.getRightX(), src.getTopY(),
        dst.getLeftX(), dst.getBottomY(), dst.getRightX(), dst.getTopY(), mask, filter);
}

void CRenderTargetGL::renderbufferStorage(u32 internalFormat, const Rect32& size, u32 samplers)
{
    if (samplers > 0)
    {
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, samplers, internalFormat, size.getWidth(), size.getHeight());
    }
    else
    {
        glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, size.getWidth(), size.getHeight());
    }
}

#endif //_OPENGL_DRIVER_
