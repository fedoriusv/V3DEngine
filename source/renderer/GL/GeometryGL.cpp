#include "GeometryGL.h"
#include "renderer/ShaderAttribute.h"
#include "renderer/ShaderData.h"
#include "utils/Logger.h"
#include "Engine.h"

#ifndef _PLATFORM_WIN_
#   pragma clang diagnostic ignored "-Wswitch"
#endif  //_PLATFORM_WIN_

#ifdef _OPENGL_DRIVER_
#include "GL/glew.h"

using namespace v3d;
using namespace renderer;

GLenum EDrawModeGL[CGeometry::eDrawModeCount] =
{
    GL_TRIANGLES,
    GL_TRIANGLE_STRIP,
    GL_LINES,
    GL_LINE_STRIP,
    GL_POINTS
};

GLenum EGeometryTargetGL[CGeometry::eArrayTargetCount] =
{
    GL_ARRAY_BUFFER,
    GL_ELEMENT_ARRAY_BUFFER,
};

GLenum EGeometryTypeGL[CGeometry::eGeometryTypeCount] =
{
    GL_STATIC_DRAW,
    GL_DYNAMIC_DRAW,
};

u32 CGeometryGL::s_currentArray = 0;
u32 CGeometryGL::s_currentBuffer[eTargetCount] = { 0 };

CGeometryGL::CGeometryGL(const CRenderTechnique* technique)
    : CGeometry(technique)
    , m_arrayId(0U)
    , m_verticesId(0U)
    , m_indicesId(0U)
{
    LOG_DEBUG("CGeometryGL: CGeometryGL constructor %x", this);
}

CGeometryGL::~CGeometryGL()
{
    ASSERT(m_arrayId == 0U && "VAO doesn't deleted");
    ASSERT(m_verticesId == 0U && "VAO doesn't deleted");
    ASSERT(m_indicesId == 0U && "IAO doesn't deleted");

    LOG_DEBUG("CGeometryGL: CGeometryGL destructor %x", this);
}

void CGeometryGL::init()
{
    if (m_data.empty())
    {
        LOG_WARNING("CGeometryGL: Data empty");
        return;
    }

    CGeometry::setInterval(0, m_data.verticesSize());

    CGeometryGL::genVertexArray(m_arrayId);
    CGeometryGL::bindVertexArray(m_arrayId);

    const RenderPassPtr& pass = m_technique->getRenderPass(m_technique->getCurrentPass());
    u32 mask = pass->getDefaultShaderData()->getVertexFormatMask() | pass->getUserShaderData()->getVertexFormatMask();
    CGeometry::setVertexMask(mask);

    CGeometryGL::genBuffers(m_verticesId);
    CGeometryGL::bindBuffers(eArrayBuffer, m_verticesId);

    ShaderDataList shaderData;
    shaderData.push_back(pass->getDefaultShaderData());
    shaderData.push_back(pass->getUserShaderData());

    u32 vertexSize = CGeometryGL::computeVertexSize(shaderData);
    CGeometryGL::bufferData(eArrayBuffer, m_geometyType, vertexSize, nullptr);
    CGeometryGL::initBufferData(shaderData);

    //Indices
    if (m_data.indicesSize() > 0)
    {
        CGeometryGL::genBuffers(m_indicesId);
        CGeometryGL::bindBuffers(eArrayElementBuffer, m_indicesId);
        CGeometryGL::bufferData(eArrayElementBuffer, m_geometyType, sizeof(GLint)* m_data.indicesSize(), m_data._indices.data());
    }

    RENDERER->checkForErrors("GeometryGL: Init Error");

    CGeometryGL::bindVertexArray(0);
    CGeometryGL::bindBuffers(eArrayBuffer, 0);
    CGeometryGL::bindBuffers(eArrayElementBuffer, 0);
}

s32 CGeometryGL::computeVertexSize(const ShaderDataList& shaderDataList)
{
    s32 vertexSize = 0;

    for (auto& shaderData : shaderDataList)
    {
        for (const AttributePair& attr : shaderData.lock()->getAttributeList())
        {
            CShaderAttribute::EShaderAttribute type = attr.second->getType();

            switch (type)
            {
            case CShaderAttribute::eAttributePosition:
            case CShaderAttribute::eAttributeNormal:
            case CShaderAttribute::eAttributeBinormal:
            case CShaderAttribute::eAttributeColor:
            case CShaderAttribute::eAttributeTangent:

                vertexSize += sizeof(GLfloat)* m_data.verticesSize() * 3;
                break;

            case CShaderAttribute::eAttributeTexture0:
            case CShaderAttribute::eAttributeTexture1:
            case CShaderAttribute::eAttributeTexture2:
            case CShaderAttribute::eAttributeTexture3:

                for (v3d::u32 layer = 0; layer < m_data._texCoords.size(); ++layer)
                {
                    vertexSize += sizeof(GLfloat)* m_data.verticesSize() * 2;
                }
                break;

            case CShaderAttribute::eAttributeUser:

                if (attr.second->getUserDataSize() > 0 && attr.second->getUserDataCount() > 0)
                {
                    vertexSize += attr.second->getUserDataSize() * attr.second->getUserDataCount();
                }
                break;

            default:
                break;
            }
        }
    }

    return vertexSize;
}

void CGeometryGL::draw()
{
    CGeometry::draw();

    CGeometryGL::bindVertexArray(m_arrayId);

    u32 passIdx = m_technique->getCurrentPass();
    const RenderPassPtr& pass = m_technique->getRenderPass(passIdx);
    const AttributeList& attributesDefault = pass->getDefaultShaderData()->getAttributeList();
    const AttributeList& attributesUser = pass->getUserShaderData()->getAttributeList();

    for (const AttributePair& attr : attributesDefault)
    {
        CShaderAttribute::EShaderAttribute type = attr.second->getType();
        CGeometryGL::vertexAttribArray(type, true);
    }

    for (const AttributePair& attr : attributesUser)
    {
        u32 id = attr.second->getID();
        CGeometryGL::vertexAttribArray(id, true);
    }

    u32 instanceAmount = pass->getRenderAdvanced()->getCountInstance();
    if (m_data.indicesSize() > 0)
    {
        CGeometryGL::drawElements(m_drawMode, m_data.indicesSize(), instanceAmount);
    }
    else
    {
        CGeometryGL::drawArrays(m_drawMode, CGeometry::getInterval()._begin, CGeometry::getInterval()._count, instanceAmount);
    }

    for (const AttributePair& attr : attributesDefault)
    {
        CShaderAttribute::EShaderAttribute type = attr.second->getType();
        CGeometryGL::vertexAttribArray(type, false);
    }

    for (const AttributePair& attr : attributesUser)
    {
        u32 id = attr.second->getID();
        CGeometryGL::vertexAttribArray(id, false);
    }

    CGeometryGL::bindVertexArray(0);

    RENDERER->checkForErrors("GeometryGL: Draw Error");
}

void CGeometryGL::free()
{
    CGeometryGL::deleteVertexArray(m_arrayId);

    CGeometryGL::deleteBuffers(m_verticesId);
    CGeometryGL::deleteBuffers(m_indicesId);
}

void CGeometryGL::initBufferData(const ShaderDataList& shaderDataList)
{
    s32 offset = 0;
    s32 size = 0;
    u32 layer = 0;

    for (auto& shaderData : shaderDataList)
    {
        for (const AttributePair& attr : shaderData.lock()->getAttributeList())
        {
            CShaderAttribute::EShaderAttribute type = attr.second->getType();

            switch (type)
            {
                case CShaderAttribute::eAttributePosition:
                {
                    size = sizeof(GLfloat)* m_data.verticesSize() * 3;
                    CGeometryGL::bufferSubData(eArrayBuffer, offset, size, m_data._vertices.data());
                    CGeometryGL::initVertexAttribPointer(CShaderAttribute::eAttributePosition, 3, false, sizeof(GLfloat) * 3, offset);
                    offset += size;
                }
                break;

            case CShaderAttribute::eAttributeNormal:
                {
                    size = sizeof(GLfloat)* m_data.verticesSize() * 3;
                    CGeometryGL::bufferSubData(eArrayBuffer, offset, size, m_data._normals.data());
                    CGeometryGL::initVertexAttribPointer(CShaderAttribute::eAttributeNormal, 3, true, sizeof(GLfloat) * 3, offset);
                    offset += size;
                }
                break;

            case CShaderAttribute::eAttributeColor:
                {
                    size = sizeof(GLfloat)* m_data.verticesSize() * 3;
                    CGeometryGL::bufferSubData(eArrayBuffer, offset, size, m_data._colors.data());
                    CGeometryGL::initVertexAttribPointer(CShaderAttribute::eAttributeColor, 3, false, sizeof(GLfloat) * 3, offset);
                    offset += size;
                }
                break;

            case CShaderAttribute::eAttributeBinormal:
                {
                    size = sizeof(GLfloat)* m_data.verticesSize() * 3;
                    CGeometryGL::bufferSubData(eArrayBuffer, offset, size, m_data._binormals.data());
                    CGeometryGL::initVertexAttribPointer(CShaderAttribute::eAttributeBinormal, 3, false, sizeof(GLfloat) * 3, offset);
                    offset += size;
                }
                break;

            case CShaderAttribute::eAttributeTangent:
                {
                    size = sizeof(GLfloat)* m_data.verticesSize() * 3;
                    CGeometryGL::bufferSubData(eArrayBuffer, offset, size, m_data._tangents.data());
                    CGeometryGL::initVertexAttribPointer(CShaderAttribute::eAttributeTangent, 3, false, sizeof(GLfloat) * 3, offset);
                    offset += size;
                }
                break;

            case CShaderAttribute::eAttributeTexture0:
            case CShaderAttribute::eAttributeTexture1:
            case CShaderAttribute::eAttributeTexture2:
            case CShaderAttribute::eAttributeTexture3:
                {
                    size = sizeof(GLfloat)* m_data.verticesSize() * 2;
                    CGeometryGL::bufferSubData(eArrayBuffer, offset, size, m_data._texCoords.at(layer).data());
                    CGeometryGL::initVertexAttribPointer(CShaderAttribute::eAttributeTexture0 + layer, 2, false, sizeof(GLfloat) * 2, offset);
                    offset += size;
                    ++layer;
                }
                break;

            case CShaderAttribute::eAttributeUser:
                {
                    if (attr.second->getUserDataCount() <= 0)
                    {
                        LOG_WARNING("CGeometryGL::initBufferData: User data have diferent size [%d]", attr.second->getUserDataCount());
                    }

                    size = attr.second->getUserDataSize() * attr.second->getUserDataCount();
                    if (size > 0)
                    {
                        CGeometryGL::bufferSubData(eArrayBuffer, offset, size, attr.second->getUserData());

                        u32 userLayer = attr.second->getID();
                        u32 componentsCount = attr.second->getUserDataSize() / sizeof(GLfloat); //TODO: need rework
                        CGeometryGL::initVertexAttribPointer(userLayer, componentsCount, false, attr.second->getUserDataSize(), offset);
                        offset += size;

                        u32 divisor = attr.second->getDivisor();
                        if (divisor > 0)
                        {
                            CGeometryGL::vertexAttribDivisior(userLayer, 1);
                        }
                    }
                }
                break;

            default:
                break;
            }
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

    u32 passIdx = m_technique->getCurrentPass();
    const RenderPassPtr& pass = m_technique->getRenderPass(passIdx);

    CGeometryGL::bindVertexArray(m_arrayId);
    CGeometryGL::bindBuffers(eArrayBuffer, m_verticesId);

    ShaderDataList shaderData;
    shaderData.push_back(pass->getDefaultShaderData());
    shaderData.push_back(pass->getUserShaderData());

    s32 vertexSize = CGeometryGL::computeVertexSize(shaderData);
    CGeometryGL::bufferData(eArrayBuffer, m_geometyType, vertexSize, nullptr);
    CGeometryGL::initBufferData(shaderData);

    //Indices
    if (m_data.indicesSize() > 0)
    {
        CGeometryGL::bindBuffers(eArrayElementBuffer, m_indicesId);
        CGeometryGL::bufferData(eArrayElementBuffer, m_geometyType, sizeof(GLint)* m_data.indicesSize(), nullptr);
        CGeometryGL::bufferSubData(eArrayElementBuffer, 0, sizeof(GLint)* m_data.indicesSize(), m_data._indices.data());
    }

    RENDERER->checkForErrors("GeometryGL: Refresh Error");

    CGeometryGL::bindVertexArray(0);
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
        buffer = 0;
    }
}

void CGeometryGL::bufferData(EGeometryTarget target, EGeomertyType type, u32 size, void* data)
{
    glBufferData(EGeometryTargetGL[target], size, data, EGeometryTypeGL[type]);
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
        buffer = 0;
    }
}

void CGeometryGL::initVertexAttribPointer(u32 vertexAttrib, u32 count, bool normalized, u32 size, u32 offset)
{
    glVertexAttribPointer(vertexAttrib, count, GL_FLOAT, normalized ? GL_TRUE : GL_FALSE, size, (const GLvoid*)offset);
}

void CGeometryGL::vertexAttribArray(u32 vertexAttrib, bool enable)
{
    if (enable)
    {
        glEnableVertexAttribArray(vertexAttrib);
    }
    else
    {
        glDisableVertexAttribArray(vertexAttrib);
    }
}

void CGeometryGL::vertexAttribDivisior(u32 vertexAttrib, u32 value)
{
    glVertexAttribDivisor(vertexAttrib, value);
}

void CGeometryGL::drawElements(EDrawMode mode, u32 count, u32 primCount)
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

void CGeometryGL::drawArrays(EDrawMode mode, u32 first, u32 count, u32 primCount)
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

#endif //_OPENGL_DRIVER_
