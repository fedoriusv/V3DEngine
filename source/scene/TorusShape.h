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

        CTorusShape(f32 minorRadius = 0.5f, f32 majorRadius = 1.0f);
        ~CTorusShape();

        void        init()         override;
        void        render()       override;
        void        update(s32 dt) override;

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

#endif //_V3D_TORUS_SHAPE_H_
