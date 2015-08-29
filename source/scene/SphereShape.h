#ifndef _V3D_SPHERE_SHAPE_H_
#define _V3D_SPHERE_SHAPE_H_

#include "scene/Shape.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CSphereShape : public CShape
    {
    public:

        explicit CSphereShape(f32 radius = 1.f);
        ~CSphereShape();


        void        init()         override;
        void        render()       override;
        void        update(s32 dt) override;

        void        setRadius(f32 radius);
        f32         getRadius() const;

    private:

        void        refresh();
        void        build();

        f32         m_radius;

    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_SPHERE_SHAPE_H_
