#ifndef _V3D_TORUS_SHAPE_H_
#define _V3D_TORUS_SHAPE_H_

#include "scene/Shape.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CTorusShape : public CShape
    {
    public:

        CTorusShape(const f32 minorRadius = 0.5f, const f32 majorRadius = 1.0f);
        virtual     ~CTorusShape();

        void        render()         override;
        void        update(f64 time) override;

        void        init()           override;

        void        setMinorRadius(const f32 radius);
        void        setMajorRadius(const f32 radius);

        f32         getMinorRadius() const;
        f32         getMajorRadius() const;
    
    private:

        void        refresh()        override;
        void        build();

        f32         m_majorRadius;
        f32         m_minorRadius;
    };



    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_TORUS_SHAPE_H_
