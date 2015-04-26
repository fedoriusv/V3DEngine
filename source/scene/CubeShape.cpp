#include "CubeShape.h"
#include "renderer/Geometry.h"
#include "Engine.h"

using namespace v3d;
using namespace scene;
using namespace renderer;

CCubeShape::CCubeShape()
{
	m_shapeType = EShapeType::eCubeShape;
}

CCubeShape::~CCubeShape()
{
}

void CCubeShape::render()
{
    CShape::render();
}

void CCubeShape::update(s32 time)
{
    CShape::update(time);
}

void CCubeShape::init()
{
    CShape::init();

    CCubeShape::build();
    CShape::setGeometryDrawMode(CGeometry::eTriangles);

    CRendereble::getGeometry()->init();
#ifdef _DEBUG
    m_debug->init();
#endif

    m_initialiazed = true;
}

void CCubeShape::refresh()
{
    if (!m_initialiazed)
    {
        return;
    }

    CCubeShape::build();

    CRendereble::getGeometry()->refresh();
#ifdef _DEBUG
    m_debug->refresh();
#endif
}

void CCubeShape::build()
{
    f32 s = 0.5f;
    u32 count = 24;

    const f32 vertex[][3] =
    {
        { -s, -s,  s }, {  s, -s,  s }, {  s,  s,  s }, { -s,  s,  s },
        { -s, -s, -s }, { -s,  s, -s }, {  s,  s, -s }, {  s, -s, -s },
        { -s,  s, -s }, { -s,  s,  s }, {  s,  s,  s }, {  s,  s, -s },
        { -s, -s, -s }, {  s, -s, -s }, {  s, -s,  s }, { -s, -s,  s },
        {  s, -s, -s }, {  s,  s, -s }, {  s,  s,  s }, {  s, -s,  s },
        { -s, -s, -s }, { -s, -s,  s }, { -s,  s,  s }, { -s,  s, -s }
    };

    const f32 normals[][3] =
    {
        { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -1.0f },
        { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f },
        { 0.0f, -1.0f, 0.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, -1.0f, 0.0f },
        { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f },
        { -1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }
    };

    const f32 texCoord[][2] =
    {
        { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f },
        { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f },
        { 0.0f, 1.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f },
        { 1.0f, 1.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f },
        { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f },
        { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f }
    };

    const u32 indices[] =
    {
        0, 3, 1, 1, 3, 2,	// front
        4, 7, 5, 5, 7, 6,	// back
        8, 11, 9, 9, 11, 10, // top
        12, 15, 13, 13, 15, 14, // bottom
        16, 19, 17, 17, 19, 18, // left
        20, 23, 21, 21, 23, 22  // right
    };


    SVertexData& data = CShape::getGeometryData();
    data.malloc(count, 36);

    CRendereble::getGeometry()->copyToVertices(vertex, count);
    CRendereble::getGeometry()->copyToNormals(normals, count);
    CRendereble::getGeometry()->copyToTexCoords(texCoord, 0, count);
    CRendereble::getGeometry()->copyToIndices(indices, 36);
}
