#ifndef _V3D_CUBE_SHAPE_H_
#define _V3D_CUBE_SHAPE_H_

#include "scene/Shape.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CCubeShape : public CShape
    {
    public:

        CCubeShape();
        virtual     ~CCubeShape();

        void        render()         override;
        void        update(f64 time) override;

        void        init()           override;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_CUBE_SHAPE_H_
