#include "Camera.h"
#include "utils/Logger.h"
#include "Engine.h"

using namespace v3d;
using namespace scene;
using namespace stream;

CCamera::CCamera()
    : m_orthogonal(false)
    , m_zNear(0.5f)
    , m_zFar(100.0f)
    , m_active(false)
    , m_up(core::Vector3D(0.0f, 1.0f, 0.0f))
    , m_target(core::Vector3D(0.0f))
    , m_matricesFlag(eCameraStateProjection)
{
    m_nodeType = ENodeType::eCamera;
    LOG_INFO("Create node type: %s", getNodeNameByType(m_nodeType).c_str());

    const core::Rect32& size = RENDERER->getViewportSize();
    m_aspect = (f32)size.getWidth() / (f32)size.getHeight();

    m_matricesFlag |= eCameraStateProjection;
}

CCamera::~CCamera()
{
    LOG_INFO("Delete node type: %s", getNodeNameByType(m_nodeType).c_str());
}

void CCamera::setTarget(const core::Vector3D& target)
{
    m_target = target;
    m_matricesFlag |= eCameraStateView;
}

void CCamera::setUpVector(const core::Vector3D& up)
{
    m_up = up;
    m_matricesFlag |= eCameraStateView;
}

void CCamera::setProjectionMatrix(const core::Matrix4D& matrix, bool ortho)
{
    m_orthogonal = ortho;
    m_matricesFlag &= ~eCameraStateProjection;
    m_transform[eTransformProjectionMatrix] = matrix;
}

const core::Vector3D& CCamera::getTarget() const
{
    return m_target;
}

const core::Vector3D& CCamera::getUpVector() const
{
    return m_up;
}

const core::Matrix4D& CCamera::getViewMatrix() const
{
    if (m_matricesFlag & eCameraStateView)
    {
        CCamera::recalculateViewMatrix();
    }

    return m_transform[eTrnsformViewMatrix];
}

const core::Matrix4D& CCamera::getProjectionMatrix() const
{
    if (m_matricesFlag & eCameraStateProjection)
    {
        CCamera::recalculateProjectionMatrix();
    }

    return m_transform[eTransformProjectionMatrix];
}

bool CCamera::isOrthogonal() const
{
    return m_orthogonal;
}

f32 CCamera::getNearValue() const
{
    return m_zNear;
}

f32 CCamera::getFarValue() const
{
    return m_zFar;
}

f32 CCamera::getAspectRatio() const
{
    return m_active;
}

bool CCamera::isActive() const
{
    return m_active;
}

void CCamera::setNearValue(f32 value)
{
    m_zNear = value;
    m_matricesFlag |= eCameraStateProjection;
}

void CCamera::setFarValue(f32 value)
{
    m_zNear = value;
    m_matricesFlag |= eCameraStateProjection;
}

void CCamera::setAspectRatio(f32 value)
{
    m_aspect = value;
    m_matricesFlag |= eCameraStateProjection;
}

void CCamera::recalculateProjectionMatrix() const
{
    if (CCamera::isOrthogonal())
    {
        const core::Rect32& size = RENDERER->getViewportSize();
        m_transform[eTransformProjectionMatrix] = core::buildProjectionMatrixOrtho(0.0f, (f32)size.getWidth(), 0.0f, (f32)size.getHeight(), m_zNear, m_zFar);
        m_transform[eTransformProjectionMatrix].makeTransposed();
    }
    else
    {
        m_transform[eTransformProjectionMatrix] = core::buildProjectionMatrixPerspective(45.0f, m_aspect, m_zNear, m_zFar);
    }
    m_matricesFlag &= ~eCameraStateProjection;
}

void CCamera::recalculateViewMatrix() const
{
    const core::Vector3D& position = CNode::getAbsTransform().getTranslation();
    
    /*core::Vector3D tgtv = m_target - position;
    core::Vector3D up = m_up;
    f32 dp = core::dotProduct(tgtv.normalize(), up.normalize());
    if (core::isEquals(fabs(dp), 1.f))
    {
        up.x = (up.x + 0.5f);
    }*/

    m_transform[eTrnsformViewMatrix] = core::buildLookAtMatrix(position, m_target, m_up);
    m_transform[eTrnsformViewMatrix].makeTransposed();
    m_matricesFlag &= ~eCameraStateView;
}

void CCamera::setActive(bool active)
{
    m_active = active;
}

void CCamera::init()
{
    m_initialiazed = true;
}

void CCamera::update(s32 dt)
{
    if (!CNode::isVisible() || !m_initialiazed)
    {
        return;
    }

    if (m_active)
    {
        CNode::update(dt);

        if (m_matricesFlag & eCameraStateProjection)
        {
            CCamera::recalculateProjectionMatrix();
        }

        if (m_matricesFlag & eCameraStateView)
        {
            CCamera::recalculateViewMatrix();
        }

        RENDERER->updateCamera(this);
    }
}

void CCamera::init(const stream::IStreamPtr& stream)
{
    IResource::setStream(stream);
}

bool CCamera::load()
{
    const IStreamPtr& stream = IResource::getStream();
    if (!stream)
    {
        LOG_ERROR("CCamera: Empty Stream with name [%s]", IResource::getResourseName().c_str());
        return false;
    }

    //TODO:

    return true;
}
