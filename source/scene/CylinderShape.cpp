#include "CylinderShape.h"

using namespace v3d;
using namespace core;
using namespace scene;
using namespace renderer;

CCylinderShape::CCylinderShape(f32 radius, f32 height)
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
    CShape::render();
}

void CCylinderShape::update(s32 dt)
{
    CShape::update(dt);
}

void CCylinderShape::init()
{
    if (m_initialiazed)
    {
        return;
    }

    CShape::init();

    CCylinderShape::build();
    CShape::setGeometryDrawMode(eTriangleStrip);

    Renderable::getGeometry()->init();
    m_initialiazed = true;
}

void CCylinderShape::setRadius(f32 radius)
{
    m_radius = radius;
    CCylinderShape::refresh();
}

void CCylinderShape::setHeight(f32 height)
{
    m_height = height;
    CCylinderShape::refresh();
}

f32 CCylinderShape::getRadius() const
{
    return m_radius;
}

f32 CCylinderShape::getHeight() const
{
    return m_height;
}

void CCylinderShape::refresh()
{
    if (!m_initialiazed)
    {
        return;
    }

    CCylinderShape::build();
    Renderable::getGeometry()->refresh();
}

void CCylinderShape::build()
{
    const u32 major = 20U;
    const u32 minor = 20U;

    f64 majorStep = (f64)m_height / major;
    f64 minorStep = 2.0 * k_pi / minor;

    SVertexData& data = CShape::getGeometryData();
    data.malloc(major * (minor + 1) * 2);

    s32 index = -1;
    for (u32 i = 0; i < major; ++i)
    {
        f32 z0 = 0.5f * m_height - i * (f32)majorStep;
        f32 z1 = z0 - (f32)majorStep;

        for (u32 j = 0; j <= minor; ++j)
        {
            f64 a = j * minorStep;
            f32 x = (f32)(m_radius * cos(a));
            f32 y = (f32)(m_radius * sin(a));

            ++index;
            data._texCoords.at(0)[index].x = j / (f32)minor;
            data._texCoords.at(0)[index].y = i / (f32)major;
            data._normals[index].x = x / (f32)m_radius;
            data._normals[index].y = y / (f32)m_radius;
            data._normals[index].z = 0.0f;
            data._vertices[index].x = x;
            data._vertices[index].y = y;
            data._vertices[index].z = z0;

            ++index;
            data._texCoords.at(0)[index].x = j / (f32)minor;
            data._texCoords.at(0)[index].y = (i + 1) / (f32)major;
            data._normals[index].x = x / (f32)m_radius;
            data._normals[index].y = y / (f32)m_radius;
            data._normals[index].z = 0.0f;
            data._vertices[index].x = x;
            data._vertices[index].y = y;
            data._vertices[index].z = z1;
        }
    }
}
