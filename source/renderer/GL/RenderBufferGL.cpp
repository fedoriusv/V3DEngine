#include "RenderBufferGL.h"
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

GLenum ERenderBufferImageFormatGL[EImageFormat::eFormatCount] =
{
    GL_RED,
    GL_RG,
    GL_RGB,
    GL_RGBA,
    GL_BGR,
    GL_BGRA,
    GL_DEPTH_COMPONENT,
};

GLenum ERenderBufferImageTypeGL[EImageType::eImageTypeCount] =
{
    GL_BYTE,
    GL_UNSIGNED_BYTE,
    GL_SHORT,
    GL_UNSIGNED_SHORT,
    GL_UNSIGNED_SHORT_5_6_5,
    GL_UNSIGNED_SHORT_4_4_4_4,
    GL_INT,
    GL_UNSIGNED_INT,
    GL_FLOAT
};

u32 CRenderBufferGL::s_currentBuffer = 0;

u32 CRenderBufferGL::internalFormat(u32 format, u32 type)
{
    switch (type)
    {
        //BYTE
        case GL_BYTE:
        {
            switch (format)
            {
            case GL_R:
            case GL_RED:
                return GL_R8_SNORM;
            case GL_RG:
                return GL_RG8_SNORM;
            case GL_RGB:
            case GL_BGR:
                return GL_RGB8_SNORM;
            case GL_RGBA:
            case GL_BGRA:
                return GL_RGBA8_SNORM;
            case GL_STENCIL:
                return GL_STENCIL_INDEX8;
            default:
                return format;
            };
        }

        //UNSIGNED_BYTE
        case GL_UNSIGNED_BYTE:
        {
            switch (format)
            {
            case GL_R:
            case GL_RED:
                return GL_R8;
            case GL_RG:
                return GL_RG8;
            case GL_RGB:
            case GL_BGR:
                return GL_RGB8;
            case GL_RGBA:
            case GL_BGRA:
                return GL_RGBA8;
            case GL_STENCIL:
                return GL_STENCIL_INDEX8;
            default:
                return format;
            };
        }

        //SHORT
        case GL_SHORT:
        {
            switch (format)
            {
            case GL_R:
            case GL_RED:
                return GL_R16I;
            case GL_RG:
                return GL_RG16I;
            case GL_RGB:
            case GL_BGR:
                return GL_RGB16I;
            case GL_RGBA:
            case GL_BGRA:
                return GL_RGBA16I;
            case GL_DEPTH_COMPONENT:
                return GL_DEPTH_COMPONENT16;
            case GL_STENCIL:
                return GL_STENCIL_INDEX16;
            default:
                return format;
            };
        };

        //UNSIGNED_SHORT
        case GL_UNSIGNED_SHORT:
        {
            switch (format)
            {
            case GL_R:
            case GL_RED:
                return GL_R16UI;
            case GL_RG:
                return GL_RG16UI;
            case GL_RGB:
            case GL_BGR:
                return GL_RGB16UI;
            case GL_RGBA:
            case GL_BGRA:
                return GL_RGBA16UI;
            case GL_DEPTH_COMPONENT:
                return GL_DEPTH_COMPONENT16;
            case GL_STENCIL:
                return GL_STENCIL_INDEX16;
            default:
                return format;
            };
        }

        //UNSIGNED_SHORT_5_6_5
        case GL_UNSIGNED_SHORT_5_6_5:
        {
            switch (format)
            {
            case GL_RGB:
            case GL_BGR:
                return GL_RGB565;
            default:
                return format;
            };
        }

        //UNSIGNED_SHORT_4_4_4_4
        case GL_UNSIGNED_SHORT_4_4_4_4:
        {
            switch (format)
            {
            case GL_RGB:
            case GL_BGR:
                return GL_RGB4;
            case GL_RGBA:
            case GL_BGRA:
                return GL_RGBA4;
            default:
                return format;
            };
        }

        //INT
        case GL_INT:
        {
            switch (format)
            {
            case GL_R:
            case GL_RED:
                return GL_R32I;
            case GL_RG:
                return GL_RG32I;
            case GL_RGB:
            case GL_BGR:
                return GL_RGB32I;
            case GL_RGBA:
            case GL_BGRA:
                return GL_RGBA32I;
            case GL_DEPTH_COMPONENT:
                return GL_DEPTH_COMPONENT24;
            default:
                return format;
            };
        }

        //UNSIGNED_INT
        case GL_UNSIGNED_INT:
        {
            switch (format)
            {
            case GL_R:
            case GL_RED:
                return GL_R32UI;
            case GL_RG:
                return GL_RG32UI;
            case GL_RGB:
            case GL_BGR:
                return GL_RGB32UI;
            case GL_RGBA:
            case GL_BGRA:
                return GL_RGBA32UI;
            case GL_DEPTH_COMPONENT:
                return GL_DEPTH_COMPONENT24;
            default:
                return format;
            };
        }

        //FLOAT
        case GL_FLOAT:
        {
            switch (format)
            {
            case GL_R:
            case GL_RED:
                return GL_R32F;
            case GL_RG:
                return GL_RG32F;
            case GL_RGB:
            case GL_BGR:
                return GL_RGB32F;
            case GL_RGBA:
            case GL_BGRA:
                return GL_RGBA32F;
            case GL_DEPTH_COMPONENT:
                return GL_DEPTH_COMPONENT32F;
            default:
                return format;
            };
        }
    };

    return format;
};

CRenderBufferGL::CRenderBufferGL(EImageFormat format, EImageType type, u32 samples)
    : m_id(0)
    , m_samplesCount(samples)
{
    LOG_DEBUG("CRenderBufferGL::CRenderBufferGL constructor %x", this);

    glGenRenderbuffers(1, &m_id);

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
    if (m_id != 0)
    {
#ifdef _DEBUG_GL
        ASSERT(glIsRenderbuffer(m_id), "Invalid Index Buffer");
#endif //_DEBUG_GL
        glDeleteRenderbuffers(1, &m_id);
        m_id = 0;
    }

    ASSERT(m_id == 0, "RBO doesn't deleted");
    LOG_DEBUG("CRenderBufferGL: CRenderBufferGL destructor %x", this);
}

bool CRenderBufferGL::bind()
{
    if (s_currentBuffer != m_id)
    {
        glBindRenderbuffer(GL_RENDERBUFFER, m_id);
        ASSERT((glIsRenderbuffer(m_id) || m_id == 0), "Invalid RBO index");
        s_currentBuffer = m_id;

        if (m_changed)
        {
            CRenderBufferGL::renderbufferStorage(CRenderBufferGL::internalFormat(ERenderBufferImageFormatGL[CRenderBuffer::getFormat()],
                ERenderBufferImageTypeGL[CRenderBuffer::getType()]), CRenderBuffer::getSize(), m_samplesCount);
        }

        return true;
    }

    if (m_changed)
    {
        CRenderBufferGL::renderbufferStorage(CRenderBufferGL::internalFormat(ERenderBufferImageFormatGL[CRenderBuffer::getFormat()],
            ERenderBufferImageTypeGL[CRenderBuffer::getType()]), CRenderBuffer::getSize(), m_samplesCount);
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
    return m_id;
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
