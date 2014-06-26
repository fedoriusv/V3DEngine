#include "DebugDrawGL.h"
#include "GeomertyGL.h"
#include "Engine.h"
#include "GL/glew.h"

using namespace v3d;
using namespace v3d::core;
using namespace v3d::renderer;

GLenum EDebugModeGL[EDrawMode::eCount] =
{
    GL_TRIANGLE_STRIP,
    GL_TRIANGLES,
    GL_TRIANGLE_FAN,
    GL_LINES
};

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
                                uniform vec4 color;\n\
                                out vec4 fragColor;\n\
                                \n\
                                void main()\n\
                                {\n\
                                    fragColor = color;\n\
                                }";

CDebugDrawGL::CDebugDrawGL(const GeometryPtr& geometry)
    : CDebugDraw(geometry)
{
    m_vertex = &k_vertex;
    m_fragment = &k_fragment;
}

CDebugDrawGL::CDebugDrawGL(const Vector3D* position, const f32* radius)
    : CDebugDraw(position, radius)
{
    m_vertex = &k_vertex;
    m_fragment = &k_fragment;
}

CDebugDrawGL::~CDebugDrawGL()
{
}

void CDebugDrawGL::draw()
{
    if (m_flag == EDebugFlag::eDebugAll)
    {
        CDebugDrawGL::drawNormals();
        CDebugDrawGL::drawEdges();

        RENDERER->checkForErrors("CDebugDrawGL Update Error");

        return;
    }

    if (m_flag & EDebugFlag::eDebugNormals)
    {
        CDebugDrawGL::drawNormals();
    }
    if (m_flag & EDebugFlag::eDebugEdges)
    {
        CDebugDrawGL::drawEdges();
    }
    if (m_flag & EDebugFlag::eDebugLights)
    {
        CDebugDrawGL::drawLights();
    }

    RENDERER->checkForErrors("CDebugDrawGL Update Error");
}

void CDebugDrawGL::drawNormals()
{
    CDebugDraw::bind(Vector4D(0.0f, 1.0f, 0.0f, 1.0f));
    u32 mode = EDebugModeGL[m_objects[EDebugDraw::eDrawNormals]._drawMode];

    CGeometryGL::bindVertexArray(m_objects[EDebugDraw::eDrawNormals]._arrayId);
    CGeometryGL::drawArrays(mode, 0, m_objects[EDebugDraw::eDrawNormals]._vertex.vertex.size());
    CGeometryGL::bindVertexArray(0);
}

void CDebugDrawGL::drawEdges()
{
    CDebugDraw::bind(Vector4D(1.0f, 0.0f, 0.0f, 1.0f));
    u32 mode = EDebugModeGL[m_objects[EDebugDraw::eDrawEdges]._drawMode];

    CGeometryGL::bindVertexArray(m_objects[EDebugDraw::eDrawEdges]._arrayId);
    if (m_objects[EDebugDraw::eDrawEdges]._index.vertex.size() > 0)
    {
        CGeometryGL::drawElements(mode, m_objects[EDebugDraw::eDrawEdges]._index.vertex.size());
    }
    else
    {
        CGeometryGL::drawArrays(mode, 0, m_objects[EDebugDraw::eDrawEdges]._vertex.vertex.size());
    }
    CGeometryGL::bindVertexArray(0);
}

void CDebugDrawGL::drawLights()
{
    CDebugDraw::bind(Vector4D(0.0f, 0.0f, 1.0f, 1.0f));
    u32 mode = EDebugModeGL[m_objects[EDebugDraw::eDrawLights]._drawMode];

    CGeometryGL::bindVertexArray(m_objects[EDebugDraw::eDrawLights]._arrayId);
    if (m_objects[EDebugDraw::eDrawLights]._index.vertex.size() > 0)
    {
        CGeometryGL::drawElements(mode, m_objects[EDebugDraw::eDrawLights]._index.vertex.size());
    }
    else
    {
        CGeometryGL::drawArrays(mode, 0, m_objects[EDebugDraw::eDrawLights]._vertex.vertex.size());
    }
    CGeometryGL::bindVertexArray(0);
}

void CDebugDrawGL::initDraw(SDebugObject& object)
{
    CGeometryGL::genVertexArray(object._arrayId);

    CGeometryGL::bindVertexArray(object._arrayId);

    CGeometryGL::genBuffers(object._vertex.id);
    CGeometryGL::bindBuffers(GL_ARRAY_BUFFER, object._vertex.id);
    CGeometryGL::bufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* object._vertex.vertex.size() * 3, object._vertex.vertex.data());
    CGeometryGL::initVertexAttribPointer(EShaderAttribute::eAttributeVertex, 3);

    if (object._index.vertex.size() > 0)
    {
        CGeometryGL::genBuffers(object._index.id);
        CGeometryGL::bindBuffers(GL_ELEMENT_ARRAY_BUFFER, object._index.id);
        CGeometryGL::bufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint)* object._index.vertex.size(), object._index.vertex.data());
    }

    CGeometryGL::bindVertexArray(0);

    RENDERER->checkForErrors("CDebugDrawGL Init Error");
}

void CDebugDrawGL::refreshDraw(SDebugObject& object)
{
    CGeometryGL::bindBuffers(GL_ARRAY_BUFFER, object._vertex.id);
    CGeometryGL::bufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* object._vertex.vertex.size() * 3, NULL);
    CGeometryGL::bufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)* object._vertex.vertex.size() * 3, object._vertex.vertex.data());
    CGeometryGL::bindBuffers(GL_ARRAY_BUFFER, 0);

    if (object._index.vertex.size() > 0)
    {
        CGeometryGL::genBuffers(object._index.id);
        CGeometryGL::bindBuffers(GL_ELEMENT_ARRAY_BUFFER, object._index.id);
        CGeometryGL::bufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)* object._index.vertex.size(), NULL);
        CGeometryGL::bufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint)* object._index.vertex.size(), object._index.vertex.data());
    }

    RENDERER->checkForErrors("CDebugDrawGL Refresh Error");
}

void CDebugDrawGL::free()
{
    CGeometryGL::bindVertexArray(0);
    CGeometryGL::bindBuffers(GL_ARRAY_BUFFER, 0);

    for (u32 i = 0; i < EDebugDraw::eDrawCount; ++i)
    {
        CGeometryGL::deleteVertexArray(m_objects[i]._arrayId);
        CGeometryGL::deleteBuffers(m_objects[i]._vertex.id);
    }

    RENDERER->checkForErrors("CDebugDrawGL Free Error");
}
