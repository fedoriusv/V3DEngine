#include "SphereShape.h"
#include "renderer/Geometry.h"
#include "Engine.h"

using namespace v3d;
using namespace core;
using namespace scene;
using namespace renderer;

CSphereShape::CSphereShape(f32 radius)
    : m_radius(radius)
{
    m_shapeType = EShapeType::eSphereShape;
}

CSphereShape::~CSphereShape()
{
}

void CSphereShape::render()
{
    CShape::render();
}

void CSphereShape::update(s32 dt)
{
    CShape::update(dt);
}

void CSphereShape::init()
{
    if (m_initialiazed)
    {
        return;
    }

    CShape::init();

    CSphereShape::build();
    CShape::setGeometryDrawMode(eTriangleStrip);

    CRenderable::getGeometry()->init();
    m_initialiazed = true;
}

void CSphereShape::setRadius(const f32 radius)
{
    m_radius = radius;
    CSphereShape::refresh();
}

f32 CSphereShape::getRadius() const
{
    return m_radius;
}

void CSphereShape::build()
{
    const u32 slices = 20U;
    const u32 stacks = 15U;

    f32 drho = k_pi / stacks;
    f32 dtheta = 2.0f * k_pi / slices;
    f32 ds = 1.0f / (f32)slices;
    f32 dt = 1.0f / (f32)stacks;
    f32 t = 1.0f;
    f32 s = 0.0f;

    SVertexData& data = CShape::getGeometryData();
    data.malloc(slices * stacks * 4);

    s32 index = -1;
    for (u32 i = 0; i < stacks; ++i)
    {
        f32 rho = (f32)i * drho;
        f32 srho = (f32)(sin(rho));
        f32 crho = (f32)(cos(rho));
        f32 srhodrho = (f32)(sinf(rho + drho));
        f32 crhodrho = (f32)(cosf(rho + drho));

        s = 0.0f;
        for (u32 j = 0; j < slices; j++)
        {
            f32 theta = (j == slices) ? 0.0f : j * dtheta;
            f32 stheta = (f32)(-sin(theta));
            f32 ctheta = (f32)(cos(theta));

            f32 x = stheta * srho;
            f32 y = ctheta * srho;
            f32 z = crho;

            ++index;
            data._texCoords.at(0)[index].x = s;
            data._texCoords.at(0)[index].y = t;
            data._normals[index].x = x;
            data._normals[index].y = y;
            data._normals[index].z = z;
            data._vertices[index].x = x * m_radius;
            data._vertices[index].y = y * m_radius;
            data._vertices[index].z = z * m_radius;

            x = stheta * srhodrho;
            y = ctheta * srhodrho;
            z = crhodrho;

            ++index;
            data._texCoords.at(0)[index].x = s;
            data._texCoords.at(0)[index].y = t - dt;
            data._normals[index].x = x;
            data._normals[index].y = y;
            data._normals[index].z = z;
            data._vertices[index].x = x * m_radius;
            data._vertices[index].y = y * m_radius;
            data._vertices[index].z = z * m_radius;

            theta = ((j + 1) == slices) ? 0.0f : (j + 1) * dtheta;
            stheta = (f32)(-sin(theta));
            ctheta = (f32)(cos(theta));

            x = stheta * srho;
            y = ctheta * srho;
            z = crho;

            s += ds;
            ++index;
            data._texCoords.at(0)[index].x = s;
            data._texCoords.at(0)[index].y = t;
            data._normals[index].x = x;
            data._normals[index].y = y;
            data._normals[index].z = z;
            data._vertices[index].x = x * m_radius;
            data._vertices[index].y = y * m_radius;
            data._vertices[index].z = z * m_radius;

            x = stheta * srhodrho;
            y = ctheta * srhodrho;
            z = crhodrho;

            ++index;
            data._texCoords.at(0)[index].x = s;
            data._texCoords.at(0)[index].y = t - dt;
            data._normals[index].x = x;
            data._normals[index].y = y;
            data._normals[index].z = z;
            data._vertices[index].x = x * m_radius;
            data._vertices[index].y = y * m_radius;
            data._vertices[index].z = z * m_radius;
        }
        t -= dt;
    }
}

void CSphereShape::refresh()
{
    if (!m_initialiazed)
    {
        return;
    }

    CSphereShape::build();
    CRenderable::getGeometry()->refresh();
}
