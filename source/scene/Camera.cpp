#include "Camera.h"
#include "Engine.h"
#include "utils/Logger.h"

using namespace v3d;
using namespace v3d::scene;

const f32 k_lod = 10000.0f;

CCamera::CCamera()
    : m_target(core::Vector3D(0.0f))
    , m_up(core::Vector3D(0.0f, 1.0f, 0.0f))
    , m_active(false)
    , m_min(-k_lod, -k_lod, -k_lod)
    , m_max(k_lod, k_lod, k_lod)
{
    m_nodeType = ENodeType::eCamera;
    LOG_INFO("Create node type: %s", getNodeNameByType(m_nodeType).c_str());
}

CCamera::~CCamera()
{
}

void CCamera::setTarget(const core::Vector3D& target)
{
    m_target = target;
}

void CCamera::setUpVector(const core::Vector3D& up)
{
    m_up = up;
}

core::Vector3D CCamera::getTarget() const
{
    return m_target;
}

core::Vector3D CCamera::getUpVector() const
{
    return m_up;
}

bool CCamera::isActive() const
{
    return m_active;
}

void CCamera::init()
{
}

void CCamera::render()
{
    if (!m_visible)
    {
        return;
    }
}

void CCamera::update(f64 time)
{
    if (!m_visible)
    {
        return;
    }

    if (m_active)
    {
        RENDERER->updateCamera(m_position, m_target, m_up);
    }
}