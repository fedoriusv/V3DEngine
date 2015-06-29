#include "GeometryGL.h"

#include "renderer/ShaderAttribute.h"
#include "renderer/ShaderData.h"
#include "utils/Logger.h"
#include "Engine.h"

#include "GL/glew.h"

#pragma clang diagnostic ignored "-Wswitch"

using namespace v3d;
using namespace renderer;

GLenum EDrawModeGL[CGeometry::eDrawCount] =
{
    GL_TRIANGLES,
    GL_TRIANGLE_STRIP,
    GL_TRIANGLE_FAN,
    GL_LINES,
    GL_LINE_STRIP,
    GL_LINE_LOOP,
    GL_POINTS

};

GLenum EGeometryTargetGL[CGeometry::eTargetCount] =
{
    GL_ARRAY_BUFFER,
    GL_ELEMENT_ARRAY_BUFFER,
};

u32 CGeometryGL::s_currentArray = 0;
u32 CGeometryGL::s_currentBuffer[eTargetCount] = { 0 };

CGeometryGL::CGeometryGL(const RenderTechniquePtr& technique)
    : CGeometry(technique)
    , m_arrayId(0)
{
}

CGeometryGL::~CGeometryGL()
{
}

void CGeometryGL::init()
{
    if (m_data.empty())
    {
        LOG_WARNING("CGeometryGL: Data empty");
        return;
    }

    CGeometryGL::genVertexArray(m_arrayId);
    CGeometryGL::bindVertexArray(m_arrayId);

    m_interval._begin = 0;
    m_interval._count = m_data._countVertices;

    for (u32 idx = 0; idx < m_technique->getRenderPassCount(); ++idx)
    {
        const RenderPassPtr& pass = m_technique->getRenderPass(idx);
        const AttributeList& attributes = pass->getDefaultShaderData()->getAttributeList();

        CGeometryGL::genBuffers(m_data._verticesId);
        CGeometryGL::bindBuffers(eArrayBuffer, m_data._verticesId);

        s32 vertexSize = CGeometryGL::computeVertexSize(attributes);
        CGeometryGL::bufferData(eArrayBuffer, vertexSize, nullptr);

        CGeometryGL::initBufferData(attributes);
    }

    //Indices
    if (m_data._countIndices > 0)
    {
        CGeometryGL::genBuffers(m_data._indicesId);
        CGeometryGL::bindBuffers(eArrayElementBuffer, m_data._indicesId);
        CGeometryGL::bufferData(eArrayElementBuffer, sizeof(GLint)* m_data._countIndices, m_data._indices.data());
    }

    RENDERER->checkForErrors("GeometryGL: Init Error");

    CGeometryGL::bindVertexArray(0);
    CGeometryGL::bindBuffers(eArrayBuffer, 0);
    CGeometryGL::bindBuffers(eArrayElementBuffer, 0);
}

s32 CGeometryGL::computeVertexSize(const AttributeList& attributes)
{
    s32 vertexSize = 0;

    for (const AttributePtr& attr : attributes)
    {
        CShaderAttribute::EShaderAttribute type = attr->getType();

        switch (type)
        {
        case CShaderAttribute::eAttributeVertex:
        case CShaderAttribute::eAttributeNormal:
        case CShaderAttribute::eAttributeBinormal:
        case CShaderAttribute::eAttributeColor:
        case CShaderAttribute::eAttributeTangent:

            vertexSize += sizeof(GLfloat)* m_data._countVertices * 3;
            break;

        case CShaderAttribute::eAttributeTexture0:
        case CShaderAttribute::eAttributeTexture1:
        case CShaderAttribute::eAttributeTexture2:
        case CShaderAttribute::eAttributeTexture3:

            for (v3d::u32 layer = 0; layer < m_data._texCoords.size(); ++layer)
            {
                vertexSize += sizeof(GLfloat)* m_data._countVertices * 2;
            }
            break;

        default:
            break;
        }
    }

    return vertexSize;
}

void CGeometryGL::draw()
{
    CGeometryGL::bindVertexArray(m_arrayId);
    if (m_data._countIndices > 0)
    {
        CGeometryGL::drawElements(m_drawMode, m_data._countIndices);
    }
    else
    {
        CGeometryGL::drawArrays(m_drawMode, m_interval._begin, m_interval._count);
    }
    //CGeometryGL::bindVertexArray(0);

    RENDERER->checkForErrors("GeometryGL: Draw Error");
}

void CGeometryGL::free()
{
    CGeometryGL::deleteVertexArray(m_arrayId);

    CGeometryGL::deleteBuffers(m_data._verticesId);
    CGeometryGL::deleteBuffers(m_data._indicesId);
}

void CGeometryGL::initBufferData(const AttributeList& attributes)
{
    s32 offset = 0;
    s32 size = 0;
    u32 layer = 0;
    for (const AttributePtr& attr : attributes)
    {
        CShaderAttribute::EShaderAttribute type = attr->getType();

        switch (type)
        {
        case CShaderAttribute::eAttributeVertex:
        {
            size = sizeof(GLfloat)* m_data._countVertices * 3;
            CGeometryGL::bufferSubData(eArrayBuffer, offset, size, m_data._vertices.data());
            CGeometryGL::initVertexAttribPointer(CShaderAttribute::eAttributeVertex, 3, false, sizeof(GLfloat)* 3, offset);
            offset += size;
        }
            break;

        case CShaderAttribute::eAttributeNormal:
        {
            size = sizeof(GLfloat)* m_data._countVertices * 3;
            CGeometryGL::bufferSubData(eArrayBuffer, offset, size, m_data._normals.data());
            CGeometryGL::initVertexAttribPointer(CShaderAttribute::eAttributeNormal, 3, true, sizeof(GLfloat)* 3, offset);
            offset += size;
        }
            break;

        case CShaderAttribute::eAttributeColor:
        {
            size = sizeof(GLfloat)* m_data._countVertices * 3;
            CGeometryGL::bufferSubData(eArrayBuffer, offset, size, m_data._colors.data());
            CGeometryGL::initVertexAttribPointer(CShaderAttribute::eAttributeColor, 3, false, sizeof(GLfloat)* 3, offset);
            offset += size;
        }
            break;

        case CShaderAttribute::eAttributeBinormal:
        {
            //TODO: binormal
        }
            break;

        case CShaderAttribute::eAttributeTangent:
        {
            size = sizeof(GLfloat)* m_data._countVertices * 3;
            CGeometryGL::bufferSubData(eArrayBuffer, offset, size, m_data._tangents.data());
            CGeometryGL::initVertexAttribPointer(CShaderAttribute::eAttributeTangent, 3, false, sizeof(GLfloat)* 3, offset);
            offset += size;
        }
            break;

        case CShaderAttribute::eAttributeTexture0:
        case CShaderAttribute::eAttributeTexture1:
        case CShaderAttribute::eAttributeTexture2:
        case CShaderAttribute::eAttributeTexture3:
        {
            size = sizeof(GLfloat)* m_data._countVertices * 2;
            CGeometryGL::bufferSubData(eArrayBuffer, offset, size, m_data._texCoords.at(layer).data());
            CGeometryGL::initVertexAttribPointer(CShaderAttribute::eAttributeTexture0 + layer, 2, false, sizeof(GLfloat)* 2, offset);
            offset += size;
            ++layer;
        }
            break;
        }
    }
}

void CGeometryGL::refresh()
{
    if (m_data.empty())
    {
        LOG_WARNING("CGeometryGL: Data empty");
        return;
    }

    for (u32 idx = 0; idx < m_technique->getRenderPassCount(); ++idx)
    {
        const RenderPassPtr& pass = m_technique->getRenderPass(idx);
        const AttributeList& attributes = pass->getDefaultShaderData()->getAttributeList();

        CGeometryGL::bindBuffers(eArrayBuffer, m_data._verticesId);

        s32 vertexSize = CGeometryGL::computeVertexSize(attributes);
        CGeometryGL::bufferData(eArrayBuffer, vertexSize, nullptr);

        CGeometryGL::initBufferData(attributes);
    }

    //Indices
    if (m_data._countIndices > 0)
    {
        CGeometryGL::bindBuffers(eArrayElementBuffer, m_data._indicesId);
        CGeometryGL::bufferData(eArrayElementBuffer, sizeof(GLint)* m_data._countIndices, nullptr);
        CGeometryGL::bufferSubData(eArrayElementBuffer, 0, sizeof(GLint)* m_data._countIndices, m_data._indices.data());
    }

    RENDERER->checkForErrors("GeometryGL: Refresh Error");

    CGeometryGL::bindBuffers(eArrayBuffer, 0);
    CGeometryGL::bindBuffers(eArrayElementBuffer, 0);
}

void CGeometryGL::genBuffers(u32& buffer)
{
    glGenBuffers(1, &buffer);
    ASSERT(glIsBuffer(buffer) || "Invalid VBO index");
}

void CGeometryGL::bindBuffers(EGeometryTarget target, u32 buffer)
{
    ASSERT(glIsBuffer(buffer) || "Invalid VBO index");
    if (s_currentBuffer[target] != buffer)
    {
        glBindBuffer(EGeometryTargetGL[target], buffer);
        s_currentBuffer[target] = buffer;
    }
}

void CGeometryGL::deleteBuffers(u32& buffer)
{
    if (buffer != 0)
    {
        ASSERT(glIsShader(buffer) || "Invalid Index Buffer");
        glDeleteBuffers(1, &buffer);
    }
}

void CGeometryGL::bufferData(EGeometryTarget target, u32 size, void* data)
{
    glBufferData(EGeometryTargetGL[target], size, data, GL_STATIC_DRAW);
}

void CGeometryGL::bufferSubData(EGeometryTarget target, u32 offset, u32 size, void* data)
{
    glBufferSubData(EGeometryTargetGL[target], offset, size, data);
}

void* CGeometryGL::mapBuffer(EGeometryTarget target, u32 access)
{
    return glMapBuffer(EGeometryTargetGL[target], access);
}

void* CGeometryGL::mapBufferRange(EGeometryTarget target, u32 offset, u32 size, u32 flags)
{
    return glMapBufferRange(EGeometryTargetGL[target], offset, size, flags);
}

bool CGeometryGL::unmapBuffer(EGeometryTarget target)
{
    return glUnmapBuffer(EGeometryTargetGL[target]) != 0;
}

void CGeometryGL::getBufferPointer(EGeometryTarget target, u32 pname, void** params)
{
    glGetBufferPointerv(EGeometryTargetGL[target], pname, params);
}

void CGeometryGL::genVertexArray(u32& buffer)
{
    glGenVertexArrays(1, &buffer);
    ASSERT(glIsVertexArray(buffer) || "Invalid VAO index");
}

void CGeometryGL::bindVertexArray(const u32 buffer)
{
    ASSERT(glIsVertexArray(buffer) || "Invalid VAO index");
    if (s_currentArray != buffer)
    {
        glBindVertexArray(buffer);
        s_currentArray = buffer;
    }
}

void CGeometryGL::deleteVertexArray(u32& buffer)
{
    if (buffer != 0)
    {
        ASSERT(glIsVertexArray(buffer) || "Invalid VAO index");
        glDeleteVertexArrays(1, &buffer);
    }
}

void CGeometryGL::initVertexAttribPointer(u32 vertexAttrib, u32 count, bool normalized, u32 size, u32 offset)
{
    glVertexAttribPointer(vertexAttrib, count, GL_FLOAT, normalized ? GL_TRUE : GL_FALSE, size, (const GLvoid*)offset);
    glEnableVertexAttribArray(vertexAttrib);
}

void CGeometryGL::drawElements(EDrawMode mode, u32 count)
{
    glDrawElements(EDrawModeGL[mode], count, GL_UNSIGNED_INT, NULL);
}

void CGeometryGL::drawElementsInstanced(EDrawMode mode, u32 count, u32 primCount)
{
    glDrawElementsInstanced(EDrawModeGL[mode], count, GL_UNSIGNED_INT, NULL, primCount);
}

void CGeometryGL::drawArrays(EDrawMode mode, u32 first, u32 count)
{
    glDrawArrays(EDrawModeGL[mode], first, count);
}

void CGeometryGL::drawArraysInstanced(EDrawMode mode, u32 first, u32 count, u32 primCount)
{
    glDrawArraysInstanced(EDrawModeGL[mode], first, count, primCount);
}
