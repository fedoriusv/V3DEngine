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
        ~CCubeShape();

        void        render()         override;
        void        update(s32 time) override;

        void        init()           override;

    private:

        void        refresh()        override;
        void        build();

    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_CUBE_SHAPE_H_
