#include "DebugGeometryGL.h"
#include "GeomertyGL.h"
#include "Engine.h"
#include "GL/glew.h"

using namespace v3d;
using namespace v3d::core;
using namespace v3d::renderer;

GLenum EDebugGeometryModeGL[CGeometry::eCount] =
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

CDebugGeometryGL::CDebugGeometryGL(const GeometryPtr& geometry)
    : CDebugGeometry(geometry)
{
    m_vertex = &k_vertex;
    m_fragment = &k_fragment;
}

CDebugGeometryGL::~CDebugGeometryGL()
{
}

void CDebugGeometryGL::draw()
{
    const ShaderDataPtr& data = s_pass->getShaderData();

    if (m_flag & EDebugGeometryFlag::eGeometryFlagNormals)
    {
        data->addUniformVector4("color", Vector4D(0.0f, 1.0f, 0.0f, 1.0f));

        CDebugGeometryGL::drawObject(m_objects[EDebugGeometry::eGeometryNormals]);
    }
    if (m_flag & EDebugGeometryFlag::eGeometryFlagEdges)
    {
        data->addUniformVector4("color", Vector4D(1.0f, 0.0f, 0.0f, 1.0f));

        CDebugGeometryGL::drawObject(m_objects[EDebugGeometry::eGeometryEdges]);
    }

    RENDERER->checkForErrors("CDebugDrawGL Update Error");
}

void CDebugGeometryGL::drawObject(const SDebugObject& object)
{
    CDebugGeometry::bind();

   /* u32 mode = EDebugGeometryModeGL[object._drawMode];
    CGeometryGL::bindVertexArray(object._arrayId);
    if (object._index.vertex.size() > 0)
    {
        CGeometryGL::drawElements(mode, object._index.vertex.size());
    }
    else
    {
        CGeometryGL::drawArrays(mode, 0, object._vertex.vertex.size());
    }*/
    CGeometryGL::bindVertexArray(0);
}

void CDebugGeometryGL::initDraw(SDebugObject& object)
{
    CGeometryGL::genVertexArray(object._arrayId);

    CGeometryGL::bindVertexArray(object._arrayId);

   /* CGeometryGL::genBuffers(object._vertex.id);
    CGeometryGL::bindBuffers(GL_ARRAY_BUFFER, object._vertex.id);
    CGeometryGL::bufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* object._vertex.vertex.size() * 3, object._vertex.vertex.data());
    CGeometryGL::initVertexAttribPointer(EShaderAttribute::eAttributeVertex, 3);

    if (object._index.vertex.size() > 0)
    {
        CGeometryGL::genBuffers(object._index.id);
        CGeometryGL::bindBuffers(GL_ELEMENT_ARRAY_BUFFER, object._index.id);
        CGeometryGL::bufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint)* object._index.vertex.size(), object._index.vertex.data());
    }*/

    CGeometryGL::bindVertexArray(0);

    RENDERER->checkForErrors("CDebugDrawGL Init Error");
}

void CDebugGeometryGL::refreshDraw(SDebugObject& object)
{
   /* CGeometryGL::bindBuffers(GL_ARRAY_BUFFER, object._vertex.id);
    CGeometryGL::bufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* object._vertex.vertex.size() * 3, NULL);
    CGeometryGL::bufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)* object._vertex.vertex.size() * 3, object._vertex.vertex.data());
    CGeometryGL::bindBuffers(GL_ARRAY_BUFFER, 0);

    if (object._index.vertex.size() > 0)
    {
        CGeometryGL::genBuffers(object._index.id);
        CGeometryGL::bindBuffers(GL_ELEMENT_ARRAY_BUFFER, object._index.id);
        CGeometryGL::bufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)* object._index.vertex.size(), NULL);
        CGeometryGL::bufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint)* object._index.vertex.size(), object._index.vertex.data());
    }*/

    RENDERER->checkForErrors("CDebugDrawGL Refresh Error");
}

void CDebugGeometryGL::free()
{
    CGeometryGL::bindVertexArray(0);
    CGeometryGL::bindBuffers(GL_ARRAY_BUFFER, 0);

   /* for (u32 i = 0; i < EDebugGeometry::eGeometryCount; ++i)
    {
        CGeometryGL::deleteVertexArray(m_objects[i]._arrayId);
        CGeometryGL::deleteBuffers(m_objects[i]._vertex.id);
    }*/

    RENDERER->checkForErrors("CDebugDrawGL Free Error");
}
