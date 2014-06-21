#include "DebugDrawGL.h"
#include "GeomertyGL.h"
#include "Engine.h"
#include "GL/glew.h"

using namespace v3d;
using namespace v3d::renderer;

const std::string k_vertex =  "#version 330\n\
                              #define POSITION 0\n\
                              layout(location = POSITION) in vec3 positions;\n\
                              \n\
                              struct Transform\n\
                              {\n\
                                mat4 projectionMatrix;\n\
                                mat4 modelMatrix;\n\
                                mat4 viewMatrix;\n\
                              };\n\
                              uniform Transform transform;\n\
                              \n\
                              void main()\n\
                              {\n\
                                vec4 vertex = transform.modelMatrix * vec4(positions, 1.0);\n\
                                gl_Position = transform.projectionMatrix * transform.viewMatrix * vertex;\n\
                              }";

const std::string k_fragment = "#version 330\n\
                                out vec4 fragColor;\n\
                                \n\
                                void main()\n\
                                {\n\
                                    fragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);\n\
                                }";

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
    CDebugDraw::bind();

    switch (m_flag)
    {
        case EDebugDraw::eDrawNormals:

            CGeometryGL::bindVertexArray(m_arrayId);
            CGeometryGL::drawArrays(GL_LINES, 0, m_normals.vertex.size());
            CGeometryGL::bindVertexArray(0);
            break;

        case EDebugDraw::eDrawAll:

            CGeometryGL::bindVertexArray(m_arrayId);
            CGeometryGL::drawArrays(GL_LINES, 0, m_normals.vertex.size());
            CGeometryGL::bindVertexArray(0);
            break;

        case EDebugDraw::eDrawNone:
        default:

            break;
    }

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