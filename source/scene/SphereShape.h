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

        CSphereShape(f32 radius = 1.0f);
        ~CSphereShape();

        void        render()         override;
        void        update(s32 time) override;

        void        init()           override;

        void        setRadius(f32 radius);
        f32         getRadius() const;

    private:

        void        refresh()        override;
        void        build();

        f32         m_radius;

    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_SPHERE_SHAPE_H_
