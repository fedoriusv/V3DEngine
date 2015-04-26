#ifndef _V3D_CYLINDER_SHAPE_H_
#define _V3D_CYLINDER_SHAPE_H_

#include "scene/Shape.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CCylinderShape : public CShape
    {
    public:

        CCylinderShape(f32 radius = 0.5f, f32 height = 1.0f);
        ~CCylinderShape();

        void        render()         override;
        void        update(s32 time) override;

        void        init()           override;

        void        setRadius(f32 radius);
        void        setHeight(f32 height);

        f32         getRadius() const;
        f32         getHeight() const;

    private:

        void        refresh()        override;
        void        build();

        f32         m_radius;
        f32         m_height;

    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_CYLINDER_SHAPE_H_
