#include "GeomertyGL.h"
#include "GL/glew.h"

#include "utils/Logger.h"
#include "Engine.h"

using namespace f3d;
using namespace f3d::renderer;

CGeometryGL::CGeometryGL()
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

	CGeometryGL::genVertexArray(m_data.m_arrayId);
	CGeometryGL::bindVertexArray(m_data.m_arrayId);

	//Vertex
	CGeometryGL::genBuffers(m_data.m_vertices.id);
	CGeometryGL::bindBuffers(GL_ARRAY_BUFFER, m_data.m_vertices.id);
	CGeometryGL::bufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_data.m_countVertices * 3, m_data.m_vertices.vertex.data());
	CGeometryGL::initVertexAttribPointer(eAttributeVertex, 3);

	//Normal
	CGeometryGL::genBuffers(m_data.m_normals.id);
	CGeometryGL::bindBuffers(GL_ARRAY_BUFFER, m_data.m_normals.id);
	CGeometryGL::bufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_data.m_countVertices * 3, m_data.m_normals.vertex.data());
	CGeometryGL::initVertexAttribPointer(eAttributeNormal, 3);

	//TexCoords
	for (f3d::u32 layer = 0; layer < m_data.m_texCoords.size(); ++layer)
	{
		CGeometryGL::genBuffers(m_data.m_texCoords.at(layer).id);
		CGeometryGL::bindBuffers(GL_ARRAY_BUFFER, m_data.m_texCoords.at(layer).id);
		CGeometryGL::bufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* m_data.m_countVertices * 2, m_data.m_texCoords.at(layer).vertex.data());
		CGeometryGL::initVertexAttribPointer(eAttributeTexture0 + layer, 2);
	}

	//Indices
	CGeometryGL::genBuffers(m_data.m_indices.id);
	CGeometryGL::bindBuffers(GL_ELEMENT_ARRAY_BUFFER, m_data.m_indices.id);
	CGeometryGL::bufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint) * m_data.m_countIndices, m_data.m_indices.vertex.data());

	f3d::CEngine::getInstance()->getRenderer()->checkForErrors("VBO Error");

	CGeometryGL::bindVertexArray(0);
	CGeometryGL::bindBuffers(GL_ARRAY_BUFFER, 0);
	CGeometryGL::bindBuffers(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void CGeometryGL::update()
{
}

void CGeometryGL::free()
{
}

void CGeometryGL::genBuffers(f3d::u32& buffer)
{
	glGenBuffers(1, &buffer);
	ASSERT(glIsBuffer(buffer) && "Invalid VBO index");
}

void CGeometryGL::bindBuffers(const f3d::u32 target, const f3d::u32 buffer)
{
	ASSERT(glIsBuffer(buffer) && "Invalid VBO index");
	glBindBuffer(target, buffer);
}

void CGeometryGL::deleteBuffers(f3d::u32& buffer)
{
	if (buffer != 0)
	{
		ASSERT(glIsShader(buffer) && "Invalid Index Buffer");
		glDeleteBuffers(1, &buffer);
	}
}

void CGeometryGL::bufferData(const f3d::u32 target, const f3d::u32 size, void* data)
{
	glBufferData(target, size, data, GL_STATIC_DRAW);
}

void CGeometryGL::bufferSubData(const f3d::u32 target, const f3d::u32 offset, const f3d::u32 size, void* data)
{
	glBufferSubData(target, offset, size, data);
}

void* CGeometryGL::mapBuffer(const f3d::u32 target, const f3d::u32 access)
{
	return glMapBuffer(target, access);
}

void* CGeometryGL::mapBufferRange(const f3d::u32 target, const f3d::u32 offset, const f3d::u32 size, const f3d::u32 flags)
{
	return glMapBufferRange(target, offset, size, flags);
}

bool CGeometryGL::unmapBuffer(const f3d::u32 target)
{
	return glUnmapBuffer(target) != 0;
}

void CGeometryGL::getBufferPointer(const f3d::u32 target, const f3d::u32 pname, void** params)
{
	glGetBufferPointerv(target, pname, params);
}

void CGeometryGL::genVertexArray(f3d::u32& buffer)
{
	glGenVertexArrays(1, &buffer);
	ASSERT(glIsVertexArray(buffer) && "Invalid VAO index");
}

void CGeometryGL::bindVertexArray(const f3d::u32 buffer)
{
	ASSERT(glIsVertexArray(buffer) && "Invalid VAO index");
	glBindVertexArray(buffer);
}

void CGeometryGL::deleteVertexArray(f3d::u32& buffer)
{
	if (buffer)
	{
		ASSERT(glIsVertexArray(buffer) && "Invalid VAO index");
		glDeleteVertexArrays(1, &buffer);
	}
}

void CGeometryGL::initVertexAttribPointer(const f3d::u32 vertexAttrib, const f3d::u32 size)
{
	glEnableVertexAttribArray(vertexAttrib);
	glVertexAttribPointer(vertexAttrib, size, GL_FLOAT, GL_FALSE, NULL, NULL);
}