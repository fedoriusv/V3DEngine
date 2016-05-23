#include "RenderBufferGL.h"
#include "TextureGL.h"
#include "utils/Logger.h"
#include "Engine.h"

#ifdef _OPENGL_DRIVER_
#include "GL/glew.h"

namespace v3d
{

using namespace core;

namespace renderer
{
namespace gl
{

GLenum ERenderBufferImageFormatGL[toEnumType(EImageFormat::eFormatCount)] =
{
    GL_RED,
    GL_RG,
    GL_RGB,
    GL_RGBA,
    GL_DEPTH_COMPONENT,
    GL_STENCIL_INDEX
};

GLenum ERenderBufferImageTypeGL[toEnumType(EImageType::eImageTypeCount)] =
{
    GL_BYTE,
    GL_UNSIGNED_BYTE,
    GL_SHORT,
    GL_UNSIGNED_SHORT,
    GL_INT,
    GL_UNSIGNED_INT,
    GL_UNSIGNED_INT_24_8,
    GL_HALF_FLOAT,
    GL_FLOAT
};

u32 CRenderBufferGL::s_currentBuffer = 0;

CRenderBufferGL::CRenderBufferGL(EImageFormat format, EImageType type, u32 samples)
    : m_renderBufferID(0)
    , m_samplesCount(samples)
{
    LOG_DEBUG("CRenderBufferGL::CRenderBufferGL constructor %x", this);

    glGenRenderbuffers(1, &m_renderBufferID);

    CRenderBuffer::setFormat(format);
    CRenderBuffer::setType(type);

    /*GLint originalRBO = 0;
    glGetIntegerv(GL_RENDERBUFFER_BINDING, &originalRBO);

    if (m_changed)
    {
        CRenderBufferGL::bind();
        CRenderBufferGL::renderbufferStorage(internalFormat(format, type), CRenderBuffer::getSize(), m_samplesCount);
        CRenderBufferGL::unbind();
    }

    if (originalRBO > 0)
    {
        glBindRenderbuffer(GL_RENDERBUFFER, originalRBO);
    }*/
}

CRenderBufferGL::~CRenderBufferGL()
{
    if (m_renderBufferID != 0)
    {
#ifdef _DEBUG_GL
        ASSERT(glIsRenderbuffer(m_renderBufferID), "Invalid Index Buffer");
#endif //_DEBUG_GL
        glDeleteRenderbuffers(1, &m_renderBufferID);
        m_renderBufferID = 0;
    }

    ASSERT(m_renderBufferID == 0, "RBO doesn't deleted");
    LOG_DEBUG("CRenderBufferGL: CRenderBufferGL destructor %x", this);
}

bool CRenderBufferGL::bind()
{
    if (s_currentBuffer != m_renderBufferID)
    {
        glBindRenderbuffer(GL_RENDERBUFFER, m_renderBufferID);
        ASSERT((glIsRenderbuffer(m_renderBufferID) || m_renderBufferID == 0), "Invalid RBO index");
        s_currentBuffer = m_renderBufferID;

        if (m_changed)
        {
            CRenderBufferGL::renderbufferStorage(CTextureGL::internalFormat(ERenderBufferImageFormatGL[toEnumType(CRenderBuffer::getFormat())],
                ERenderBufferImageTypeGL[toEnumType(CRenderBuffer::getType())]), CRenderBuffer::getSize(), m_samplesCount);
        }

        return true;
    }

    if (m_changed)
    {
        CRenderBufferGL::renderbufferStorage(CTextureGL::internalFormat(ERenderBufferImageFormatGL[toEnumType(CRenderBuffer::getFormat())],
            ERenderBufferImageTypeGL[toEnumType(CRenderBuffer::getType())]), CRenderBuffer::getSize(), m_samplesCount);
    }

    return false;
}

void CRenderBufferGL::unbind()
{
    if (s_currentBuffer != 0)
    {
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        s_currentBuffer = 0;
    }
}

u32 CRenderBufferGL::getRenderBufferID() const
{
    return m_renderBufferID;
}

void CRenderBufferGL::renderbufferStorage(u32 internalFormat, const Dimension2D& size, u32 samplers)
{
    if (samplers > 0)
    {
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, samplers, internalFormat, size.width, size.height);
    }
    else
    {
        glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, size.width, size.height);
    }

    m_changed = false;

    RENDERER->checkForErrors("CRenderBufferGL::renderbufferStorage Error");
}

} //namespace gl
} //namespace renderer
} //namespace v3d

#endif //_OPENGL_DRIVER_
