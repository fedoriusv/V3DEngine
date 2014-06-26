#include "DebugLightGL.h"

using namespace v3d;
using namespace v3d::core;
using namespace v3d::renderer;

CDebugLightGL::CDebugLightGL(const core::Vector3D& position, const scene::SLightData& data)
    : CDebugLight(position, data)
{
}

CDebugLightGL::~CDebugLightGL()
{
}

void CDebugLightGL::draw()
{
}

void CDebugLightGL::free()
{
}

//void CDebugDrawGL::drawLights()
//{
//    CDebugDraw::bind(Vector4D(0.0f, 0.0f, 1.0f, 1.0f));
//    u32 mode = EDebugModeGL[m_objects[EDebugDraw::eDrawLights]._drawMode];
//
//    CGeometryGL::bindVertexArray(m_objects[EDebugDraw::eDrawLights]._arrayId);
//    if (m_objects[EDebugDraw::eDrawLights]._index.vertex.size() > 0)
//    {
//        CGeometryGL::drawElements(mode, m_objects[EDebugDraw::eDrawLights]._index.vertex.size());
//    }
//    else
//    {
//        CGeometryGL::drawArrays(mode, 0, m_objects[EDebugDraw::eDrawLights]._vertex.vertex.size());
//    }
//    CGeometryGL::bindVertexArray(0);
//}
