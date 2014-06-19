#include "TorusShape.h"

using namespace v3d;
using namespace v3d::scene;
using namespace v3d::renderer;

CTorusShape::CTorusShape(const f32 minorRadius, const f32 majorRadius)
    : m_minorRadius(minorRadius)
    , m_majorRadius(majorRadius)
{
    m_shapeType = EShapeType::eTorusShape;
}

CTorusShape::~CTorusShape()
{
}

void CTorusShape::render()
{
    if (!m_visible)
    {
        return;
    }

    m_renderJob->job();
}

void CTorusShape::update(f64 time)
{
    CNode::update(time);
}

void CTorusShape::init()
{
    CShape::init();

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

            data._texCoords.at(0).vertex[index].x = (f32)(i) / (f32)(numMajor);
            data._texCoords.at(0).vertex[index].y = (f32)(j) / (f32)(numMinor);
            data._normals.vertex[index].x = vNormal.x;
            data._normals.vertex[index].y = vNormal.y;
            data._normals.vertex[index].z = vNormal.z;
            data._vertices.vertex[index].x = x0 * r;
            data._vertices.vertex[index].y = y0 * r;
            data._vertices.vertex[index].z = z;

            vNormal = Vector3D(x1*c, y1*c, z / m_minorRadius);
            vNormal.normalize();
            ++index;

            data._texCoords.at(0).vertex[index].x = (f32)(i + 1) / (f32)(numMajor);
            data._texCoords.at(0).vertex[index].y = (f32)(j) / (f32)(numMinor);
            data._normals.vertex[index].x = vNormal.x;
            data._normals.vertex[index].y = vNormal.y;
            data._normals.vertex[index].z = vNormal.z;
            data._vertices.vertex[index].x = x1 * r;
            data._vertices.vertex[index].y = y1 * r;
            data._vertices.vertex[index].z = z;
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

void CTorusShape::setMinorRadius(const f32 radius)
{
    m_minorRadius = radius;
    CTorusShape::init();
}

void CTorusShape::setMajorRadius(const f32 radius)
{
    m_majorRadius = radius;
    CTorusShape::init();
}

f32 CTorusShape::getMinorRadius() const
{
    return m_minorRadius;
}

f32 CTorusShape::getMajorRadius() const
{
    return m_majorRadius;
}