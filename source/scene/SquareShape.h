#ifndef _V3D_SQUARE_SHAPE_H_
#define _V3D_SQUARE_SHAPE_H_

#include "scene/Shape.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CSquareShape : public CShape
    {
    public:

        explicit CSquareShape();
        ~CSquareShape();

        void        render()         override;
        void        update(f64 time) override;

        void        init()           override;

    private:

        void        refresh()        override;
        void        build();
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_SQUARE_SHAPE_H_