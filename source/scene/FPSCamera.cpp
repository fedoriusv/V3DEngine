#include "FPSCamera.h"
#ifdef _WIN32
#	include <windows.h>
#endif // _WIN32
#include "Engine.h"


using namespace v3d;
using namespace v3d::scene;

CFPSCamera::CFPSCamera()
{
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
    s32 middleX = RENDERER->getViewportSize().width/2;
    s32 middleY = RENDERER->getViewportSize().height/2;
    f32 angleY = 0.0f;
    f32 angleZ = 0.0f;

    static f32 currentRotX = 0.0f;
    
    ShowCursor(0);

    Vector3D position(0, 0);
    CFPSCamera::getCursorPosition(position);

    if ((position.x == middleX) && (position.y == middleY))
    {
        return;
    }

    CFPSCamera::setCursorPosition(Vector3D(position.x, position.y));
    
    angleY = static_cast<f32>(middleX - position.x)/ 1000.0f;
    angleZ = static_cast<f32>(middleY - position.y)/1000.0f;

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
        rotate(angleZ, vAxis);
    }

    rotate(angleY, Vector3D(0, 1, 0));
}

void CFPSCamera::rotate(f32 angle, Vector3D& point)
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
#ifdef _WIN32
    SetCursorPos(static_cast<u32>(position.x), static_cast<u32>(position.y));
#endif //_WIN32
}

void CFPSCamera::getCursorPosition(Vector3D& position)
{
#ifdef _WIN32
    POINT mouse;
    GetCursorPos(&mouse);

    position.x = static_cast<f32>(mouse.x);
    position.y = static_cast<f32>(mouse.y);
#endif //_WIN32

}

void CFPSCamera::update(f64 time)
{
    CFPSCamera::rotateByMouse();
    CCamera::update(time);
}