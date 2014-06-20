#include "DebugDrawGL.h"
#include "GeomertyGL.h"
#include "Engine.h"
#include "GL/glew.h"

using namespace v3d;
using namespace v3d::renderer;

const std::string k_vertex = "";
const std::string k_fragment = "";

CDebugDrawGL::CDebugDrawGL(const GeometryPtr& geometry)
: CDebugDraw(geometry)
{
    m_vertex = &k_vertex;
    m_fragment = &k_fragment;
}

CDebugDrawGL::~CDebugDrawGL()
{
}

void CDebugDrawGL::draw()
{
    CGeometryGL::bindVertexArray(m_arrayId);

    glDrawArrays(GL_LINES, 0, m_normals.vertex.size());

    CGeometryGL::bindVertexArray(0);

    RENDERER->checkForErrors("CDebugDrawGL Update Error");
}

void CDebugDrawGL::refresh()
{
    CGeometryGL::bindBuffers(GL_ARRAY_BUFFER, m_normals.id);
    CGeometryGL::bufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* m_normals.vertex.size() * 3, NULL);
    CGeometryGL::bufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)* m_normals.vertex.size() * 3, m_normals.vertex.data());

    RENDERER->checkForErrors("CDebugDrawGL Refresh Error");

    CGeometryGL::bindBuffers(GL_ARRAY_BUFFER, 0);
}

void CDebugDrawGL::free()
{
    CGeometryGL::bindBuffers(GL_ARRAY_BUFFER, 0);
    CGeometryGL::deleteVertexArray(m_arrayId);
    CGeometryGL::deleteBuffers(m_normals.id);
}

void CDebugDrawGL::initDrawNormalsData()
{
    CDebugDraw::initDrawNormalsData();

    CGeometryGL::genVertexArray(m_arrayId);
    CGeometryGL::bindVertexArray(m_arrayId);

    CGeometryGL::genBuffers(m_normals.id);
    CGeometryGL::bindBuffers(GL_ARRAY_BUFFER, m_normals.id);
    CGeometryGL::bufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* m_normals.vertex.size() * 3, m_normals.vertex.data());
    CGeometryGL::initVertexAttribPointer(EShaderAttribute::eAttributeVertex, 3);

    RENDERER->checkForErrors("CDebugDrawGL Init Error");

    CGeometryGL::bindVertexArray(0);
    CGeometryGL::bindBuffers(GL_ARRAY_BUFFER, 0);
}