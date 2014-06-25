#include "GeomertyGL.h"

#include "renderer/ShaderAttribute.h"
#include "renderer/ShaderData.h"
#include "utils/Logger.h"
#include "Engine.h"

#include "GL/glew.h"

using namespace v3d;
using namespace v3d::renderer;

GLenum EDrawModeGL[EDrawMode::eCount] =
{
    GL_TRIANGLE_STRIP,
    GL_TRIANGLES,
    GL_TRIANGLE_FAN,
    GL_LINES
};


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
		LOG_WARRNING("Data empty");
		return;
	}

    CGeometryGL::genVertexArray(m_arrayId);
    CGeometryGL::bindVertexArray(m_arrayId);

    for (u32 idx = 0; idx < m_technique->getRenderPassCount(); ++idx)
    {
        const RenderPassPtr& pass = m_technique->getRenderPass(idx);
        const AttributeList& attributes = pass->getShaderData()->getAttributeList();

        for (auto attr : attributes)
        {
            EShaderAttribute type = attr.second->getAttributeType();

            switch (type)
            {
                case EShaderAttribute::eAttributeVertex:
                {
                    CGeometryGL::genBuffers(m_data._vertices.id);
                    CGeometryGL::bindBuffers(GL_ARRAY_BUFFER, m_data._vertices.id);
                    CGeometryGL::bufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* m_data._countVertices * 3, m_data._vertices.vertex.data());
                    CGeometryGL::initVertexAttribPointer(EShaderAttribute::eAttributeVertex, 3);
                }
                break;

                case EShaderAttribute::eAttributeNormal:
                {
                    CGeometryGL::genBuffers(m_data._normals.id);
                    CGeometryGL::bindBuffers(GL_ARRAY_BUFFER, m_data._normals.id);
                    CGeometryGL::bufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* m_data._countVertices * 3, m_data._normals.vertex.data());
                    CGeometryGL::initVertexAttribPointer(EShaderAttribute::eAttributeNormal, 3);
                }
                break;

                case EShaderAttribute::eAttributeColor:
                {
                    //TODO: Color
                }
                break;

                case EShaderAttribute::eAttributeBinormal:
                {
                    //TODO: binormal
                }
                break;

                case EShaderAttribute::eAttributeTangent:
                {
                    //TODO: tangent
                }
                break;

                case EShaderAttribute::eAttributeTexture0:
                case EShaderAttribute::eAttributeTexture1:
                case EShaderAttribute::eAttributeTexture2:
                case EShaderAttribute::eAttributeTexture3:
                {
                    for (v3d::u32 layer = 0; layer < m_data._texCoords.size(); ++layer)
                    {
                        CGeometryGL::genBuffers(m_data._texCoords.at(layer).id);
                        CGeometryGL::bindBuffers(GL_ARRAY_BUFFER, m_data._texCoords.at(layer).id);
                        CGeometryGL::bufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* m_data._countVertices * 2, m_data._texCoords.at(layer).vertex.data());
                        CGeometryGL::initVertexAttribPointer(EShaderAttribute::eAttributeTexture0 + layer, 2);
                    }
                }
                break;
            }
        }
    }
	

    //Indices
    if (m_data._countIndices > 0)
    {
        CGeometryGL::genBuffers(m_data._indices.id);
        CGeometryGL::bindBuffers(GL_ELEMENT_ARRAY_BUFFER, m_data._indices.id);
        CGeometryGL::bufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint)* m_data._countIndices, m_data._indices.vertex.data());
    }

    RENDERER->checkForErrors("GeometryGL Init Error");

	CGeometryGL::bindVertexArray(0);
	CGeometryGL::bindBuffers(GL_ARRAY_BUFFER, 0);
	CGeometryGL::bindBuffers(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void CGeometryGL::draw()
{
    CGeometryGL::bindVertexArray(m_arrayId);

    if (m_data._countIndices > 0)
    {
        CGeometryGL::drawElements(EDrawModeGL[m_drawMode], m_data._countIndices);
    }
    else
    {
        CGeometryGL::drawArrays(EDrawModeGL[m_drawMode], 0, m_data._countVertices);
    }

    CGeometryGL::bindVertexArray(0);

    RENDERER->checkForErrors("GeometryGL Draw Error");
}

void CGeometryGL::free()
{
	CGeometryGL::deleteVertexArray(m_arrayId);

	CGeometryGL::deleteBuffers(m_data._vertices.id);
	CGeometryGL::deleteBuffers(m_data._normals.id);
	for (v3d::u32 layer = 0; layer < m_data._texCoords.size(); ++layer)
	{
		CGeometryGL::deleteBuffers(m_data._texCoords.at(layer).id);
	}
	CGeometryGL::deleteBuffers(m_data._indices.id);
}

void CGeometryGL::refresh()
{
    if (m_data.empty())
    {
        LOG_WARRNING("Data empty");
        return;
    }

    for (u32 idx = 0; idx < m_technique->getRenderPassCount(); ++idx)
    {
        const RenderPassPtr pass = m_technique->getRenderPass(idx);
        const AttributeList& attributes = pass->getShaderData()->getAttributeList();

        for (auto attr : attributes)
        {
            EShaderAttribute type = attr.second->getAttributeType();

            switch (type)
            {
                case EShaderAttribute::eAttributeVertex:
                {
                    CGeometryGL::bindBuffers(GL_ARRAY_BUFFER, m_data._vertices.id);
                    CGeometryGL::bufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* m_data._countVertices * 3, NULL);
                    CGeometryGL::bufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)* m_data._countVertices * 3, m_data._vertices.vertex.data());
                }
                break;

                case EShaderAttribute::eAttributeNormal:
                {
                    CGeometryGL::bindBuffers(GL_ARRAY_BUFFER, m_data._normals.id);
                    CGeometryGL::bufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* m_data._countVertices * 3, NULL);
                    CGeometryGL::bufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)* m_data._countVertices * 3, m_data._normals.vertex.data());
                }
                break;

                case EShaderAttribute::eAttributeColor:
                {
                    //TODO: Color
                }
                break;

                case EShaderAttribute::eAttributeBinormal:
                {
                    //TODO: binormal
                }
                break;

                case EShaderAttribute::eAttributeTangent:
                {
                    //TODO: tangent
                }
                break;

                case EShaderAttribute::eAttributeTexture0:
                case EShaderAttribute::eAttributeTexture1:
                case EShaderAttribute::eAttributeTexture2:
                case EShaderAttribute::eAttributeTexture3:
                {
                    for (v3d::u32 layer = 0; layer < m_data._texCoords.size(); ++layer)
                    {
                        CGeometryGL::bindBuffers(GL_ARRAY_BUFFER, m_data._texCoords.at(layer).id);
                        CGeometryGL::bufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* m_data._countVertices * 3, NULL);
                        CGeometryGL::bufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)* m_data._countVertices * 2, m_data._texCoords.at(layer).vertex.data());
                    }
                }
                break;

                default:
                {
                    LOG_WARRNING("Unknown Attribute: %s", CShaderAttribute::getShaderAttributeNameByType(type).c_str());
                }
                break;
            }
        }

    }

    //Indices
    if (m_data._countIndices > 0)
    {
        CGeometryGL::bindBuffers(GL_ELEMENT_ARRAY_BUFFER, m_data._indices.id);
        CGeometryGL::bufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint)* m_data._countIndices, NULL);
        CGeometryGL::bufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLint)* m_data._countIndices, m_data._indices.vertex.data());
    }

    RENDERER->checkForErrors("GeometryGL Refresh Error");

	CGeometryGL::bindBuffers(GL_ARRAY_BUFFER, 0);
	CGeometryGL::bindBuffers(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void CGeometryGL::genBuffers(v3d::u32& buffer)
{
	glGenBuffers(1, &buffer);
	ASSERT(glIsBuffer(buffer) || "Invalid VBO index");
}

void CGeometryGL::bindBuffers(const v3d::u32 target, const v3d::u32 buffer)
{
	ASSERT(glIsBuffer(buffer) || "Invalid VBO index");
	glBindBuffer(target, buffer);
}

void CGeometryGL::deleteBuffers(v3d::u32& buffer)
{
	if (buffer != 0)
	{
		ASSERT(glIsShader(buffer) || "Invalid Index Buffer");
		glDeleteBuffers(1, &buffer);
	}
}

void CGeometryGL::bufferData(const v3d::u32 target, const v3d::u32 size, void* data)
{
	glBufferData(target, size, data, GL_STATIC_DRAW);
}

void CGeometryGL::bufferSubData(const v3d::u32 target, const v3d::u32 offset, const v3d::u32 size, void* data)
{
	glBufferSubData(target, offset, size, data);
}

void* CGeometryGL::mapBuffer(const v3d::u32 target, const v3d::u32 access)
{
	return glMapBuffer(target, access);
}

void* CGeometryGL::mapBufferRange(const v3d::u32 target, const v3d::u32 offset, const v3d::u32 size, const v3d::u32 flags)
{
	return glMapBufferRange(target, offset, size, flags);
}

bool CGeometryGL::unmapBuffer(const v3d::u32 target)
{
	return glUnmapBuffer(target) != 0;
}

void CGeometryGL::getBufferPointer(const v3d::u32 target, const v3d::u32 pname, void** params)
{
	glGetBufferPointerv(target, pname, params);
}

void CGeometryGL::genVertexArray(v3d::u32& buffer)
{
	glGenVertexArrays(1, &buffer);
	ASSERT(glIsVertexArray(buffer) || "Invalid VAO index");
}

void CGeometryGL::bindVertexArray(const v3d::u32 buffer)
{
	ASSERT(glIsVertexArray(buffer) || "Invalid VAO index");
	glBindVertexArray(buffer);
}

void CGeometryGL::deleteVertexArray(v3d::u32& buffer)
{
	if (buffer)
	{
		ASSERT(glIsVertexArray(buffer) || "Invalid VAO index");
		glDeleteVertexArrays(1, &buffer);
	}
}

void CGeometryGL::initVertexAttribPointer(const v3d::u32 vertexAttrib, const v3d::u32 size)
{
	glEnableVertexAttribArray(vertexAttrib);
	glVertexAttribPointer(vertexAttrib, size, GL_FLOAT, GL_FALSE, NULL, NULL);
}

void CGeometryGL::drawElements(const u32 mode, const u32 count)
{
    glDrawElements(mode, count, GL_UNSIGNED_INT, NULL);
}

void CGeometryGL::drawArrays(const u32 mode, const u32 first, const u32 count)
{
    glDrawArrays(mode, first, count);
}