#include "RectangleShape.h"
#include "renderer/Geometry.h"
#include "Engine.h"

using namespace v3d;
using namespace core;
using namespace scene;
using namespace renderer;

CRectangleShape::CRectangleShape(const Rect32& rect)
: m_rect(rect)
{
    m_shapeType = EShapeType::eRectangleShape;
}

CRectangleShape::~CRectangleShape()
{
}

void CRectangleShape::render()
{
    CShape::render();
}

void CRectangleShape::update(s32 time)
{
    CShape::update(time);
}

void CRectangleShape::init()
{
    CShape::init();

    CRectangleShape::build();
    CShape::setGeometryDrawMode(CGeometry::eTriangles);

    CRendereble::getGeometry()->init();
#ifdef _DEBUG
    m_debug->init();
#endif

    m_initialiazed = true;
}

void CRectangleShape::refresh()
{
    if (!m_initialiazed)
    {
        return;
    }

    CRectangleShape::build();

    CRendereble::getGeometry()->refresh();
#ifdef _DEBUG
    m_debug->refresh();
#endif
}

void CRectangleShape::build()
{
    const u32 count = 6;

    Vector2D leftUp = RENDERER->convertPosScreenToCanvas(Point2D(m_rect.getLeftX(), m_rect.getTopY()));
    Vector2D rightDown = RENDERER->convertPosScreenToCanvas(Point2D(m_rect.getRightX(), m_rect.getBottomY()));

    const f32 vertex[][3] =
    {
        { leftUp.x, rightDown.y, 0.0f }, { leftUp.x, leftUp.y, 0.0f }, { rightDown.x, leftUp.y, 0.0f },
        { leftUp.x, rightDown.y, 0.0f }, { rightDown.x, leftUp.y, 0.0f }, { rightDown.x, rightDown.y, 0.0f }
    };

    const f32 normals[][3] =
    {
        { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, 
        { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }
    };

    const f32 texCoord[][2] =
    {
        { 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f },
        { 0.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }
    };

    SVertexData& data = CShape::getGeometryData();
    data.malloc(count, 0);

    CRendereble::getGeometry()->copyToVertices(vertex, count);
    CRendereble::getGeometry()->copyToNormals(normals, count);
    CRendereble::getGeometry()->copyToTexCoords(texCoord, 0, count);
}