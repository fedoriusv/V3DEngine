#include "BufferGL.h"
#include "utils/Logger.h"
#include "Engine.h"

#ifdef _OPENGL_DRIVER_
#include "GL/glew.h"

namespace v3d
{
namespace renderer
{
namespace gl
{

GLenum EBufferTargetGL[EBufferTarget::eBufferTargetCount] =
{
    GL_ARRAY_BUFFER,
    GL_ELEMENT_ARRAY_BUFFER,
    GL_TRANSFORM_FEEDBACK_BUFFER,
    GL_TEXTURE_BUFFER
};

GLenum EDataUsageTypeGL[EDataUsageType::eDataUsageTypeCount] =
{
    GL_STATIC_DRAW,
    GL_DYNAMIC_DRAW,
    GL_STATIC_READ,
    GL_DYNAMIC_READ,
    GL_STATIC_COPY,
    GL_DYNAMIC_COPY
};

u32 BufferGL::s_currentBuffer[EBufferTarget::eBufferTargetCount] = { 0 };

BufferGL::BufferGL(EBufferTarget target)
    : Buffer(target)
    , m_bufferID(0)
    , m_lock(false)
{
    LOG_DEBUG("BufferGL: BufferGL constructor %x", this);

    glGenBuffers(1, &m_bufferID);
}

BufferGL::~BufferGL()
{
    ASSERT(!m_lock, "Map buffer used");

    BufferGL::unbind();
    if (m_bufferID != 0)
    {
#ifdef _DEBUG_GL
        ASSERT(glIsBuffer(m_bufferID), "Invalid Index Buffer");
#endif //_DEBUG_GL
        glDeleteBuffers(1, &m_bufferID);
        m_bufferID = 0;
    }

    ASSERT(m_bufferID == 0, "IAO doesn't deleted");
    LOG_DEBUG("BufferGL: BufferGL destructor %x", this);
}

void BufferGL::bind() const
{
    if (s_currentBuffer[m_target] != m_bufferID)
    {
        glBindBuffer(EBufferTargetGL[m_target], m_bufferID);
#ifdef _DEBUG_GL
        ASSERT(glIsBuffer(m_bufferID), "Invalid VBO index");
#endif //_DEBUG_GL
        s_currentBuffer[m_target] = m_bufferID;
    }
}

void BufferGL::bindToTarget(EBufferTarget target, u32 offset, u32 size) const
{
    BufferGL::bind();
    if (size == 0 && offset == 0)
    {
        glBindBufferBase(EBufferTargetGL[target], 0, m_bufferID);
    }
    else
    {
        glBindBufferRange(EBufferTargetGL[target], 0, m_bufferID, size, offset);
    }

    RENDERER->checkForErrors("BufferGL::bindToBuffer Error");
}

void BufferGL::unbind() const
{
    if (s_currentBuffer[m_target] != 0)
    {
        glBindBuffer(EBufferTargetGL[m_target], 0);
        s_currentBuffer[m_target] = 0;
    }
}

void BufferGL::set(EDataUsageType type, u32 size, const void* data)
{
    if (m_lock)
    {
        return;
    }

    BufferGL::bind();
    glBufferData(EBufferTargetGL[m_target], size, data, EDataUsageTypeGL[type]);

    RENDERER->checkForErrors("BufferGL::setData Error");
}

void BufferGL::update(u32 offset, u32 size, const void* data)
{
    if (m_lock)
    {
        return;
    }

    BufferGL::bind();
    glBufferSubData(EBufferTargetGL[m_target], offset, size, data);

    RENDERER->checkForErrors("BufferGL::updateData Error");
}

void BufferGL::read(u32 offset, u32 size, void* data) const
{
    if (m_lock)
    {
        return;
    }

    BufferGL::bind();
#ifdef _DEBUG_GL
    GLint sizeBuff = 0;
    glGetBufferParameteriv(EBufferTargetGL[m_target], GL_BUFFER_SIZE, &sizeBuff);
    ASSERT(sizeBuff >= (s32)size, "Get buffer size out the range");
#endif //_DEBUG_GL
    glGetBufferSubData(EBufferTargetGL[m_target], offset, size, data);

    RENDERER->checkForErrors("BufferGL::readData Error");
}

void* BufferGL::map(u32 access)
{
    BufferGL::bind();

    m_lock = true;
    return glMapBuffer(EBufferTargetGL[m_target], access);
}

bool BufferGL::unmap()
{
    m_lock = false;
    return glUnmapBuffer(EBufferTargetGL[m_target]) != 0;
}

u32 BufferGL::getID() const
{
    return m_bufferID;
}

} //namespace gl
} //namespace renderer
} //namespace v3d

#endif //_OPENGL_DRIVER_
