#include "GeometryGL.h"
#include "renderer/ShaderAttribute.h"
#include "renderer/ShaderData.h"
#include "utils/Logger.h"
#include "Engine.h"

#ifndef _PLATFORM_WIN_
#   pragma clang diagnostic ignored "-Wswitch"
#endif  //_PLATFORM_WIN_

#ifdef _OPENGL_DRIVER_
#include "BufferGL.h"
#include "GL/glew.h"

namespace v3d
{
namespace renderer
{
namespace gl
{

const GLenum EPrimitivesModeGL[EPrimitivesMode::ePrimitivesModeCount] =
{
    GL_TRIANGLES,
    GL_TRIANGLE_STRIP,
    GL_LINES,
    GL_LINE_STRIP,
    GL_POINTS
};

u32 CGeometryGL::s_currentArray = 0;

CGeometryGL::CGeometryGL(const CRenderTechnique* technique)
    : CGeometry(technique)
    , m_arrayId(0U)

    , m_initialized(false)
{
    LOG_DEBUG("CGeometryGL: CGeometryGL constructor %x", this);
}

CGeometryGL::~CGeometryGL()
{
    ASSERT(m_arrayId == 0, "VAO doesn't deleted");
    ASSERT(!m_vertexBuffer, "Buffer doesn't deleted");
    ASSERT(!m_indexBuffer, "Buffer doesn't deleted");
    LOG_DEBUG("CGeometryGL: CGeometryGL destructor %x", this);
}

void CGeometryGL::init()
{
    if (m_initialized)
    {
        return;
    }

    if (m_data.empty())
    {
        LOG_WARNING("CGeometryGL: Data empty");
        return;
    }

    CGeometry::setInterval(0, m_data.verticesSize());

    const RenderPassPtr& pass = m_technique->getRenderPass(m_technique->getCurrentPass());
    u32 mask = pass->getDefaultShaderData()->getVertexFormatMask() | pass->getUserShaderData()->getVertexFormatMask();
    CGeometry::setVertexMask(mask);

    ShaderDataList shaderData;
    shaderData.push_back(pass->getDefaultShaderData());
    shaderData.push_back(pass->getUserShaderData());

    //Array
    CGeometryGL::genVertexArray(m_arrayId);
    CGeometryGL::bindVertexArray(m_arrayId);

    //Vertices
    u32 vertexSize = CGeometryGL::computeVertexSize(shaderData);
    m_vertexBuffer = new BufferGL(eVertexBuffer);
    m_vertexBuffer->set(m_geometyType, vertexSize, nullptr);
    CGeometryGL::initBufferData(shaderData);

    //Indices
    if (m_data.indicesSize() > 0)
    {
        m_indexBuffer = new BufferGL(eIndexBuffer);
        m_indexBuffer->set(m_geometyType, sizeof(GLint)* m_data.indicesSize(), m_data._indices.data());
    }

    RENDERER->checkForErrors("GeometryGL: Init Error");

    CGeometryGL::bindVertexArray(0);

    m_instanceAmount = pass->getRenderAdvanced()->getCountInstance();

    if (pass->getRenderAdvanced()->getPrimitiveMode() != EPrimitivesMode::ePrimitivesNone)
    {
        m_drawMode = pass->getRenderAdvanced()->getPrimitiveMode();
    }

    const CGeometry::SInterval& interval = pass->getRenderAdvanced()->getDrawInterval();
    if (interval._begin != 0 || interval._count != 0)
    {
        CGeometry::setInterval(interval._begin, interval._count);
    }

    m_initialized = true;
}

s32 CGeometryGL::computeVertexSize(const ShaderDataList& shaderDataList)
{
    s32 vertexSize = 0;

    for (auto& shaderData : shaderDataList)
    {
        for (const AttributePair& attr : shaderData.lock()->getAttributeList())
        {
            CShaderAttribute::EShaderAttribute attribute = attr.second->getData();

            switch (attribute)
            {
            case CShaderAttribute::eAttribVertexPosition:
            case CShaderAttribute::eAttribVertexNormal:
            case CShaderAttribute::eAttribVertexBinormal:
            case CShaderAttribute::eAttribVertexColor:
            case CShaderAttribute::eAttribVertexTangent:
            case CShaderAttribute::eAttribParticalPosition:
            case CShaderAttribute::eAttribParticalColor:
            case CShaderAttribute::eAttribParticalVelocity:

                vertexSize += sizeof(GLfloat)* m_data.verticesSize() * 3;
                break;

            case CShaderAttribute::eAttribVertexTexture0:
            case CShaderAttribute::eAttribVertexTexture1:
            case CShaderAttribute::eAttribVertexTexture2:
            case CShaderAttribute::eAttribVertexTexture3:

                for (v3d::u32 layer = 0; layer < m_data._texCoords.size(); ++layer)
                {
                    vertexSize += sizeof(GLfloat)* m_data.verticesSize() * 2;
                }
                break;

            case CShaderAttribute::eAttribParticalSize:
            case CShaderAttribute::eAttribParticalLifeTime:

                vertexSize += sizeof(GLfloat)* m_data.verticesSize();
                break;

            case CShaderAttribute::eAttribParticalType:

                vertexSize += sizeof(GLint)* m_data.verticesSize();
                break;

            case CShaderAttribute::eAttribUser:

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
    if (!m_initialized)
    {
        return;
    }

    u32 passIdx = m_technique->getCurrentPass();
    const RenderPassPtr& pass = m_technique->getRenderPass(passIdx);

    CGeometry::draw();

    CGeometryGL::bindVertexArray(m_arrayId);

    const AttributeList& attributesDefault = pass->getDefaultShaderData()->getAttributeList();
    for (const AttributePair& attr : attributesDefault)
    {
        s32 attribute = attr.second->getID();
        CGeometryGL::vertexAttribArray(attribute, true);
    }

    const AttributeList& attributesUser = pass->getUserShaderData()->getAttributeList();
    for (const AttributePair& attr : attributesUser)
    {
        s32 attribute = attr.second->getID();
        CGeometryGL::vertexAttribArray(attribute, true);
    }

    if (m_data.indicesSize() > 0)
    {
        CGeometryGL::drawElements(m_drawMode, m_data.indicesSize(), m_instanceAmount);
    }
    else
    {
        CGeometryGL::drawArrays(m_drawMode, CGeometry::getInterval()._begin, CGeometry::getInterval()._count, m_instanceAmount);
    }

    for (const AttributePair& attr : attributesDefault)
    {
        s32 attribute = attr.second->getID();
        CGeometryGL::vertexAttribArray(attribute, false);
    }

    for (const AttributePair& attr : attributesUser)
    {
        s32 attribute = attr.second->getID();
        CGeometryGL::vertexAttribArray(attribute, false);
    }

    CGeometryGL::bindVertexArray(0);

    RENDERER->checkForErrors("GeometryGL: Draw Error");
}

void CGeometryGL::free()
{
    if (!m_initialized)
    {
        return;
    }

    CGeometryGL::bindVertexArray(0);

    if (m_vertexBuffer)
    {
        delete m_vertexBuffer;
        m_vertexBuffer = nullptr;
    }

    if (m_indexBuffer)
    {
        delete m_indexBuffer;
        m_indexBuffer = nullptr;
    }

    CGeometryGL::deleteVertexArray(m_arrayId);

    RENDERER->checkForErrors("GeometryGL: Delete buffers Error");
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
            CShaderAttribute::EShaderAttribute attribute = attr.second->getData();

            switch (attribute)
            {
                case CShaderAttribute::eAttribVertexPosition:
                {
                    size = sizeof(GLfloat)* m_data.verticesSize() * 3;
                    m_vertexBuffer->update(offset, size, m_data._vertices.data());
                    CGeometryGL::initVertexAttribPointer(attr.second->getID(), EDataType::eTypeVector3, 3, sizeof(GLfloat) * 3, offset);
                    offset += size;

                    RENDERER->checkForErrors("GeometryGL: init eAttribVertexPosition Error");
                }
                break;

                case CShaderAttribute::eAttribVertexNormal:
                {
                    size = sizeof(GLfloat)* m_data.verticesSize() * 3;
                    m_vertexBuffer->update(offset, size, m_data._normals.data());
                    CGeometryGL::initVertexAttribPointer(attr.second->getID(), EDataType::eTypeVector3, 3, sizeof(GLfloat) * 3, offset);
                    offset += size;

                    RENDERER->checkForErrors("GeometryGL: init eAttribVertexNormal Error");
                }
                break;

                case CShaderAttribute::eAttribVertexColor:
                {
                    size = sizeof(GLfloat)* m_data.verticesSize() * 3;
                    m_vertexBuffer->update(offset, size, m_data._colors.data());
                    CGeometryGL::initVertexAttribPointer(attr.second->getID(), EDataType::eTypeVector3, 3, sizeof(GLfloat) * 3, offset);
                    offset += size;

                    RENDERER->checkForErrors("GeometryGL: init eAttribVertexColor Error");
                }
                break;

                case CShaderAttribute::eAttribVertexBinormal:
                {
                    size = sizeof(GLfloat)* m_data.verticesSize() * 3;
                    m_vertexBuffer->update(offset, size, m_data._binormals.data());
                    CGeometryGL::initVertexAttribPointer(attr.second->getID(), EDataType::eTypeVector3, 3, sizeof(GLfloat) * 3, offset);
                    offset += size;

                    RENDERER->checkForErrors("GeometryGL: init eAttribVertexBinormal Error");
                }
                break;

                case CShaderAttribute::eAttribVertexTangent:
                {
                    size = sizeof(GLfloat)* m_data.verticesSize() * 3;
                    m_vertexBuffer->update(offset, size, m_data._tangents.data());
                    CGeometryGL::initVertexAttribPointer(attr.second->getID(), EDataType::eTypeVector3, 3, sizeof(GLfloat) * 3, offset);
                    offset += size;

                    RENDERER->checkForErrors("GeometryGL: init eAttribVertexTangent Error");
                }
                break;

                case CShaderAttribute::eAttribVertexTexture0:
                case CShaderAttribute::eAttribVertexTexture1:
                case CShaderAttribute::eAttribVertexTexture2:
                case CShaderAttribute::eAttribVertexTexture3:
                {
                    size = sizeof(GLfloat)* m_data.verticesSize() * 2;
                    m_vertexBuffer->update(offset, size, m_data._texCoords.at(layer).data());
                    CGeometryGL::initVertexAttribPointer(attr.second->getID(), EDataType::eTypeVector2, 2, sizeof(GLfloat) * 2, offset);
                    offset += size;
                    ++layer;

                    RENDERER->checkForErrors("GeometryGL: init eAttribVertexTexture Error");
                }
                break;

                case CShaderAttribute::eAttribParticalPosition:
                case CShaderAttribute::eAttribParticalColor:
                case CShaderAttribute::eAttribParticalVelocity:
                case CShaderAttribute::eAttribParticalLifeTime:
                case CShaderAttribute::eAttribParticalSize:
                case CShaderAttribute::eAttribParticalType:
                case CShaderAttribute::eAttribUser:
                {
                    if (attr.second->getUserDataCount() <= 0)
                    {
                        LOG_WARNING("CGeometryGL::initBufferData: User data have diferent size [%d]", attr.second->getUserDataCount());
                    }

                    size = attr.second->getUserDataSize() * attr.second->getUserDataCount();
                    if (size > 0)
                    {
                        m_vertexBuffer->update(offset, size, attr.second->getUserData());

                       auto componentsCount = [](EDataType type) -> u32
                        {
                            switch (type)
                            {
                            case EDataType::eTypeInt:
                            case EDataType::eTypeFloat:
                            case EDataType::ETypeDouble:
                                return 1;

                            case EDataType::eTypeVector2:
                                return 2;

                            case EDataType::eTypeVector3:
                                return 3;

                            case EDataType::eTypeVector4:
                                return 4;

                            case EDataType::eTypeMatrix3:
                            case EDataType::eTypeMatrix4:
                            default:
                                return 0;
                            }
                        };

                        s32 userLayer = attr.second->getID();
                        EDataType type = attr.second->getType();
                        CGeometryGL::initVertexAttribPointer(userLayer, type, componentsCount(type), attr.second->getUserDataSize(), offset);
                        offset += size;

                        RENDERER->checkForErrors("GeometryGL: init eAttribUser Error");

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
    if (!m_initialized)
    {
        return;
    }

    if (m_data.empty())
    {
        LOG_WARNING("CGeometryGL: Data empty");
        return;
    }

    u32 passIdx = m_technique->getCurrentPass();
    const RenderPassPtr& pass = m_technique->getRenderPass(passIdx);

    ShaderDataList shaderData;
    shaderData.push_back(pass->getDefaultShaderData());
    shaderData.push_back(pass->getUserShaderData());

    //Array
    CGeometryGL::bindVertexArray(m_arrayId);

    //Vertices
    s32 vertexSize = CGeometryGL::computeVertexSize(shaderData);
    m_vertexBuffer->set(m_geometyType, vertexSize, nullptr);
    CGeometryGL::initBufferData(shaderData);

    //Indices
    if (m_data.indicesSize() > 0)
    {
        m_indexBuffer->set(m_geometyType, sizeof(GLint)* m_data.indicesSize(), m_data._indices.data());
    }

    RENDERER->checkForErrors("GeometryGL: Refresh Error");

    //Array
    CGeometryGL::bindVertexArray(0);
}


void CGeometryGL::genVertexArray(u32& buffer)
{
    glGenVertexArrays(1, &buffer);
}

void CGeometryGL::bindVertexArray(u32 buffer)
{
    if (s_currentArray != buffer)
    {
        glBindVertexArray(buffer);
        ASSERT(glIsVertexArray(buffer) || buffer == 0, "Invalid VAO index");
        s_currentArray = buffer;
    }
}

void CGeometryGL::deleteVertexArray(u32& buffer)
{
    if (buffer != 0)
    {
        ASSERT(glIsVertexArray(buffer), "Invalid VAO index");
        glDeleteVertexArrays(1, &buffer);
        buffer = 0;
    }
}

void CGeometryGL::initVertexAttribPointer(s32 attrib, EDataType type, u32 count, u32 size, u32 offset)
{
    auto format = [](EDataType type) -> u32
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

void CGeometryGL::vertexAttribArray(s32 attrib, bool enable)
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

void CGeometryGL::vertexAttribDivisior(s32 attrib, u32 value)
{
    glVertexAttribDivisor(attrib, value);
}

void CGeometryGL::drawElements(EPrimitivesMode mode, u32 count, u32 primCount)
{
    if (primCount > 0)
    {
        glDrawElementsInstanced(EPrimitivesModeGL[mode], count, GL_UNSIGNED_INT, NULL, primCount);
    }
    else
    {
        glDrawElements(EPrimitivesModeGL[mode], count, GL_UNSIGNED_INT, NULL);
    }

    RENDERER->checkForErrors("GeometryGL: CGeometryGL::drawElements Error");
}

void CGeometryGL::drawArrays(EPrimitivesMode mode, u32 first, u32 count, u32 primCount)
{
    if (primCount > 0)
    {
        glDrawArraysInstanced(EPrimitivesModeGL[mode], first, count, primCount);
    }
    else
    {
        glDrawArrays(EPrimitivesModeGL[mode], first, count);
    }

    RENDERER->checkForErrors("GeometryGL: CGeometryGL::drawArrays Error");
}

} //namespace gl
} //namespace renderer
} //namespace v3d

#endif //_OPENGL_DRIVER_
