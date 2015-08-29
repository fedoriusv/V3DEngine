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

        explicit CPlaneShape(f32 extent = 2.f);
        ~CPlaneShape();

        void        init()          override;
        void        render()        override;
        void        update(s32 dt)  override;

        void        setExtent(f32 extent);
        f32         getExtent() const;

    private:

        void        refresh();
        void        build();

        f32         m_extent;

    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_PLANE_SHAPE_H_
