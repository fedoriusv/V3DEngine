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
    if (!m_visible)
    {
        return;
    }

    m_renderJob->job();
}

void CSampleShape::update(f64 time)
{
    if (!m_visible)
    {
        return;
    }

    CNode::updateTransform(time);
}

void CSampleShape::init()
{
    CShape::init();

    const float triangle[][3] = 
    {
         -1.0f, -1.0f, 0.0f,
          0.0f,  1.0f, 0.0f, 
          1.0f, -1.0f, 0.0f,
    };


    SVertexData& data = CShape::getGeometryData();
    data.malloc(3, 0);

    m_geometry->copyVertices(triangle, 3);
    CShape::setGeometryDrawMode(EDrawMode::eTriangles);

    m_geometry->init();
}