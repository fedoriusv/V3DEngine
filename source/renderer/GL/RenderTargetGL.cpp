#include "RenderTargetGL.h"
#include "RenderStateGL.h"
#include "scene/TextureManager.h"
#include "Engine.h"
#include "utils/Logger.h"

#include "GL/glew.h"

using namespace v3d;
using namespace renderer;
using namespace scene;
using namespace core;

u32 CRenderTargetGL::s_currentFBO = 0;
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
        const Rect& rect = getViewport();
        glViewport(rect.getLeftX(), rect.getTopY(), rect.getRightX(), rect.getBottomY());

        if (m_attachBuffers.size() > 0)
        {
            glDrawBuffers((GLsizei)m_attachBuffers.size(), &m_attachBuffers[0]);
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
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    bool chaned = CRenderTargetGL::bindFramebuffer(0);
    if (chaned)
    {
        glReadBuffer(GL_BACK);
        glDrawBuffer(GL_BACK);

        const RenderTargetPtr& target = RENDERER->getDefaultRenderTarget();
        const Rect& rect = target->getViewport();
        glViewport(rect.getLeftX(), rect.getTopY(), rect.getWidth(), rect.getHeight());

        RENDERER->setCurrentRenderTarget(target);
    }
}

bool CRenderTargetGL::create()
{
    const Rect& rect = getViewport();
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

    CRenderTarget::setViewport(Rect(x, y, x + width, y + height));

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
    //TODO: Have gl error GL_INVALID_OPERATION after call glFramebufferTexture2D

    CRenderTargetGL::deleteRenderbuffers(m_renderBufferID);
    CRenderTargetGL::deleteFramebuffers(m_frameBufferID);
    m_frameBufferID = 0;
    m_renderBufferID = 0;
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
            m_attachBuffers.push_back(GL_COLOR_ATTACHMENT0 + attach._index);

            CRenderTargetGL::genRenderbuffer(attach._rboID);
            CRenderTargetGL::bindRenderbuffer(attach._rboID);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, rect.getWidth(), rect.getHeight());
            CRenderTargetGL::bindRenderbuffer(0);

            CRenderTargetGL::framebufferRenderbuffer(GL_COLOR_ATTACHMENT0 + attach._index, GL_FRAMEBUFFER, attach._rboID);

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
            m_attachBuffers.push_back(GL_COLOR_ATTACHMENT0 + attach._index);

            formatColor(attach._format);
            if (m_MSAA)
            {
                GLuint tex;
                glGenTextures(1, &tex);
                glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, tex);
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA8, rect.getWidth(), rect.getHeight(), false);
                CRenderTargetGL::framebufferTexture2D(GL_COLOR_ATTACHMENT0 + attach._index, GL_TEXTURE_2D_MULTISAMPLE, tex);

                /*attach._texture = CTextureManager::getInstance()->createTexture2DMSAA(Dimension2D(rect.getWidth(), rect.getHeight()), imageFormat, imageType);
                CRenderTargetGL::framebufferTexture2D(GL_COLOR_ATTACHMENT0 + attach._index, GL_TEXTURE_2D_MULTISAMPLE, attach._texture->getTextureID());*/
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
                GLuint tex;
                glGenTextures(1, &tex);
                glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, tex);
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_DEPTH_COMPONENT, rect.getWidth(), rect.getHeight(), false);
                CRenderTargetGL::framebufferTexture2D(GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, tex);

                /*attach._texture = CTextureManager::getInstance()->createTexture2DMSAA(Dimension2D(rect.getWidth(), rect.getHeight()), eDepthComponent, eUnsignedInt);
                CRenderTargetGL::framebufferTexture2D(GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, attach._texture->getTextureID());*/
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

void CRenderTargetGL::genFramebuffer(u32& buffer)
{
    glGenFramebuffers(1, &buffer);
}

bool CRenderTargetGL::bindFramebuffer(u32 buffer)
{
    if (s_currentFBO != buffer)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, buffer);
        ASSERT((glIsFramebuffer(buffer) || buffer == 0) && "Invalid FBO index");
        s_currentFBO = buffer;

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

void CRenderTargetGL::framebufferRenderbuffer(s32 attachment, s32 target, u32 buffer)
{
    ASSERT((glIsRenderbuffer(buffer) || buffer == 0) && "Invalid Index Renderbuffer");
    glFramebufferRenderbuffer(target, attachment, GL_RENDERBUFFER, buffer);
}

void CRenderTargetGL::blitFramebuffer(const Rect& src, const Rect& dst, u32 mask)
{
    glBlitFramebuffer(src.getLeftX(), src.getTopY(), src.getRightX(), src.getHeight(),
        dst.getLeftX(), dst.getTopY(), dst.getRightX(), dst.getHeight(), mask, GL_NEAREST);
}
