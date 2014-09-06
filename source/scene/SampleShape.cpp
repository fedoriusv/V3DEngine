#include "SampleShape.h"
#include "renderer/Geometry.h"
#include "Engine.h"

using namespace v3d;
using namespace v3d::scene;
using namespace v3d::renderer;

CSampleShape::CSampleShape()
{
    m_shapeType = EShapeType::eSampleShape;
}

CSampleShape::~CSampleShape()
{
}

void CSampleShape::render()
{
    CShape::render();
}

void CSampleShape::update(f64 time)
{
    CShape::update(time);
}

void CSampleShape::init()
{
    CShape::init();

    CSampleShape::build();
    CShape::setGeometryDrawMode(CGeometry::eTriangles);

    m_geometry->init();
#ifdef _DEBUG
    m_debug->init();
#endif

    m_initialiazed = true;
}

void CSampleShape::build()
{
    const f32 vertex[][3] =
    {
        -1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
    };

    const f32 normal[][3] =
    {
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
    };

    const f32 texCoord[][2] =
    {
        0.0f, 0.0f,
        0.5f, 1.0f,
        1.0f, 0.0f,
    };


    SVertexData& data = CShape::getGeometryData();
    data.malloc(3, 0);

    m_geometry->copyToVertices(vertex, 3);
    m_geometry->copyToNormals(normal, 3);
    m_geometry->copyToTexCoords(texCoord, 0, 3);
}

void CSampleShape::refresh()
{
    if (!m_initialiazed)
    {
        return;
    }

    CSampleShape::build();

    m_geometry->refresh();
#ifdef _DEBUG
    m_debug->refresh();
#endif
}