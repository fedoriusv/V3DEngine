#include "GeomertyGL.h"
#include "GL/glew.h"

#include "renderer/ShaderAttribute.h"
#include "renderer/ShaderData.h"
#include "utils/Logger.h"
#include "Engine.h"

using namespace v3d;
using namespace v3d::renderer;

CGeometryGL::CGeometryGL(const AttributeList& attributes)
    : CGeometry(attributes)
	, m_arrayId(0)
	, m_drawModeGL(GL_TRIANGLE_STRIP)
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

    for (auto attr : m_attributes)
    {
        EShaderAttribute type = attr.second->getAttributeType();

        switch (type)
        {
            case EShaderAttribute::eAttributeVertex:
            {
                CGeometryGL::genBuffers(m_data.m_vertices.id);
                CGeometryGL::bindBuffers(GL_ARRAY_BUFFER, m_data.m_vertices.id);
                CGeometryGL::bufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* m_data.m_countVertices * 3, m_data.m_vertices.vertex.data());
                CGeometryGL::initVertexAttribPointer(EShaderAttribute::eAttributeVertex, 3);
            }
            break;

            case EShaderAttribute::eAttributeNormal:
            {
                CGeometryGL::genBuffers(m_data.m_normals.id);
                CGeometryGL::bindBuffers(GL_ARRAY_BUFFER, m_data.m_normals.id);
                CGeometryGL::bufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* m_data.m_countVertices * 3, m_data.m_normals.vertex.data());
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
                for (v3d::u32 layer = 0; layer < m_data.m_texCoords.size(); ++layer)
                {
                    CGeometryGL::genBuffers(m_data.m_texCoords.at(layer).id);
                    CGeometryGL::bindBuffers(GL_ARRAY_BUFFER, m_data.m_texCoords.at(layer).id);
                    CGeometryGL::bufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* m_data.m_countVertices * 2, m_data.m_texCoords.at(layer).vertex.data());
                    CGeometryGL::initVertexAttribPointer(EShaderAttribute::eAttributeTexture0 + layer, 2);
                }
            }
            break;
        }
    }
	
    //Indices
	CGeometryGL::genBuffers(m_data.m_indices.id);
	CGeometryGL::bindBuffers(GL_ELEMENT_ARRAY_BUFFER, m_data.m_indices.id);
	CGeometryGL::bufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint) * m_data.m_countIndices, m_data.m_indices.vertex.data());

	v3d::CEngine::getInstance()->getRenderer()->checkForErrors("GeometryGL Init Error");

	CGeometryGL::bindVertexArray(0);
	CGeometryGL::bindBuffers(GL_ARRAY_BUFFER, 0);
	CGeometryGL::bindBuffers(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_drawModeGL = CGeometryGL::getGLDrawMode(m_drawMode);
}

void CGeometryGL::update()
{
	CGeometryGL::bindVertexArray(m_arrayId);

	glDrawElements(m_drawModeGL, m_data.m_countIndices, GL_UNSIGNED_INT, NULL);
	//glDrawArrays(_mode, _firstPoint, (_count == 0) ? _vertexData.nVertices : _count);

	CGeometryGL::bindVertexArray(0);

	v3d::CEngine::getInstance()->getRenderer()->checkForErrors("GeometryGL Update Error");
}

void CGeometryGL::free()
{
	CGeometryGL::deleteVertexArray(m_arrayId);

	CGeometryGL::deleteBuffers(m_data.m_vertices.id);
	CGeometryGL::deleteBuffers(m_data.m_normals.id);
	for (v3d::u32 layer = 0; layer < m_data.m_texCoords.size(); ++layer)
	{
		CGeometryGL::deleteBuffers(m_data.m_texCoords.at(layer).id);
	}
	CGeometryGL::deleteBuffers(m_data.m_indices.id);
}

void CGeometryGL::refresh()
{
	//Vertex
	CGeometryGL::bindBuffers(GL_ARRAY_BUFFER, m_data.m_vertices.id);
	CGeometryGL::bufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* m_data.m_countVertices * 3, NULL);
	CGeometryGL::bufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)* m_data.m_countVertices * 3, m_data.m_vertices.vertex.data());

	//Normal
	CGeometryGL::bindBuffers(GL_ARRAY_BUFFER, m_data.m_normals.id);
	CGeometryGL::bufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* m_data.m_countVertices * 3, NULL);
	CGeometryGL::bufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)* m_data.m_countVertices * 3, m_data.m_normals.vertex.data());

	//TexCoords
	for (v3d::u32 layer = 0; layer < m_data.m_texCoords.size(); ++layer)
	{
		CGeometryGL::bindBuffers(GL_ARRAY_BUFFER, m_data.m_texCoords.at(layer).id);
		CGeometryGL::bufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* m_data.m_countVertices * 3, NULL);
		CGeometryGL::bufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)* m_data.m_countVertices * 2, m_data.m_texCoords.at(layer).vertex.data());
	}

	//Indices
	CGeometryGL::bindBuffers(GL_ELEMENT_ARRAY_BUFFER, m_data.m_indices.id);
	CGeometryGL::bufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint)* m_data.m_countIndices, NULL);
	CGeometryGL::bufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLint)* m_data.m_countIndices, m_data.m_indices.vertex.data());

	CGeometryGL::bindBuffers(GL_ARRAY_BUFFER, 0);
	CGeometryGL::bindBuffers(GL_ELEMENT_ARRAY_BUFFER, 0);

	v3d::CEngine::getInstance()->getRenderer()->checkForErrors("GeometryGL Refresh Error");

}

void CGeometryGL::genBuffers(v3d::u32& buffer)
{
	glGenBuffers(1, &buffer);
	ASSERT(glIsBuffer(buffer) && "Invalid VBO index");
}

void CGeometryGL::bindBuffers(const v3d::u32 target, const v3d::u32 buffer)
{
	ASSERT(glIsBuffer(buffer) && "Invalid VBO index");
	glBindBuffer(target, buffer);
}

void CGeometryGL::deleteBuffers(v3d::u32& buffer)
{
	if (buffer != 0)
	{
		ASSERT(glIsShader(buffer) && "Invalid Index Buffer");
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
	ASSERT(glIsVertexArray(buffer) && "Invalid VAO index");
}

void CGeometryGL::bindVertexArray(const v3d::u32 buffer)
{
	ASSERT(glIsVertexArray(buffer) && "Invalid VAO index");
	glBindVertexArray(buffer);
}

void CGeometryGL::deleteVertexArray(v3d::u32& buffer)
{
	if (buffer)
	{
		ASSERT(glIsVertexArray(buffer) && "Invalid VAO index");
		glDeleteVertexArrays(1, &buffer);
	}
}

void CGeometryGL::initVertexAttribPointer(const v3d::u32 vertexAttrib, const v3d::u32 size)
{
	glEnableVertexAttribArray(vertexAttrib);
	glVertexAttribPointer(vertexAttrib, size, GL_FLOAT, GL_FALSE, NULL, NULL);
}

v3d::u32 CGeometryGL::getGLDrawMode(EDrawMode mode)
{
	switch (mode)
	{
		case EDrawMode::eTriangleStrip:
		{
			return GL_TRIANGLE_STRIP;
		}

		case EDrawMode::eTriangles:
		{
			return GL_TRIANGLES;
		}
		
		case EDrawMode::eTriangleFan:
		{
			return GL_TRIANGLE_FAN;
		}

		default:
		{
			return GL_TRIANGLE_STRIP;
		}
	}

	return GL_TRIANGLE_STRIP;
}