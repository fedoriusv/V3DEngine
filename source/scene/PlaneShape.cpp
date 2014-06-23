#include "PlaneShape.h"

using namespace v3d;
using namespace v3d::scene;
using namespace v3d::renderer;


CPlaneShape::CPlaneShape(const f32 extent)
    : m_extent(extent)
{
    m_shapeType = EShapeType::ePlaneShape;
}

CPlaneShape::~CPlaneShape()
{
}

void CPlaneShape::render()
{
    CShape::render();
}

void CPlaneShape::update(f64 time)
{
    CShape::update(time);
}

void CPlaneShape::init()
{
    CShape::init();

    f32 step = 1.0f;
    f32 y = 0.0f;

    f32 s = 0.0f;
    f32 t = 0.0f;
    f32 texStep = 1.0f / (m_extent * 0.075f);

    SVertexData& data = CShape::getGeometryData();
    data.malloc((s32)(2 * (2 * m_extent + 1) * (2 * m_extent + 1)));

    s32 index = -1;
    for (f32 strip = -m_extent; strip <= m_extent; strip += step)
    {
        t = 0.0f;
        for (f32 run = m_extent; run >= -m_extent; run -= step)
        {
            ++index;

            data._texCoords.at(0).vertex[index].x = s;
            data._texCoords.at(0).vertex[index].y = t;
            data._normals.vertex[index].x = 0.0f;
            data._normals.vertex[index].y = 1.0f;
            data._normals.vertex[index].z = 0.0f;
            data._vertices.vertex[index].x = strip;
            data._vertices.vertex[index].y = y;
            data._vertices.vertex[index].z = run;

            ++index;
            
            data._texCoords.at(0).vertex[index].x = s + texStep;
            data._texCoords.at(0).vertex[index].y = t;
            data._normals.vertex[index].x = 0.0f;
            data._normals.vertex[index].y = 1.0f;
            data._normals.vertex[index].z = 0.0f;
            data._vertices.vertex[index].x = strip + step;
            data._vertices.vertex[index].y = y;
            data._vertices.vertex[index].z = run;

            t += texStep;
        }
        s += texStep;
    }

    CShape::setGeometryDrawMode(EDrawMode::eTriangleStrip);

    if (data._vertices.id == 0)
    {
        m_geometry->init();
#ifdef _DEBUG
        m_debug->init();
#endif
    }
    else
    {
        m_geometry->refresh();
#ifdef _DEBUG
        m_debug->refresh();
#endif
    }
}

void CPlaneShape::setExtent(const f32 extent)
{
    m_extent = extent;
    CPlaneShape::init();
}

f32 CPlaneShape::getExtent() const
{
    return m_extent;
}
