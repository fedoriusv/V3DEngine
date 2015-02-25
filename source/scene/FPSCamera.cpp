#include "FPSCamera.h"
#ifdef _PLATFORM_WIN_
#	include <windows.h>
#endif //_PLATFORM_WIN_
#include "Engine.h"


using namespace v3d;
using namespace v3d::scene;

CFPSCamera::CFPSCamera()
    : m_speed(0.001f)
{
    m_keys._forward = EKeyCode::eKeyKey_W;
    m_keys._back    = EKeyCode::eKeyKey_S;
    m_keys._left    = EKeyCode::eKeyKey_A;
    m_keys._right   = EKeyCode::eKeyKey_D;
}

CFPSCamera::~CFPSCamera()
{
}

void CFPSCamera::move(const Vector3D& direction)
{
    Vector3D pos = m_position;
    Vector3D view = m_target;
    Vector3D move = m_target - m_position;
    Vector3D strafe = core::crossProduct(move, m_up);

    move.normalize();
    strafe.normalize();

    m_position += (move * direction.z);
    m_target += (move * direction.z);

    m_position += (strafe * direction.x);
    m_target += (strafe * direction.x);

    m_position.y += direction.y;
    m_target.y += direction.y;

    if (CFPSCamera::isPointOut(m_position))
    {
        m_position = pos;
        m_target = view;
    }

    CNode::updateTransform(ENodeTransform::eTranslation);
}

bool CFPSCamera::isPointOut(const Vector3D& point)
{
    if (point.x <= m_min.x || point.y <= m_min.y || point.z <= m_min.z ||
        point.x >= m_max.x || point.y >= m_max.y || point.z >= m_max.z)
    {
        return true;
    }

    return false;
}

void CFPSCamera::rotateByMouse()
{
    Vector3D middle(0, 0);
    middle.x = static_cast<f32>(WINDOW->getSize().width / 2);
    middle.y = static_cast<f32>(WINDOW->getSize().height / 2);

    static f32 currentRotX = 0.0f;
    
    //ShowCursor(0);

    Vector3D position(0, 0);
    CFPSCamera::getCursorPosition(position);

    if (position == middle)
    {
        return;
    }
    m_needUpdate = true;

    CFPSCamera::setCursorPosition(middle);
    
    Vector3D angle = (middle - position) / 1000.0f;

    static f32 lastRotX = 0.0f;
    lastRotX = -currentRotX;

    Vector3D vAxis = crossProduct(Vector3D(m_target - m_position), m_up);
    vAxis.normalize();

    if (currentRotX > 1.0f)
    {
        currentRotX = 1.0f;
        if (lastRotX != 1.0f)
        {
            rotate(1.0f - lastRotX, vAxis);
        }
    }
    else if (currentRotX < -1.0f)
    {
        currentRotX = -1.0f;
        if (lastRotX != -1.0f)
        {
            rotate(-1.0f - lastRotX, vAxis);
        }
    }
    else
    {
        rotate(angle.y, vAxis);
    }

    CFPSCamera::rotate(angle.x, Vector3D(0.0f, 1.0f, 0.0f));
}

void CFPSCamera::rotate(f32 angle, const Vector3D& point)
{
    Vector3D newView;
    Vector3D oldView;

    oldView = m_target - m_position;

    float cosTheta = static_cast<f32>(cos(angle));
    float sinTheta = static_cast<f32>(sin(angle));

    newView.x = (cosTheta + (1 - cosTheta) * point.x * point.x) * oldView.x;
    newView.x = newView.x + ((1 - cosTheta) * point.x * point.y - point.z * sinTheta)	* oldView.y;
    newView.x = newView.x + ((1 - cosTheta) * point.x * point.z + point.y * sinTheta)	* oldView.z;

    newView.y = ((1 - cosTheta) * point.x * point.y + point.z * sinTheta) * oldView.x;
    newView.y = newView.y + (cosTheta + (1 - cosTheta) * point.y * point.y) * oldView.y;
    newView.y = newView.y + ((1 - cosTheta) * point.y * point.z - point.x * sinTheta) * oldView.z;

    newView.z = ((1 - cosTheta) * point.x * point.z - point.y * sinTheta) * oldView.x;
    newView.z = newView.z + ((1 - cosTheta) * point.y * point.z + point.x * sinTheta)	* oldView.y;
    newView.z = newView.z + (cosTheta + (1 - cosTheta) * point.z * point.z) * oldView.z;

    m_target = m_position + newView;
}

void CFPSCamera::setCursorPosition(const Vector3D& position)
{
#ifdef _PLATFORM_WIN_
    SetCursorPos(static_cast<s32>(position.x), static_cast<s32>(position.y));
#endif //_PLATFORM_WIN_
}

void CFPSCamera::getCursorPosition(Vector3D& position)
{
#ifdef _PLATFORM_WIN_
    POINT mouse;
    GetCursorPos(&mouse);

    position.x = static_cast<f32>(mouse.x);
    position.y = static_cast<f32>(mouse.y);
#endif //_PLATFORM_WIN_

}

void CFPSCamera::update(f64 time)
{
    if (!m_visible)
    {
        return;
    }

    if (m_active)
    {
        f32 s = m_speed * static_cast<f32>(time);
        if (INPUT_EVENTS->isKeyPressed(m_keys._forward))
        {
            CFPSCamera::move(Vector3D(0.0f, 0.0f, s));
            m_needUpdate = true;
        }
        if (INPUT_EVENTS->isKeyPressed(m_keys._back))
        {
            CFPSCamera::move(Vector3D(0.0f, 0.0f, -s));
            m_needUpdate = true;
        }
        if (INPUT_EVENTS->isKeyPressed(m_keys._left))
        {
            CFPSCamera::move(Vector3D(-s, 0.0f, 0.0f));
            m_needUpdate = true;
        }
        if (INPUT_EVENTS->isKeyPressed(m_keys._right))
        {
            CFPSCamera::move(Vector3D(s, 0.0f, 0.0f));
            m_needUpdate = true;
        }

        CFPSCamera::rotateByMouse();
        CCamera::update(time);
    }
}

void CFPSCamera::setSpeed(f32 speed)
{
    m_speed = speed;
}

f32 CFPSCamera::getSpeed() const
{
    return m_speed;
}

void CFPSCamera::setMoveKeys(const SMoveKeys& keys)
{
    m_keys._forward = keys._forward;
    m_keys._back = keys._back;
    m_keys._left = keys._left;
    m_keys._right = keys._right;
}
