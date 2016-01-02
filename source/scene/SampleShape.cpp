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

void CSampleShape::update(s32 dt)
{
    CShape::update(dt);
}

void CSampleShape::init()
{
    if (m_initialiazed)
    {
        return;
    }

    CShape::init();

    CSampleShape::build();
    CShape::setGeometryDrawMode(eTriangles);

    CRenderable::getGeometry()->init();
    m_initialiazed = true;
}

void CSampleShape::build()
{
    SVertexData& data = CShape::getGeometryData();

    data._vertices =
    {
        {-1.0f, -1.0f, 0.0f},
        { 0.0f,  1.0f, 0.0f},
        { 1.0f, -1.0f, 0.0f},
    };

    data._normals =
    {
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f},
    };

    data._texCoords.resize(1);
    data._texCoords[0] =
    {
        {0.0f, 0.0f},
        {0.5f, 1.0f},
        {1.0f, 0.0f},
    };
}

void CSampleShape::refresh()
{
    if (!m_initialiazed)
    {
        return;
    }

    CSampleShape::build();
    CRenderable::getGeometry()->refresh();
}
