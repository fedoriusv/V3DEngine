#include "GeomertyGL.h"
#include "GL/glew.h"

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
	ASSERT(glIsBuffer(buffer) || "Invalid VBO index");
	glBindBuffer(target, buffer);
}

void CGeometryGL::deleteBuffers(f3d::u32& buffer)
{
	if (buffer != 0)
	{
		ASSERT(glIsShader(buffer) || "Invalid Index Buffer");
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
	ASSERT(glIsVertexArray(buffer) || "Invalid VAO index");
}

void CGeometryGL::bindVertexArray(const f3d::u32 buffer)
{
	ASSERT(glIsVertexArray(buffer) || "Invalid VAO index");
	glBindVertexArray(buffer);
}

void CGeometryGL::deleteVertexArray(f3d::u32& buffer)
{
	if (buffer)
	{
		ASSERT(glIsVertexArray(buffer) || "Invalid VAO index");
		glDeleteVertexArrays(1, &buffer);
	}
}

void CGeometryGL::initVertexAttribPointer(const f3d::u32 vertexAttrib, const f3d::u32 size)
{
	glEnableVertexAttribArray(vertexAttrib);
	glVertexAttribPointer(vertexAttrib, size, GL_FLOAT, GL_FALSE, NULL, NULL);
}