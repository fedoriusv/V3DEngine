#ifndef _V3D_PLANE_SHAPE_H_
#define _V3D_PLANE_SHAPE_H_

#include "scene/Shape.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CPlaneShape : public CShape
    {
    public:

        CPlaneShape(f32 extent);
        ~CPlaneShape();

        void        render()         override;
        void        update(s32 time) override;

        void        init()           override;

        void        setExtent(const f32 extent);
        f32         getExtent() const;

    private:

        void        refresh()        override;
        void        build();

        f32         m_extent;

    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_PLANE_SHAPE_H_
