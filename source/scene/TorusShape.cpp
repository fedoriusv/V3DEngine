#include "TorusShape.h"

using namespace v3d;
using namespace core;
using namespace scene;
using namespace renderer;

CTorusShape::CTorusShape(f32 minorRadius, f32 majorRadius)
    : m_majorRadius(majorRadius)
    , m_minorRadius(minorRadius)
{
    m_shapeType = EShapeType::eTorusShape;
}

CTorusShape::~CTorusShape()
{
}

void CTorusShape::render()
{
    CShape::render();
}

void CTorusShape::update(s32 dt)
{
    CShape::update(dt);
}

void CTorusShape::init()
{
    if (m_initialiazed)
    {
        return;
    }

    CShape::init();

    CTorusShape::build();
    CShape::setGeometryDrawMode(eTriangleStrip);

    Renderable::getGeometry()->init();
    m_initialiazed = true;
}

void CTorusShape::setMinorRadius(f32 radius)
{
    m_minorRadius = radius;
    CTorusShape::refresh();
}

void CTorusShape::setMajorRadius(f32 radius)
{
    m_majorRadius = radius;
    CTorusShape::refresh();
}

f32 CTorusShape::getMinorRadius() const
{
    return m_minorRadius;
}

f32 CTorusShape::getMajorRadius() const
{
    return m_majorRadius;
}

void CTorusShape::build()
{
    const u32 numMajor = 50U;
    const u32 numMinor = 20U;

    f64 majorStep = 2.0f * k_pi / numMajor;
    f64 minorStep = 2.0f * k_pi / numMinor;

    SVertexData& data = CShape::getGeometryData();
    data.malloc(numMajor * (numMinor + 1) * 2);

    s32 index = -1;
    for (u32 i = 0; i < numMajor; ++i)
    {
        f64 a0 = i * majorStep;
        f64 a1 = a0 + majorStep;
        f32 x0 = (f32)cos(a0);
        f32 y0 = (f32)sin(a0);
        f32 x1 = (f32)cos(a1);
        f32 y1 = (f32)sin(a1);

        for (u32 j = 0; j <= numMinor; ++j)
        {
            f64 b = j * minorStep;
            f32 c = (f32)cos(b);
            f32 r = m_minorRadius * c + m_majorRadius;
            f32 z = m_minorRadius * (f32)sin(b);

            Vector3D vNormal(x0*c, y0*c, z / m_minorRadius);
            vNormal.normalize();
            ++index;

            data._texCoords.at(0)[index].x = (f32)(i) / (f32)(numMajor);
            data._texCoords.at(0)[index].y = (f32)(j) / (f32)(numMinor);
            data._normals[index].x = vNormal.x;
            data._normals[index].y = vNormal.y;
            data._normals[index].z = vNormal.z;
            data._vertices[index].x = x0 * r;
            data._vertices[index].y = y0 * r;
            data._vertices[index].z = z;

            vNormal = Vector3D(x1*c, y1*c, z / m_minorRadius);
            vNormal.normalize();
            ++index;

            data._texCoords.at(0)[index].x = (f32)(i + 1) / (f32)(numMajor);
            data._texCoords.at(0)[index].y = (f32)(j) / (f32)(numMinor);
            data._normals[index].x = vNormal.x;
            data._normals[index].y = vNormal.y;
            data._normals[index].z = vNormal.z;
            data._vertices[index].x = x1 * r;
            data._vertices[index].y = y1 * r;
            data._vertices[index].z = z;
        }
    }
}

void CTorusShape::refresh()
{
    if (!m_initialiazed)
    {
        return;
    }

    CTorusShape::build();
    Renderable::getGeometry()->refresh();
}
