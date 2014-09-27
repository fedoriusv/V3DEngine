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

        CCylinderShape(const float radius = 0.5f, const float height = 1.0f);
        virtual     ~CCylinderShape();

        void        render()         override;
        void        update(f64 time) override;

        void        init()           override;

        void        setRadius(const f32 radius);
        void        setHeight(const f32 height);

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
