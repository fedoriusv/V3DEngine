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

        explicit CRectangleShape(const core::Rect32& rect);
        ~CRectangleShape();

        void            init()          override;
        void            render()        override;
        void            update(s32 dt)  override;

    private:

        void            refresh();
        void            build();

        core::Rect32    m_rect;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_RECTANGLE_SHAPE_H_