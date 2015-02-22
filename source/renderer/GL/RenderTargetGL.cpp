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
{
    const core::Dimension2D& size = WINDOW->getSize();
    CRenderTarget::setViewportSize(size);

    glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint*)&m_frameBufferID);
}

CRenderTargetGL::~CRenderTargetGL()
{
    CRenderTargetGL::destroy();
}

void CRenderTargetGL::bind()
{
    bool chaned = CRenderTargetGL::bindFramebuffer(m_frameBufferID);
    if (chaned)
    {
        glViewport(0, 0, getViewportSize().width, getViewportSize().height);
    }

    u32 frameIdx = RENDERER->getFrameIndex();
    if (chaned || frameIdx != m_lastFrameIndex)
    {
        m_lastFrameIndex = frameIdx;

        u32 flags = 0;
        if (CRenderTarget::hasClearDepthTarget())
        {
            flags = CRenderTarget::getClearDepthBuffer() ? GL_DEPTH_BUFFER_BIT : 0;
            glDepthMask(GL_TRUE);

        }

        if (CRenderTarget::hasClearColorTarget())
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

    EImageFormat format = CRenderTarget::getImageFormat();
    EImageType type = CRenderTarget::getImageType();;

    GLint originalFBO = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &originalFBO);
    GLint originalRBO = 0;
    glGetIntegerv(GL_RENDERBUFFER_BINDING, &originalRBO);

    CRenderTarget::setViewportSize(size);

    //colorTexture
    CRenderTargetGL::genFramebuffer(m_frameBufferID);
    CRenderTargetGL::bindFramebuffer(m_frameBufferID);

    if (CRenderTarget::hasClearColorTarget())
    {
        TexturePtr colorTexture = CTextureManager::getInstance()->createTexture2DFromData(size, format, type, nullptr);
        CRenderTarget::setColorTexture(colorTexture);

        CRenderTargetGL::framebufferTexture2D(GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture->getTextureID());
    }

    //depthTexture
    if (CRenderTarget::hasClearDepthTarget())
    {
        if (glewIsSupported("GL_ARB_depth_texture"))
        {
            TexturePtr depthTexture = CTextureManager::getInstance()->createTexture2DFromData(size, eDepthComponent, eUnsignedInt, nullptr);
            CRenderTarget::setDepthTexture(depthTexture);

            CRenderTargetGL::framebufferTexture2D(GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture->getTextureID());
        }
        else
        {
            CRenderTargetGL::genRenderbuffer(m_depthBufferID);
            CRenderTargetGL::bindRenderbuffer(m_depthBufferID);

            std::function<s32(s32)> componentSize = [](s32 component)
            {
                switch (component)
                {
                case 16:
                    return GL_DEPTH_COMPONENT16;
                case 24:
                    return GL_DEPTH_COMPONENT24;
                case 32:
                    return GL_DEPTH_COMPONENT32;
                default:
                    return GL_DEPTH_COMPONENT16;
                };
                return GL_DEPTH_COMPONENT16;
            };

            glRenderbufferStorage(GL_RENDERBUFFER, componentSize(m_depthSize), size.width, size.height);
            CRenderTargetGL::framebufferTexture2D(GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBufferID);
        }
    }

    GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (result != GL_FRAMEBUFFER_COMPLETE)
    {
        LOG_ERROR("CRenderTargetGL : Bad framebuffer");
    }

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
    CRenderTarget::setColorTexture(nullptr);
    CRenderTarget::setDepthTexture(nullptr);

    framebufferTexture2D(GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
    CRenderTargetGL::deleteRenderbuffers(m_depthBufferID);

    framebufferTexture2D(GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0);
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
