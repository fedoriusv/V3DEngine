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
    : m_frameBufferID(0)
    , m_depthBufferID(0)
    , m_lastFrameIndex(1U)

    , m_hasClearColor(true)
    , m_hasClearDepth(true)
    , m_hasClearStencil(false)
{
    const core::Dimension2D& size = WINDOW->getSize();
    CRenderTarget::setViewportSize(size);

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
        glViewport(0, 0, getViewportSize().width, getViewportSize().height);
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
    core::Dimension2D size = getViewportSize();
    if (size.width == 0)
    {
        size.width = WINDOW->getSize().width;
    }

    if (size.height == 0)
    {
        size.height = WINDOW->getSize().height;
    }

    size.width = core::getSmallestPowerOf2(size.width);
    size.height = core::getSmallestPowerOf2(size.height);

    GLint originalFBO = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &originalFBO);
    GLint originalRBO = 0;
    glGetIntegerv(GL_RENDERBUFFER_BINDING, &originalRBO);

    CRenderTarget::setViewportSize(size);

    CRenderTargetGL::genFramebuffer(m_frameBufferID);
    CRenderTargetGL::bindFramebuffer(m_frameBufferID);

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

    for (auto& attach : m_attachmentsList)
    {
        switch (attach._type)
        {
            case eColorAttach:
            {
                if (attach._index >= (u32)maxColorAttachments)
                {
                    LOG_ERROR("CRenderTarget: Range out Color attachment max %d index %d", maxColorAttachments, attach._index);
                    ASSERT("CRenderTarget: Range out Color attachment" && false);
                    continue;
                }

                /*formatColor(attach._format);
                attach._texture = CTextureManager::getInstance()->createTexture2DFromData(size, imageFormat, imageType, nullptr);
                CRenderTargetGL::framebufferTexture2D(GL_COLOR_ATTACHMENT0 + attach._index, GL_TEXTURE_2D, attach._texture->getTextureID());*/

                //TEST
                GLuint colorBufferID;
                CRenderTargetGL::genRenderbuffer(colorBufferID);
                CRenderTargetGL::bindRenderbuffer(colorBufferID);
                glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, size.width, size.height);
                CRenderTargetGL::bindRenderbuffer(0);

                CRenderTargetGL::framebufferRenderbuffer(GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER, colorBufferID);
                //

                RENDERER->checkForErrors("CRenderTargetGL: Color attachment error");
            }
                break;

            case eDepthAttach:
            {
                /*attach._texture = CTextureManager::getInstance()->createTexture2DFromData(size, eDepthComponent, eUnsignedInt, nullptr);
                CRenderTargetGL::framebufferTexture2D(GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, attach._texture->getTextureID());*/

                //TEST
                GLuint depthBufferID;
                CRenderTargetGL::genRenderbuffer(depthBufferID);
                CRenderTargetGL::bindRenderbuffer(depthBufferID);
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size.width, size.height);
                CRenderTargetGL::bindRenderbuffer(0);

                CRenderTargetGL::framebufferRenderbuffer(GL_DEPTH_ATTACHMENT, GL_FRAMEBUFFER, depthBufferID);
                //

                RENDERER->checkForErrors("CRenderTargetGL: Depth attachment error");
            }
                break;

            case eStencilAttach:
            {
                attach._texture = CTextureManager::getInstance()->createTexture2DFromData(size, eDepthComponent, eUnsignedInt, nullptr);
                CRenderTargetGL::framebufferTexture2D(GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, attach._texture->getTextureID());

                RENDERER->checkForErrors("CRenderTargetGL: Stencil attachment error");
            }
        }
    }

    ////colorTexture
    //if (CRenderTarget::hasClearColorTarget())
    //{
    //    TexturePtr colorTexture = CTextureManager::getInstance()->createTexture2DFromData(size, format, type, nullptr);
    //    CRenderTarget::setColorTexture(colorTexture);

    //    CRenderTargetGL::framebufferTexture2D(GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture->getTextureID());

    //    GLuint m_depthBufferID1;
    //    CRenderTargetGL::genRenderbuffer(m_depthBufferID1);
    //    CRenderTargetGL::bindRenderbuffer(m_depthBufferID1);

    //    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, size.width, size.height);
    //    CRenderTargetGL::framebufferRenderbuffer(GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER, m_depthBufferID1);
    //    RENDERER->checkForErrors("CRenderTargetGL: Create render target Error");*/
    //}

    ////depthTexture
    //if (CRenderTarget::hasClearDepthTarget())
    //{
    //    if (glewIsSupported("GL_ARB_depth_texture"))
    //    {
    //        TexturePtr depthTexture = CTextureManager::getInstance()->createTexture2DFromData(size, eDepthComponent, eUnsignedInt, nullptr);
    //        CRenderTarget::setDepthTexture(depthTexture);

    //        CRenderTargetGL::framebufferTexture2D(GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture->getTextureID());
    //    }
    //    else
    //    {
    //        CRenderTargetGL::genRenderbuffer(m_depthBufferID);
    //        CRenderTargetGL::bindRenderbuffer(m_depthBufferID);

    //        std::function<s32(s32)> componentSize = [](s32 component)
    //        {
    //            switch (component)
    //            {
    //            case 16:
    //                return GL_DEPTH_COMPONENT16;
    //            case 24:
    //                return GL_DEPTH_COMPONENT24;
    //            case 32:
    //                return GL_DEPTH_COMPONENT32;
    //            default:
    //                return GL_DEPTH_COMPONENT16;
    //            };
    //            return GL_DEPTH_COMPONENT16;
    //        };

    //        glRenderbufferStorage(GL_RENDERBUFFER, componentSize(m_depthSize), size.width, size.height);
    //        CRenderTargetGL::framebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depthBufferID);
    //    }
    //}

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

    CRenderTargetGL::deleteRenderbuffers(m_depthBufferID);
    CRenderTargetGL::deleteFramebuffers(m_frameBufferID);
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
