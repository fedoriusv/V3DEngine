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

void CRectangleShape::update(s32 dt)
{
    CShape::update(dt);
}

void CRectangleShape::init()
{
    if (m_initialiazed)
    {
        return;
    }

    CShape::init();

    CRectangleShape::build();
    CShape::setGeometryDrawMode(CGeometry::eTriangles);

    CRenderable::getGeometry()->init();
    m_initialiazed = true;
}

void CRectangleShape::refresh()
{
    if (!m_initialiazed)
    {
        return;
    }

    CRectangleShape::build();
    CRenderable::getGeometry()->refresh();
}

void CRectangleShape::build()
{
    SVertexData& data = CShape::getGeometryData();

    Vector2D leftUp = RENDERER->convertPosScreenToCanvas(Point2D(m_rect.getLeftX(), m_rect.getTopY()));
    Vector2D rightDown = RENDERER->convertPosScreenToCanvas(Point2D(m_rect.getRightX(), m_rect.getBottomY()));

    data._vertices =
    {
        { leftUp.x, rightDown.y, 0.0f }, { leftUp.x, leftUp.y, 0.0f }, { rightDown.x, leftUp.y, 0.0f },
        { leftUp.x, rightDown.y, 0.0f }, { rightDown.x, leftUp.y, 0.0f }, { rightDown.x, rightDown.y, 0.0f }
    };

    data._normals =
    {
        { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, 
        { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }
    };

    data._texCoords.resize(1);
    data._texCoords[0] =
    {
        { 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f },
        { 0.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }
    };
}