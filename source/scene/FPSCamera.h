#ifndef F3D_FPSCAMERA_H_
#define F3D_FPSCAMERA_H_

#include "Camera.h"
#include "event/KeyCodes.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    struct SMoveKeys
    {
        v3d::EKeyCode _left;
        v3d::EKeyCode _right;
        v3d::EKeyCode _forward;
        v3d::EKeyCode _back;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CFPSCamera : public CCamera
    {
    public:

        CFPSCamera();
        virtual     ~CFPSCamera();

        void        update(f64 time) override;

        void        setSpeed(f32 speed);
        f32         getSpeed() const;
        void        setMoveKeys(const SMoveKeys& keys);

    private:

        void        move(const Vector3D& direction);
        bool        isPointOut(const Vector3D& point);

        void        rotateByMouse();
        void        rotate(f32 angle, const Vector3D& point);

        void        setCursorPosition(const Vector3D& position);
        void        getCursorPosition(Vector3D& position);

        SMoveKeys   m_keys;
        f32         m_speed;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //F3D_FPSCAMERA_H_