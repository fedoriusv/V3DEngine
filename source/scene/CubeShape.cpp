#include "CubeShape.h"
#include "renderer/Geometry.h"
#include "Engine.h"

using namespace v3d;
using namespace v3d::scene;
using namespace v3d::renderer;

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

void CCubeShape::update(f64 time)
{
    CShape::update(time);
}

void CCubeShape::init()
{
    CShape::init();

    CCubeShape::build();
    CShape::setGeometryDrawMode(CGeometry::eTriangles);

    m_geometry->init();
#ifdef _DEBUG
    m_debug->init();
#endif

    m_initialiazed = true;
}

void CCubeShape::refresh()
{
    CCubeShape::build();

    m_geometry->refresh();
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
        { -s, -s, s }, { s, -s, s }, { s, s, s }, { -s, s, s },
        { -s, -s, -s }, { -s, s, -s }, { s, s, -s }, { s, -s, -s },
        { -s, s, -s }, { -s, s, s }, { s, s, s }, { s, s, -s },
        { -s, -s, -s }, { s, -s, -s }, { s, -s, s }, { -s, -s, s },
        { s, -s, -s }, { s, s, -s }, { s, s, s }, { s, -s, s },
        { -s, -s, -s }, { -s, -s, s }, { -s, s, s }, { -s, s, -s }
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

    m_geometry->copyToVertices(vertex, count);
    m_geometry->copyToNormals(normals, count);
    m_geometry->copyToTexCoords(texCoord, 0, count);
    m_geometry->copyToIndices(indices, 36);
}