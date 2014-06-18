#include "CylinderShape.h"

using namespace v3d;
using namespace v3d::scene;
using namespace v3d::renderer;

CCylinderShape::CCylinderShape(const float radius, const float height)
    : m_radius(radius)
    , m_height(height)
{
    m_shapeType = EShapeType::eCylinderShape;
}

CCylinderShape::~CCylinderShape()
{
}

void CCylinderShape::render()
{
    if (!m_visible)
    {
        return;
    }

    m_renderJob->job();
}

void CCylinderShape::update(f64 time)
{
    CNode::update(time);
}

void CCylinderShape::init()
{
    CShape::init();

    u32 major = 20;
    u32 minor = 20;
    f64 radius = m_radius;

    f64 majorStep = (f64)m_height / major;
    f64 minorStep = 2.0 * k_pi / minor;

    SVertexData& data = CShape::getGeometryData();
    data.malloc(major * (minor + 1) * 2);

    int index = -1;
    for (u32 i = 0; i < major; ++i)
    {
        f32 z0 = 0.5f * m_height - i * (f32)majorStep;
        f32 z1 = z0 - (f32)majorStep;

        for (u32 j = 0; j <= minor; ++j)
        {
            f64 a = j * minorStep;
            f32 x = (f32)(radius * cos(a));
            f32 y = (f32)(radius * sin(a));

            ++index;
            data._texCoords.at(0).vertex[index].x = j / (f32)minor;
            data._texCoords.at(0).vertex[index].y = i / (f32)major;
            data._normals.vertex[index].x =  x / (f32)radius;
            data._normals.vertex[index].y =  y / (f32)radius;
            data._normals.vertex[index].z = 0.0f;
            data._vertices.vertex[index].x = x;
            data._vertices.vertex[index].y = y;
            data._vertices.vertex[index].z = z0;

            ++index;
            data._texCoords.at(0).vertex[index].x = j / (f32)minor;
            data._texCoords.at(0).vertex[index].y = (i + 1) / (f32)major;
            data._normals.vertex[index].x = x / (f32)radius;
            data._normals.vertex[index].y = y / (f32)radius;
            data._normals.vertex[index].z = 0.0f;
            data._vertices.vertex[index].x = x;
            data._vertices.vertex[index].y = y;
            data._vertices.vertex[index].z = z1;
        }
    }

    CShape::setGeometryDrawMode(EDrawMode::eTriangleStrip);

    if (data._vertices.id == 0)
    {
        m_geometry->init();
    }
    else
    {
        m_geometry->refresh();
    }
}

void CCylinderShape::setRadius(const f32 radius)
{
    m_radius = radius;
    CCylinderShape::init();
}

void CCylinderShape::setHeight(const f32 height)
{
    m_height = height;
    CCylinderShape::init();
}

f32 CCylinderShape::getRadius() const
{
    return m_radius;
}

f32 CCylinderShape::getHeight() const
{
    return m_height;
}
