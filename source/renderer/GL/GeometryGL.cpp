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

using namespace gl;

CGeometryGL::CGeometryGL(const CRenderTechnique* technique)
    : CGeometry(technique)
    , m_arrayId(0U)
    , m_verticesId(0U)
    , m_indicesId(0U)

    , m_initialized(false)
{
    LOG_DEBUG("CGeometryGL: CGeometryGL constructor %x", this);
}

CGeometryGL::~CGeometryGL()
{
    ASSERT(m_arrayId == 0, "VAO doesn't deleted");
    ASSERT(m_verticesId == 0, "VAO doesn't deleted");
    ASSERT(m_indicesId == 0, "IAO doesn't deleted");

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

    BufferGL::genVertexArray(m_arrayId);
    BufferGL::bindVertexArray(m_arrayId);

    const RenderPassPtr& pass = m_technique->getRenderPass(m_technique->getCurrentPass());
    u32 mask = pass->getDefaultShaderData()->getVertexFormatMask() | pass->getUserShaderData()->getVertexFormatMask();
    CGeometry::setVertexMask(mask);

    BufferGL::genBuffer(m_verticesId);
    BufferGL::bindBuffer(eVertexBuffer, m_verticesId);

    ShaderDataList shaderData;
    shaderData.push_back(pass->getDefaultShaderData());
    shaderData.push_back(pass->getUserShaderData());

    //Vertices
    u32 vertexSize = CGeometryGL::computeVertexSize(shaderData);
    BufferGL::bufferData(eVertexBuffer, m_geometyType, vertexSize, nullptr);
    CGeometryGL::initBufferData(shaderData);

    //Indices
    if (m_data.indicesSize() > 0)
    {
        BufferGL::genBuffer(m_indicesId);
        BufferGL::bindBuffer(eIndexBuffer, m_indicesId);
        BufferGL::bufferData(eIndexBuffer, m_geometyType, sizeof(GLint)* m_data.indicesSize(), m_data._indices.data());
    }

    RENDERER->checkForErrors("GeometryGL: Init Error");

    BufferGL::bindVertexArray(0);
    BufferGL::bindBuffer(eVertexBuffer, 0);
    BufferGL::bindBuffer(eIndexBuffer, 0);

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

    CGeometry::draw();

    BufferGL::bindVertexArray(m_arrayId);

    u32 passIdx = m_technique->getCurrentPass();
    const RenderPassPtr& pass = m_technique->getRenderPass(passIdx);

    const AttributeList& attributesDefault = pass->getDefaultShaderData()->getAttributeList();
    for (const AttributePair& attr : attributesDefault)
    {
        u32 attribute = attr.second->getID();
        BufferGL::vertexAttribArray(attribute, true);
    }

    const AttributeList& attributesUser = pass->getUserShaderData()->getAttributeList();
    for (const AttributePair& attr : attributesUser)
    {
        u32 attribute = attr.second->getID();
        BufferGL::vertexAttribArray(attribute, true);
    }

    u32 instanceAmount = pass->getRenderAdvanced()->getCountInstance();
    if (m_data.indicesSize() > 0)
    {
        BufferGL::drawElements(m_drawMode, m_data.indicesSize(), instanceAmount);
    }
    else
    {
        BufferGL::drawArrays(m_drawMode, CGeometry::getInterval()._begin, CGeometry::getInterval()._count, instanceAmount);
    }

    for (const AttributePair& attr : attributesDefault)
    {
        u32 attribute = attr.second->getID();
        BufferGL::vertexAttribArray(attribute, false);
    }

    for (const AttributePair& attr : attributesUser)
    {
        u32 attribute = attr.second->getID();
        BufferGL::vertexAttribArray(attribute, false);
    }

    BufferGL::bindVertexArray(0);

    RENDERER->checkForErrors("GeometryGL: Draw Error");
}

void CGeometryGL::free()
{
    if (!m_initialized)
    {
        return;
    }

    BufferGL::deleteBuffer(m_indicesId);
    BufferGL::deleteBuffer(m_verticesId);

    BufferGL::deleteVertexArray(m_arrayId);

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
                    BufferGL::bufferSubData(eVertexBuffer, offset, size, m_data._vertices.data());
                    BufferGL::initVertexAttribPointer(attr.second->getID(), EDataType::eTypeVector3, 3, sizeof(GLfloat) * 3, offset);
                    offset += size;
                }
                break;

                case CShaderAttribute::eAttribVertexNormal:
                {
                    size = sizeof(GLfloat)* m_data.verticesSize() * 3;
                    BufferGL::bufferSubData(eVertexBuffer, offset, size, m_data._normals.data());
                    BufferGL::initVertexAttribPointer(attr.second->getID(), EDataType::eTypeVector3, 3, sizeof(GLfloat) * 3, offset);
                    offset += size;
                }
                break;

                case CShaderAttribute::eAttribVertexColor:
                {
                    size = sizeof(GLfloat)* m_data.verticesSize() * 3;
                    BufferGL::bufferSubData(eVertexBuffer, offset, size, m_data._colors.data());
                    BufferGL::initVertexAttribPointer(attr.second->getID(), EDataType::eTypeVector3, 3, sizeof(GLfloat) * 3, offset);
                    offset += size;
                }
                break;

                case CShaderAttribute::eAttribVertexBinormal:
                {
                    size = sizeof(GLfloat)* m_data.verticesSize() * 3;
                    BufferGL::bufferSubData(eVertexBuffer, offset, size, m_data._binormals.data());
                    BufferGL::initVertexAttribPointer(attr.second->getID(), EDataType::eTypeVector3, 3, sizeof(GLfloat) * 3, offset);
                    offset += size;
                }
                break;

                case CShaderAttribute::eAttribVertexTangent:
                {
                    size = sizeof(GLfloat)* m_data.verticesSize() * 3;
                    BufferGL::bufferSubData(eVertexBuffer, offset, size, m_data._tangents.data());
                    BufferGL::initVertexAttribPointer(attr.second->getID(), EDataType::eTypeVector3, 3, sizeof(GLfloat) * 3, offset);
                    offset += size;
                }
                break;

                case CShaderAttribute::eAttribVertexTexture0:
                case CShaderAttribute::eAttribVertexTexture1:
                case CShaderAttribute::eAttribVertexTexture2:
                case CShaderAttribute::eAttribVertexTexture3:
                {
                    size = sizeof(GLfloat)* m_data.verticesSize() * 2;
                    BufferGL::bufferSubData(eVertexBuffer, offset, size, m_data._texCoords.at(layer).data());
                    BufferGL::initVertexAttribPointer(attr.second->getID(), EDataType::eTypeVector2, 2, sizeof(GLfloat) * 2, offset);
                    offset += size;
                    ++layer;
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
                        BufferGL::bufferSubData(eVertexBuffer, offset, size, attr.second->getUserData());

                        std::function<u32(EDataType)> componentsCount = [](EDataType type) -> u32
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

                        u32 userLayer = attr.second->getID();
                        EDataType type = attr.second->getType();
                        BufferGL::initVertexAttribPointer(userLayer, type, componentsCount(type), attr.second->getUserDataSize(), offset);
                        offset += size;

                        u32 divisor = attr.second->getDivisor();
                        if (divisor > 0)
                        {
                            BufferGL::vertexAttribDivisior(userLayer, 1);
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

    BufferGL::bindVertexArray(m_arrayId);
    BufferGL::bindBuffer(eVertexBuffer, m_verticesId);

    ShaderDataList shaderData;
    shaderData.push_back(pass->getDefaultShaderData());
    shaderData.push_back(pass->getUserShaderData());

    s32 vertexSize = CGeometryGL::computeVertexSize(shaderData);
    BufferGL::bufferData(eVertexBuffer, m_geometyType, vertexSize, nullptr);
    CGeometryGL::initBufferData(shaderData);

    //Indices
    if (m_data.indicesSize() > 0)
    {
        BufferGL::bindBuffer(eIndexBuffer, m_indicesId);
        BufferGL::bufferData(eIndexBuffer, m_geometyType, sizeof(GLint)* m_data.indicesSize(), nullptr);
        BufferGL::bufferSubData(eIndexBuffer, 0, sizeof(GLint)* m_data.indicesSize(), m_data._indices.data());
    }

    RENDERER->checkForErrors("GeometryGL: Refresh Error");

    BufferGL::bindVertexArray(0);
    BufferGL::bindBuffer(eVertexBuffer, 0);
    BufferGL::bindBuffer(eIndexBuffer, 0);
}

} //namespace renderer
} //namespace v3d

#endif //_OPENGL_DRIVER_
