#include "BufferGL.h"

#ifdef _OPENGL_DRIVER_
#include "GL/glew.h"

namespace v3d
{
namespace renderer
{
namespace gl
{

GLenum EDrawModeGL[EDrawMode::eDrawModeCount] =
{
    GL_TRIANGLES,
    GL_TRIANGLE_STRIP,
    GL_LINES,
    GL_LINE_STRIP,
    GL_POINTS
};

GLenum EGeometryTargetGL[EGeometryTarget::eArrayTargetCount] =
{
    GL_ARRAY_BUFFER,
    GL_ELEMENT_ARRAY_BUFFER,
    GL_TRANSFORM_FEEDBACK_BUFFER,
};

GLenum EGeometryTypeGL[EGeomertyType::eGeometryTypeCount] =
{
    GL_STATIC_DRAW,
    GL_DYNAMIC_DRAW,
};

u32 BufferGL::s_currentArray = 0;
u32 BufferGL::s_currentBuffer[eTargetCount] = { 0 };

void BufferGL::genBuffer(u32& buffer)
{
    glGenBuffers(1, &buffer);
}

void BufferGL::bindBuffer(EGeometryTarget target, u32 buffer)
{
    if (s_currentBuffer[target] != buffer)
    {
        glBindBuffer(EGeometryTargetGL[target], buffer);
        ASSERT(glIsBuffer(buffer) || buffer == 0, "Invalid VBO index");
        s_currentBuffer[target] = buffer;
    }
}

void BufferGL::bindBufferTarget(EGeometryTarget target, u32 buffer, u32 size, u32 offset)
{
    ASSERT(EGeometryTargetGL[target] == GL_TRANSFORM_FEEDBACK_BUFFER, "Invalid GL Target");
    if (size == 0U && offset == 0U)
    {
        glBindBufferBase(EGeometryTargetGL[target], 0, buffer);
    }
    else
    {
        glBindBufferRange(EGeometryTargetGL[target], 0, buffer, size, offset);
    }
    ASSERT(glIsBuffer(buffer), "Invalid VBO index");
}

void BufferGL::deleteBuffer(u32& buffer)
{
    if (buffer != 0)
    {
        //TODO: some times return false for shader id
        /*ASSERT(glIsShader(buffer), "Invalid Index Buffer");*/
        glDeleteBuffers(1, &buffer);
        buffer = 0;
    }
}

void BufferGL::bufferData(EGeometryTarget target, EGeomertyType type, u32 size, void* data)
{
    glBufferData(EGeometryTargetGL[target], size, data, EGeometryTypeGL[type]);
}

void BufferGL::bufferSubData(EGeometryTarget target, u32 offset, u32 size, void* data)
{
    glBufferSubData(EGeometryTargetGL[target], offset, size, data);
}

void* BufferGL::mapBuffer(EGeometryTarget target, u32 access)
{
    return glMapBuffer(EGeometryTargetGL[target], access);
}

void* BufferGL::mapBufferRange(EGeometryTarget target, u32 offset, u32 size, u32 flags)
{
    return glMapBufferRange(EGeometryTargetGL[target], offset, size, flags);
}

bool BufferGL::unmapBuffer(EGeometryTarget target)
{
    return glUnmapBuffer(EGeometryTargetGL[target]) != 0;
}

void BufferGL::getBufferPointer(EGeometryTarget target, u32 pname, void** params)
{
    glGetBufferPointerv(EGeometryTargetGL[target], pname, params);
}

void BufferGL::genVertexArray(u32& buffer)
{
    glGenVertexArrays(1, &buffer);
}

void BufferGL::bindVertexArray(u32 buffer)
{
    if (s_currentArray != buffer)
    {
        glBindVertexArray(buffer);
        ASSERT(glIsVertexArray(buffer) || buffer == 0, "Invalid VAO index");
        s_currentArray = buffer;
    }
}

void BufferGL::deleteVertexArray(u32& buffer)
{
    if (buffer != 0)
    {
        ASSERT(glIsVertexArray(buffer), "Invalid VAO index");
        glDeleteVertexArrays(1, &buffer);
        buffer = 0;
    }
}

void BufferGL::initVertexAttribPointer(u32 attrib, EDataType type, u32 count, u32 size, u32 offset)
{
    std::function<u32(EDataType)> format = [](EDataType type) -> u32
    {
        switch (type)
        {
        case EDataType::eTypeInt:
            return GL_INT;

        case EDataType::eTypeFloat:
        case EDataType::eTypeVector2:
        case EDataType::eTypeVector3:
        case EDataType::eTypeVector4:
        case EDataType::eTypeMatrix3:
        case EDataType::eTypeMatrix4:
            return GL_FLOAT;

        case EDataType::ETypeDouble:
            return GL_DOUBLE;

        default:
            return GL_FLOAT;
        }
    };

    glVertexAttribPointer(attrib, count, format(type), GL_FALSE, size, (const GLvoid*)offset);
}

void BufferGL::vertexAttribArray(u32 attrib, bool enable)
{
    if (enable)
    {
        glEnableVertexAttribArray(attrib);
    }
    else
    {
        glDisableVertexAttribArray(attrib);
    }
}

void BufferGL::vertexAttribDivisior(u32 attrib, u32 value)
{
    glVertexAttribDivisor(attrib, value);
}

void BufferGL::drawElements(EDrawMode mode, u32 count, u32 primCount)
{
    if (primCount > 0)
    {
        glDrawElementsInstanced(EDrawModeGL[mode], count, GL_UNSIGNED_INT, NULL, primCount);
    }
    else
    {
        glDrawElements(EDrawModeGL[mode], count, GL_UNSIGNED_INT, NULL);
    }
}

void BufferGL::drawArrays(EDrawMode mode, u32 first, u32 count, u32 primCount)
{
    if (primCount > 0)
    {
        glDrawArraysInstanced(EDrawModeGL[mode], first, count, primCount);
    }
    else
    {
        glDrawArrays(EDrawModeGL[mode], first, count);
    }
}

} //namespace gl
} //namespace renderer
} //namespace v3d

#endif //_OPENGL_DRIVER_
