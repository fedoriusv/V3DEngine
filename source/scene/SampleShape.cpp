#include "SampleShape.h"
#include "renderer/Geometry.h"
#include "Engine.h"

using namespace v3d;
using namespace scene;
using namespace renderer;

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

void CSampleShape::update(s32 time)
{
    CShape::update(time);
}

void CSampleShape::init()
{
    CShape::init();

    CSampleShape::build();
    CShape::setGeometryDrawMode(CGeometry::eTriangles);

    CRendereble::getGeometry()->init();
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

    CRendereble::getGeometry()->copyToVertices(vertex, 3);
    CRendereble::getGeometry()->copyToNormals(normal, 3);
    CRendereble::getGeometry()->copyToTexCoords(texCoord, 0, 3);
}

void CSampleShape::refresh()
{
    if (!m_initialiazed)
    {
        return;
    }

    CSampleShape::build();

    CRendereble::getGeometry()->refresh();
#ifdef _DEBUG
    m_debug->refresh();
#endif
}
