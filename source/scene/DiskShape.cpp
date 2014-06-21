#include "DiskShape.h"

using namespace v3d;
using namespace v3d::scene;
using namespace v3d::renderer;

CDiskShape::CDiskShape(const f32 minorRadius, const f32 majorRadius)
    : m_minorRadius(minorRadius)
    , m_majorRadius(majorRadius)
{
    m_shapeType = EShapeType::eDiskShape;
}

CDiskShape::~CDiskShape()
{
}

void CDiskShape::render()
{
    CShape::render();
}

void CDiskShape::update(f64 time)
{
    CNode::update(time);
}

void CDiskShape::init()
{
    CShape::init();
    
    const u32 slices = 20U;
    const u32 stacks = 3U;

    f32 stepSizeRadial = m_majorRadius - m_minorRadius;
    if (stepSizeRadial < 0.0f)
    {
        stepSizeRadial *= -1.0f;
    }

    stepSizeRadial /= f32(stacks);

    f32 stepSizeSlice = (k_pi * 2.0f) / (f32)slices;

    SVertexData& data = CShape::getGeometryData();
    data.malloc(slices * stacks * 4);

    f32 radialScale = 1.0f / m_majorRadius;
    s32 index = -1;
    for (u32 i = 0; i < stacks; ++i) // Stacks
    {
        f32 theyta;
        f32 theytaNext;
        for (u32 j = 0; j < slices; ++j) // Slices
        {
            float inner = m_minorRadius + (f32(i)) * stepSizeRadial;
            float outer = m_minorRadius + (f32(i + 1)) * stepSizeRadial;

            theyta = stepSizeSlice * f32(j);
            if (j == (slices - 1))
                theytaNext = 0.0f;
            else
                theytaNext = stepSizeSlice * (f32(j + 1));

            // Inner First
            ++index;
            data._vertices.vertex[index].x = cos(theyta) * inner;
            data._vertices.vertex[index].y = sin(theyta) * inner;
            data._vertices.vertex[index].z = 0.0f;
            data._normals.vertex[index].x = 0.0f;
            data._normals.vertex[index].y = 0.0f;
            data._normals.vertex[index].z = 1.0f;
            data._texCoords.at(0).vertex[index].x = (((cos(theyta) * inner) * radialScale) + 1.0f) * 0.5f;
            data._texCoords.at(0).vertex[index].y = (((sin(theyta) * inner) * radialScale) + 1.0f) * 0.5f;


            // Outer First
            ++index;
            data._vertices.vertex[index].x = cos(theyta) * outer;
            data._vertices.vertex[index].y = sin(theyta) * outer;
            data._vertices.vertex[index].z = 0.0f;
            data._normals.vertex[index].x = 0.0f;
            data._normals.vertex[index].y = 0.0f;
            data._normals.vertex[index].z = 1.0f;
            data._texCoords.at(0).vertex[index].x = (((cos(theyta) * outer) * radialScale) + 1.0f) * 0.5f;
            data._texCoords.at(0).vertex[index].y = (((sin(theyta) * outer) * radialScale) + 1.0f) * 0.5f;

            // Inner Second
            ++index;
            data._vertices.vertex[index].x = cos(theytaNext) * inner;
            data._vertices.vertex[index].y = sin(theytaNext) * inner;
            data._vertices.vertex[index].z = 0.0f;
            data._normals.vertex[index].x = 0.0f;
            data._normals.vertex[index].y = 0.0f;
            data._normals.vertex[index].z = 1.0f;
            data._texCoords.at(0).vertex[index].x = (((cos(theytaNext) * inner) * radialScale) + 1.0f) * 0.5f;
            data._texCoords.at(0).vertex[index].y = (((sin(theytaNext) * inner) * radialScale) + 1.0f) * 0.5f;


            // Outer Second
            ++index;
            data._vertices.vertex[index].x = cos(theytaNext) * outer;
            data._vertices.vertex[index].y = sin(theytaNext) * outer;
            data._vertices.vertex[index].z = 0.0f;
            data._normals.vertex[index].x = 0.0f;
            data._normals.vertex[index].y = 0.0f;
            data._normals.vertex[index].z = 1.0f;
            data._texCoords.at(0).vertex[index].x = (((cos(theytaNext) * outer) * radialScale) + 1.0f) * 0.5f;
            data._texCoords.at(0).vertex[index].y = (((sin(theytaNext) * outer) * radialScale) + 1.0f) * 0.5f;
        }
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

void CDiskShape::setMinorRadius(const f32 radius)
{
    m_minorRadius = radius;
    CDiskShape::init();
}

void CDiskShape::setMajorRadius(const f32 radius)
{
    m_majorRadius = radius;
    CDiskShape::init();
}

f32 CDiskShape::getMinorRadius() const
{
    return m_minorRadius;
}

f32 CDiskShape::getMajorRadius() const
{
    return m_majorRadius;
}
