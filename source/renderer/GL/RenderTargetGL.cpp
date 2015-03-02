#include "RenderTargetGL.h"
#include "scene/TextureManager.h"
#include "Engine.h"
#include "utils/Logger.h"

#include "GL/glew.h"

using namespace v3d;
using namespace renderer;
using namespace scene;
using namespace core;

u32 CRenderTargetGL::m_currentFBO = 0;
u32 CRenderTargetGL::m_currentRBO = 0;

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
        const Rect& rect = getViewport();
        glViewport(rect.getLeftX(), rect.getBottomY(), rect.getWidth(), rect.getHeight());
    }

    u32 frameIdx = RENDERER->getFrameIndex();
    if (chaned || frameIdx != m_lastFrameIndex)
    {
        m_lastFrameIndex = frameIdx;

        u32 flags = 0;
        if (CRenderTargetGL::hasClearDepthTarget())
        {
            flags = CRenderTarget::getClearDepthBuffer() ? GL_DEPTH_BUFFER_BIT : 0;
            glDepthMask(GL_TRUE);

        }

        if (CRenderTargetGL::hasClearStencilTarget())
        {
            flags = CRenderTarget::getClearDepthBuffer() ? GL_STENCIL_BUFFER_BIT : 0;
            glStencilMask(GL_TRUE);

        }

        if (CRenderTargetGL::hasClearColorTarget())
        {
            flags |= GL_COLOR_BUFFER_BIT;

            const core::Vector4D& color = CRenderTarget::getCearColor();
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
        RENDERER->setCurrentRenderTarget(shared_from_this());
    }
}

bool CRenderTargetGL::create()
{
    const Rect& rect = getViewport();
    u32 width = rect.getWidth();
    u32 height = rect.getHeight();
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

    CRenderTarget::setViewport(Rect(rect.getLeftX(), rect.getBottomY(), width, height));

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
    };

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
            framebufferTexture2D(GL_COLOR_ATTACHMENT0 + attach._index, GL_TEXTURE_2D, 0);
            break;

        case eDepthAttach:
            framebufferTexture2D(GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0);
            break;

        case eStencilAttach:
            framebufferTexture2D(GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, 0);
            break;
        }
    }

    CRenderTargetGL::deleteRenderbuffers(m_renderBufferID);
    CRenderTargetGL::deleteFramebuffers(m_frameBufferID);
}

void CRenderTargetGL::createRenderbuffer(SAttachments& attach, const Rect& rect)
{
    GLint maxColorAttachments;
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColorAttachments);

    switch (attach._type)
    {
        case eColorAttach:
        {
            if (attach._index >= (u32)maxColorAttachments)
            {
                LOG_ERROR("CRenderTarget: Range out Color attachment max %d index %d", maxColorAttachments, attach._index);
                ASSERT("CRenderTarget: Range out Color attachment" && false);
                return;
            }

            CRenderTargetGL::genRenderbuffer(attach._rboID);
            CRenderTargetGL::bindRenderbuffer(attach._rboID);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, rect.getWidth(), rect.getHeight());
            CRenderTargetGL::bindRenderbuffer(0);

            CRenderTargetGL::framebufferRenderbuffer(GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER, attach._rboID);

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
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, rect.getWidth(), rect.getHeight());
            CRenderTargetGL::bindRenderbuffer(0);

            CRenderTargetGL::framebufferRenderbuffer(GL_DEPTH_ATTACHMENT, GL_FRAMEBUFFER, attach._rboID);

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
            glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8, rect.getWidth(), rect.getHeight());
            CRenderTargetGL::bindRenderbuffer(0);

            CRenderTargetGL::framebufferRenderbuffer(GL_STENCIL_ATTACHMENT, GL_FRAMEBUFFER, attach._rboID);

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

void CRenderTargetGL::createRenderToTexture(SAttachments& attach, const Rect& rect)
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

    GLint maxColorAttachments;
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColorAttachments);

    switch (attach._type)
    {
        case eColorAttach:
        {
            if (attach._index >= (u32)maxColorAttachments)
            {
                LOG_ERROR("CRenderTarget: Range out Color attachment max %d index %d", maxColorAttachments, attach._index);
                ASSERT("CRenderTarget: Range out Color attachment" && false);
                return;
            }

            formatColor(attach._format);
            attach._texture = CTextureManager::getInstance()->createTexture2DFromData(Dimension2D(rect.getWidth(), rect.getWidth()), imageFormat, imageType, nullptr);
            CRenderTargetGL::framebufferTexture2D(GL_COLOR_ATTACHMENT0 + attach._index, GL_TEXTURE_2D, attach._texture->getTextureID());

            RENDERER->checkForErrors("CRenderTargetGL: Color attachment error");
        }
            break;

        case eDepthAttach:
        {
            attach._texture = CTextureManager::getInstance()->createTexture2DFromData(Dimension2D(rect.getWidth(), rect.getWidth()), eDepthComponent, eUnsignedInt, nullptr);
            CRenderTargetGL::framebufferTexture2D(GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, attach._texture->getTextureID());

            RENDERER->checkForErrors("CRenderTargetGL: Depth attachment error");
        }
            break;

        case eStencilAttach:
        {
            //TODO: need rework
            attach._texture = CTextureManager::getInstance()->createTexture2DFromData(Dimension2D(rect.getWidth(), rect.getWidth()), eDepthComponent, eUnsignedInt, nullptr);
            CRenderTargetGL::framebufferTexture2D(GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, attach._texture->getTextureID());

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

void CRenderTargetGL::genFramebuffer(u32& buffer)
{
    glGenFramebuffers(1, &buffer);
    ASSERT(glIsFramebuffer(buffer) || "Invalid FBO index");
}

bool CRenderTargetGL::bindFramebuffer(const u32 buffer)
{
    ASSERT(glIsFramebuffer(buffer) || "Invalid FBO index");
    if (m_currentFBO != buffer)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, buffer);
        m_currentFBO = buffer;

        return true;
    }

    return false;
}

void CRenderTargetGL::deleteFramebuffers(u32& buffer)
{
    if (buffer != 0)
    {
        ASSERT(glIsFramebuffer(buffer) || "Invalid Index FBO");
        glDeleteFramebuffers(1, &buffer);
    }
}

void CRenderTargetGL::genRenderbuffer(u32& buffer)
{
    glGenRenderbuffers(1, &buffer);
    ASSERT(glIsRenderbuffer(buffer) || "Invalid RBO index");
}

bool CRenderTargetGL::bindRenderbuffer(const u32 buffer)
{
    ASSERT(glIsRenderbuffer(buffer) || "Invalid RBO index");
    if (m_currentRBO != buffer)
    {
        glBindRenderbuffer(GL_RENDERBUFFER, buffer);
        m_currentRBO = buffer;

        return true;
    }

    return false;
}

void CRenderTargetGL::deleteRenderbuffers(u32& buffer)
{
    if (buffer != 0)
    {
        ASSERT(glIsRenderbuffer(buffer) || "Invalid Index RBO");
        glDeleteRenderbuffers(1, &buffer);
    }
}

void CRenderTargetGL::framebufferTexture2D(s32 attachment, s32 target, u32 texture)
{
    ASSERT(glIsTexture(texture) || "Invalid Index Texture");
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, target, texture, 0);
}

void CRenderTargetGL::framebufferRenderbuffer(s32 attachment, s32 target, u32 buffer)
{
    ASSERT(glIsRenderbuffer(buffer) || "Invalid Index Renderbuffer");
    glFramebufferRenderbuffer(target, attachment, GL_RENDERBUFFER, buffer);
}
