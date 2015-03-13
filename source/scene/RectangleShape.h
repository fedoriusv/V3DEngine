#ifndef _V3D_RECTANGLE_SHAPE_H_
#define _V3D_RECTANGLE_SHAPE_H_

#include "scene/Shape.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CRectangleShape : public CShape
    {
    public:

        explicit CRectangleShape(const core::Rect& rect);
        ~CRectangleShape();

        void        render()         override;
        void        update(f64 time) override;

        void        init()           override;

    private:

        void        refresh()        override;
        void        build();

        core::Rect  m_rect;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_RECTANGLE_SHAPE_H_