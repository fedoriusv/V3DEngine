#include "BufferGL.h"
#include "utils/Logger.h"

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
};

GLenum EDataUsageTypeGL[EDataUsageType::eDataUsageTypeCount] =
{
    GL_STATIC_DRAW,
    GL_DYNAMIC_DRAW,
};

u32 BufferGL::s_currentBuffer[EBufferTarget::eBufferTargetCount] = { 0 };

BufferGL::BufferGL(EBufferTarget target)
    : Buffer(target)
    , m_id(0)
    , m_lock(false)
{
    LOG_DEBUG("BufferGL: BufferGL constructor %x", this);

    glGenBuffers(1, &m_id);
}

BufferGL::~BufferGL()
{
    ASSERT(!m_lock, "Map buffer used");

    BufferGL::unbind();
    if (m_id != 0)
    {
        //TODO: some times return false for shader id
        /*ASSERT(glIsShader(m_id), "Invalid Index Buffer");*/
        glDeleteBuffers(1, &m_id);
        m_id = 0;
    }

    ASSERT(m_id == 0, "IAO doesn't deleted");
    LOG_DEBUG("BufferGL: BufferGL destructor %x", this);
}

void BufferGL::bind()
{
    if (s_currentBuffer[m_target] != m_id)
    {
        glBindBuffer(EBufferTargetGL[m_target], m_id);
        ASSERT(glIsBuffer(m_id), "Invalid VBO index");
        s_currentBuffer[m_target] = m_id;
    }
}

void BufferGL::bindToBuffer(const Buffer* buffer, u32 offset, u32 size)
{
    BufferGL::bind();
    ASSERT(buffer, "Target Buffer nullptr");
    ASSERT(glIsBuffer(static_cast<const BufferGL*>(buffer)->m_id), "Invalid VBO index");

    if (size == 0 && offset == 0)
    {
        glBindBufferBase(EBufferTargetGL[buffer->getTarget()], 0, static_cast<const BufferGL*>(buffer)->m_id);
    }
    else
    {
        glBindBufferRange(EBufferTargetGL[buffer->getTarget()], 0, static_cast<const BufferGL*>(buffer)->m_id, size, offset);
    }
}

void BufferGL::unbind()
{
    if (s_currentBuffer[m_target] != 0)
    {
        glBindBuffer(EBufferTargetGL[m_target], 0);
        s_currentBuffer[m_target] = 0;
    }
}

void BufferGL::setData(EDataUsageType type, u32 size, void* data)
{
    if (m_lock)
    {
        return;
    }

    BufferGL::bind();
    glBufferData(EBufferTargetGL[m_target], size, data, EDataUsageTypeGL[type]);
}

void BufferGL::updateData(u32 offset, u32 size, void* data)
{
    if (m_lock)
    {
        return;
    }

    BufferGL::bind();
    glBufferSubData(EBufferTargetGL[m_target], offset, size, data);
}

void * BufferGL::map(u32 access)
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


} //namespace gl
} //namespace renderer
} //namespace v3d

#endif //_OPENGL_DRIVER_
