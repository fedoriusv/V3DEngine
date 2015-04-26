#ifndef _V3D_CAMERA_H_
#define _V3D_CAMERA_H_

#include "Node.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CScene;

    class CCamera : public CNode
    {
    public:

        enum ETransformMatrix
        {
            eTrnsformViewMatrix = 0,
            eTransformProjectionMatrix,

            eTransformCount,
        };

        enum ECameraState
        {
            eCameraStateView       = 1 << 0,
            eCameraStateProjection = 1 << 1,

            eCameraStateAll = eCameraStateView | eCameraStateProjection,
        };

        CCamera();
        virtual                 ~CCamera();

        void                    setTarget(const core::Vector3D& target);
        void                    setUpVector(const core::Vector3D& up);
        void                    setProjectionMatrix(const core::Matrix4D& matrix, bool ortho);
        void                    setNearValue(f32 value);
        void                    setFarValue(f32 value);
        void                    setAspectRatio(f32 value);

        const core::Vector3D&   getTarget()             const;
        const core::Vector3D&   getUpVector()           const;
        const core::Matrix4D&   getViewMatrix()         const;
        const core::Matrix4D&   getProjectionMatrix()   const;

        f32                     getNearValue()          const;
        f32                     getFarValue()           const;
        f32                     getAspectRatio()        const;

        bool                    isOrthogonal()          const;

        void                    init()                  override;
        void                    update(s32 time)        override;
        void                    render()                override;


        bool                    isActive()              const;

    protected:

        friend                  CScene;

        void                    setActive(bool active);
        void                    recalculateProjectionMatrix()   const;
        void                    recalculateViewMatrix()         const;

        mutable core::Matrix4D  m_transform[eTransformCount];
        bool                    m_orthogonal;

        f32                     m_aspect; 
        f32                     m_zNear; 
        f32                     m_zFar;

        bool                    m_active;

    private:

        core::Vector3D          m_up;
        core::Vector3D          m_target;

        mutable u16             m_matricesFlag;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CCamera>    CameraPtr;
    typedef std::weak_ptr<CCamera>      CameraWPtr;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_CAMERA_H_