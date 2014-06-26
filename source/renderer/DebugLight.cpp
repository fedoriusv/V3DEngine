#include "DebugLight.h"
#include "Geometry.h"

using namespace v3d;
using namespace v3d::core;
using namespace v3d::renderer;

CDebugLight::CDebugLight(const core::Vector3D& position, const scene::SLightData& data)
    : m_data(data)
    , m_position(position)
    , m_flag(EDebugLightFlag::eLightFlagNone)
{
}

CDebugLight::~CDebugLight()
{
}

void CDebugLight::setDebugFlag(s32 flag)
{
    m_flag = flag;
}

void CDebugLight::init()
{
}

void CDebugLight::refresh()
{
}

void CDebugLight::bind()
{
}

void CDebugLight::initShader()
{
}

void CDebugLight::initLightPosition()
{
    const f32 s = 0.3f;
    const f32 vertex[][3] =
    {
        { -s, -s, s  }, { s, -s, s  }, { s, s, s  }, { -s, s, s  },
        { -s, -s, -s }, { -s, s, -s }, { s, s, -s }, { s, -s, -s },
        { -s, s, -s  }, { -s, s, s  }, { s, s, s  }, { s, s, -s  },
        { -s, -s, -s }, { s, -s, -s }, { s, -s, s }, { -s, -s, s },
        { s, -s, -s  }, { s, s, -s  }, { s, s, s  }, { s, -s, s  },
        { -s, -s, -s }, { -s, -s, s }, { -s, s, s }, { -s, s, -s }
    };

    const u32 indices[] =
    {
        0,  3,  1,  1,  3,  2,
        4,  7,  5,  5,  7,  6,
        8,  11, 9,  9,  11, 10,
        12, 15, 13, 13, 15, 14,
        16, 19, 17, 17, 19, 18,
        20, 23, 21, 21, 23, 22
    };

    SVertices<core::Vector3D>& light = m_objects[EDebugLight::eDebugLightPosition]._vertex;
    SVertices<u32>& lightIdx = m_objects[EDebugLight::eDebugLightPosition]._index;
    m_objects[EDebugLight::eDebugLightPosition]._drawMode = EDrawMode::eTriangles;

    light.vertex.clear();
    light.vertex.resize(24);

    for (u32 i = 0; i < 24; ++i)
    {
        light.vertex[i] = vertex[i];
    }

    lightIdx.vertex.clear();
    lightIdx.vertex.resize(36);
    std::copy(indices, indices + 36, lightIdx.vertex.begin());
}
