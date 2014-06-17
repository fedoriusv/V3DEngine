#ifndef F3D_FPSCAMERA_H_
#define F3D_FPSCAMERA_H_

#include "Camera.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CFPSCamera : public CCamera
    {
    public:

        CFPSCamera();
        virtual ~CFPSCamera();

        void    update(f64 time) override;

    private:

        void    move(const Vector3D& direction);
        bool    isPointOut(const Vector3D& point);

        void    rotateByMouse();
        void    rotate(f32 angle, Vector3D& point);

        void    setCursorPosition(const Vector3D& position);
        void    getCursorPosition(Vector3D& position);
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //F3D_FPSCAMERA_H_