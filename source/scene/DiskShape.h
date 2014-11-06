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

        CDiskShape(const f32 minorRadius, const f32 majorRadius);
        ~CDiskShape();

        void        render()         override;
        void        update(f64 time) override;

        void        init()           override;

        void        setMinorRadius(const f32 radius);
        void        setMajorRadius(const f32 radius);
        f32         getMinorRadius() const;
        f32         getMajorRadius() const;

    private:

        void        refresh()       override;
        void        build();

        f32         m_majorRadius;
        f32         m_minorRadius;

    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_DISK_SHAPE_H_
