#include "SquareShape.h"
#include "renderer/Geometry.h"
#include "Engine.h"

using namespace v3d;
using namespace scene;
using namespace renderer;

CSquareShape::CSquareShape()
{
    m_shapeType = EShapeType::eSquareShape;
}

CSquareShape::~CSquareShape()
{
}

void CSquareShape::render()
{
    CShape::render();
}

void CSquareShape::update(f64 time)
{
    CShape::update(time);
}

void CSquareShape::init()
{
    CShape::init();

    CSquareShape::build();
    CShape::setGeometryDrawMode(CGeometry::eTriangles);

    m_geometry->init();
#ifdef _DEBUG
    m_debug->init();
#endif

    m_initialiazed = true;
}

void CSquareShape::refresh()
{
    if (!m_initialiazed)
    {
        return;
    }

    CSquareShape::build();

    m_geometry->refresh();
#ifdef _DEBUG
    m_debug->refresh();
#endif
}

void CSquareShape::build()
{
    const u32 count = 6;

    const f32 vertex[][3] =
    {
        { -1.0f, -1.0f, 0.0f }, { 1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 0.0f },
        { -1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 0.0f }, { -1.0, 1.0f, 0.0f }
    };

    const f32 normals[][3] =
    {
        { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -1.0f },
    };

    const f32 texCoord[][2] =
    {
        { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f },
        { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f },
    };

    SVertexData& data = CShape::getGeometryData();
    data.malloc(count, 0);

    m_geometry->copyToVertices(vertex, count);
    m_geometry->copyToNormals(normals, count);
    m_geometry->copyToTexCoords(texCoord, 0, count);
}