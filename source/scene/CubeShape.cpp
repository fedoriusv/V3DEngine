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

void CCubeShape::update(s32 dt)
{
    CShape::update(dt);
}

void CCubeShape::init()
{
    if (m_initialiazed)
    {
        return;
    }

    CShape::init();

    CCubeShape::build();
    CShape::setGeometryDrawMode(eTriangles);

    CRenderable::getGeometry()->init();
    m_initialiazed = true;
}

void CCubeShape::build()
{
    f32 s = k_extend;
    SVertexData& data = CShape::getGeometryData();

    data._vertices =
    {
        { -s, s, s }, { s, s, s }, { s, -s, s }, { -s, -s, s },
        { s, s, -s }, { -s, s, -s }, { -s, -s, -s }, { s, -s, -s },
        { -s, s, -s }, { s, s, -s }, { s, s, s }, { -s, s, s },
        { s, -s, -s }, { -s, -s, -s }, { -s, -s, s }, { s, -s, s },
        { -s, s, -s }, { -s, s, s }, { -s, -s, s }, { -s, -s, -s },
        { s, s, s }, { s, s, -s }, { s, -s, -s }, { s, -s, s }
    };

    data._normals =
    {
        { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -1.0f },
        { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f },
        { 0.0f, -1.0f, 0.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, -1.0f, 0.0f },
        { -1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f },
        { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }
    };

    data._texCoords.resize(1);
    data._texCoords[0] =
    {
        { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f },
        { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f },
        { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f },
        { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f },
        { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f },
        { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f }
    };

    data._indices =
    {
        0, 3, 1, 1, 3, 2,       // front
        4, 7, 5, 5, 7, 6,       // back
        8, 11, 9, 9, 11, 10,    // top
        12, 15, 13, 13, 15, 14, // bottom
        16, 19, 17, 17, 19, 18, // left
        20, 23, 21, 21, 23, 22  // right
    };

    data.computeTangentBasis();
}
