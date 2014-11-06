#include "DebugLightGL.h"
#include "GeometryGL.h"
#include "Engine.h"

#include "GL/glew.h"

using namespace v3d;
using namespace v3d::core;
using namespace v3d::renderer;

const std::string k_vertex = "#version 330\n\
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

CDebugLightGL::CDebugLightGL(const core::Vector3D& position, const scene::SLightData& data)
    : CDebugLight(position, data)
{
    m_vertex = &k_vertex;
    m_fragment = &k_fragment;
}

CDebugLightGL::~CDebugLightGL()
{
}

void CDebugLightGL::draw()
{
    if (m_flag & EDebugLightFlag::eLightFlagPosition)
    {
        CDebugLightGL::drawLightPosition();
    }
    if (m_flag & EDebugLightFlag::eLightFlagDirection)
    {
        //TODO:
    }
    if (m_flag & EDebugLightFlag::eLightFlagRadius)
    {
        //TODO:
    }

    RENDERER->checkForErrors("CDebugLightGL Update Error");
}

void CDebugLightGL::free()
{
    CGeometryGL::bindVertexArray(0);
    CGeometryGL::bindBuffers(GL_ARRAY_BUFFER, 0);

    /*for (u32 i = 0; i < EDebugLight::eDebugLightCount; ++i)
    {
        CGeometryGL::deleteVertexArray(m_objects[i]._arrayId);
        CGeometryGL::deleteBuffers(m_objects[i]._vertex.id);
    }*/

    RENDERER->checkForErrors("CDebugLightGL Free Error");
}

void CDebugLightGL::initDraw(SDebugLight& object)
{
    CGeometryGL::genVertexArray(object._arrayId);

    CGeometryGL::bindVertexArray(object._arrayId);

    /*CGeometryGL::genBuffers(object._vertex.id);
    CGeometryGL::bindBuffers(GL_ARRAY_BUFFER, object._vertex.id);
    CGeometryGL::bufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* object._vertex.vertex.size() * 3, object._vertex.vertex.data());
    CGeometryGL::initVertexAttribPointer(EShaderAttribute::eAttributeVertex, 3);*/

    CGeometryGL::bindVertexArray(0);

    RENDERER->checkForErrors("CDebugLightGL Init Error");
}

void CDebugLightGL::drawLightPosition()
{
    CDebugLight::bind();

   /* CGeometryGL::bindVertexArray(m_objects[EDebugLight::eDebugLightPosition]._arrayId);
    CGeometryGL::drawArrays(GL_LINES, 0, m_objects[EDebugLight::eDebugLightPosition]._vertex.vertex.size());
    CGeometryGL::bindVertexArray(0);*/

    RENDERER->checkForErrors("CDebugLightGL Update Error2");
}
