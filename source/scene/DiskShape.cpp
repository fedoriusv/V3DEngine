#include "DiskShape.h"

using namespace v3d;
using namespace scene;
using namespace renderer;

CDiskShape::CDiskShape(f32 minorRadius, f32 majorRadius)
    : m_majorRadius(majorRadius)
    , m_minorRadius(minorRadius)
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

void CDiskShape::update(s32 time)
{
    CShape::update(time);
}

void CDiskShape::init()
{
    CShape::init();
    
    CDiskShape::build();
    CShape::setGeometryDrawMode(CGeometry::eTriangleStrip);

    CRenderable::getGeometry()->init();
    m_initialiazed = true;
}

void CDiskShape::setMinorRadius(f32 radius)
{
    m_minorRadius = radius;
    CDiskShape::refresh();
}

void CDiskShape::setMajorRadius(f32 radius)
{
    m_majorRadius = radius;
    CDiskShape::refresh();
}

f32 CDiskShape::getMinorRadius() const
{
    return m_minorRadius;
}

f32 CDiskShape::getMajorRadius() const
{
    return m_majorRadius;
}

void CDiskShape::build()
{
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
            f32 inner = m_minorRadius + (f32(i)) * stepSizeRadial;
            f32 outer = m_minorRadius + (f32(i + 1)) * stepSizeRadial;

            theyta = stepSizeSlice * f32(j);
            if (j == (slices - 1))
            {
                theytaNext = 0.0f;
            }
            else
            {
                theytaNext = stepSizeSlice * (f32(j + 1));
            }

            // Inner First
            ++index;
            data._vertices[index].x = cos(theyta) * inner;
            data._vertices[index].y = sin(theyta) * inner;
            data._vertices[index].z = 0.0f;
            data._normals[index].x = 0.0f;
            data._normals[index].y = 0.0f;
            data._normals[index].z = 1.0f;
            data._texCoords.at(0)[index].x = (((cos(theyta) * inner) * radialScale) + 1.0f) * 0.5f;
            data._texCoords.at(0)[index].y = (((sin(theyta) * inner) * radialScale) + 1.0f) * 0.5f;


            // Outer First
            ++index;
            data._vertices[index].x = cos(theyta) * outer;
            data._vertices[index].y = sin(theyta) * outer;
            data._vertices[index].z = 0.0f;
            data._normals[index].x = 0.0f;
            data._normals[index].y = 0.0f;
            data._normals[index].z = 1.0f;
            data._texCoords.at(0)[index].x = (((cos(theyta) * outer) * radialScale) + 1.0f) * 0.5f;
            data._texCoords.at(0)[index].y = (((sin(theyta) * outer) * radialScale) + 1.0f) * 0.5f;

            // Inner Second
            ++index;
            data._vertices[index].x = cos(theytaNext) * inner;
            data._vertices[index].y = sin(theytaNext) * inner;
            data._vertices[index].z = 0.0f;
            data._normals[index].x = 0.0f;
            data._normals[index].y = 0.0f;
            data._normals[index].z = 1.0f;
            data._texCoords.at(0)[index].x = (((cos(theytaNext) * inner) * radialScale) + 1.0f) * 0.5f;
            data._texCoords.at(0)[index].y = (((sin(theytaNext) * inner) * radialScale) + 1.0f) * 0.5f;


            // Outer Second
            ++index;
            data._vertices[index].x = cos(theytaNext) * outer;
            data._vertices[index].y = sin(theytaNext) * outer;
            data._vertices[index].z = 0.0f;
            data._normals[index].x = 0.0f;
            data._normals[index].y = 0.0f;
            data._normals[index].z = 1.0f;
            data._texCoords.at(0)[index].x = (((cos(theytaNext) * outer) * radialScale) + 1.0f) * 0.5f;
            data._texCoords.at(0)[index].y = (((sin(theytaNext) * outer) * radialScale) + 1.0f) * 0.5f;
        }
    }
}

void CDiskShape::refresh()
{
    if (!m_initialiazed)
    {
        return;
    }

    CDiskShape::build();
    CRenderable::getGeometry()->refresh();
}
