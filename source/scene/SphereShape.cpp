#include "SphereShape.h"
#include "renderer/Geometry.h"
#include "Engine.h"

using namespace v3d;
using namespace v3d::scene;
using namespace v3d::renderer;

CSphereShape::CSphereShape()
    : m_slices(30U)
    , m_stacks(30U)
    , m_radius(1.0f)
{
    m_shapeType = EShapeType::eSphereShape;
}

CSphereShape::~CSphereShape()
{
}

void CSphereShape::render()
{
    if (!m_visible)
    {
        return;
    }

    m_renderJob->job();
}

void CSphereShape::update(f64 time)
{
    CNode::update(time);
}

void CSphereShape::init()
{
    CShape::init();

    f32 drho = k_pi / m_stacks;
    f32 dtheta = 2.0f * k_pi / m_slices;
    f32 ds = 1.0f / (f32)m_slices;
    f32 dt = 1.0f / (f32)m_stacks;
    f32 t = 1.0f;
    f32 s = 0.0f;

    SVertexData& data = CShape::getGeometryData();
    data.malloc(m_slices * m_stacks * 4);

    s32 index = -1;
    for (u32 i = 0; i < m_stacks; ++i)
    {
        f32 rho = (f32)i * drho;
        f32 srho = (f32)(sin(rho));
        f32 crho = (f32)(cos(rho));
        f32 srhodrho = (f32)(sinf(rho + drho));
        f32 crhodrho = (f32)(cosf(rho + drho));

        s = 0.0f;
        for (u32 j = 0; j < m_slices; j++)
        {
            f32 theta = (j == m_slices) ? 0.0f : j * dtheta;
            f32 stheta = (f32)(-sin(theta));
            f32 ctheta = (f32)(cos(theta));

            f32 x = stheta * srho;
            f32 y = ctheta * srho;
            f32 z = crho;

            ++index;
            data._texCoords.at(0).vertex[index].x = s;
            data._texCoords.at(0).vertex[index].y = t;
            data._normals.vertex[index].x = x;
            data._normals.vertex[index].y = y ;
            data._normals.vertex[index].z = z;
            data._vertices.vertex[index].x = x * m_radius;
            data._vertices.vertex[index].y = y * m_radius;
            data._vertices.vertex[index].z = z * m_radius;

            x = stheta * srhodrho;
            y = ctheta * srhodrho;
            z = crhodrho;

            ++index;
            data._texCoords.at(0).vertex[index].x = s;
            data._texCoords.at(0).vertex[index].y = t - dt;
            data._normals.vertex[index].x = x;
            data._normals.vertex[index].y = y;
            data._normals.vertex[index].z = z;
            data._vertices.vertex[index].x = x * m_radius;
            data._vertices.vertex[index].y = y * m_radius;
            data._vertices.vertex[index].z = z * m_radius;

            theta = ((j + 1) == m_slices) ? 0.0f : (j + 1) * dtheta;
            stheta = (f32)(-sin(theta));
            ctheta = (f32)(cos(theta));

            x = stheta * srho;
            y = ctheta * srho;
            z = crho;

            s += ds;
            ++index;
            data._texCoords.at(0).vertex[index].x = s;
            data._texCoords.at(0).vertex[index].y = t;
            data._normals.vertex[index].x = x;
            data._normals.vertex[index].y = y;
            data._normals.vertex[index].z = z;
            data._vertices.vertex[index].x =  x * m_radius;
            data._vertices.vertex[index].y  = y * m_radius;
            data._vertices.vertex[index].z = z * m_radius;

            x = stheta * srhodrho;
            y = ctheta * srhodrho;
            z = crhodrho;

            ++index;
            data._texCoords.at(0).vertex[index].x = s;
            data._texCoords.at(0).vertex[index].y = t - dt;
            data._normals.vertex[index].x = x;
            data._normals.vertex[index].y = y;
            data._normals.vertex[index].z = z;
            data._vertices.vertex[index].x = x * m_radius;
            data._vertices.vertex[index].y = y * m_radius;
            data._vertices.vertex[index].z = z * m_radius;
        }
        t -= dt;
    }

    CShape::setGeometryDrawMode(EDrawMode::eTriangleStrip);

    m_geometry->init();
}

void CSphereShape::setSlices(const u32 value)
{
    m_slices = value;
}

void CSphereShape::setStacks(const u32 value)
{
    m_stacks = value;
}

void CSphereShape::setRadius(const f32 radius)
{
    m_radius = radius;
}

u32 CSphereShape::getSlices() const
{
    return m_slices;
}

u32 CSphereShape::getStacks() const
{
    return m_stacks;
}

f32 CSphereShape::getRadius() const
{
    return m_radius;
}