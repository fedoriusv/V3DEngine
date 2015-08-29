#ifndef _V3D_DISK_SHAPE_H_
#define _V3D_DISK_SHAPE_H_

#include "scene/Shape.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CDiskShape : public CShape
    {
    public:

        CDiskShape(f32 minorRadius = 1.f, f32 majorRadius = 2.f);
        ~CDiskShape();

        void        init()          override;
        void        render()        override;
        void        update(s32 dt)  override;

        void        setMinorRadius(f32 radius);
        void        setMajorRadius(f32 radius);
        f32         getMinorRadius() const;
        f32         getMajorRadius() const;

    private:

        void        refresh();
        void        build();

        f32         m_majorRadius;
        f32         m_minorRadius;

    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_DISK_SHAPE_H_
