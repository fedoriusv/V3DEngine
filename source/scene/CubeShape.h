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

        void        init()          override;
        void        render()        override;
        void        update(s32 dt)  override;

    private:

        void        build();

        const f32   k_extend = 0.5f;

    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_CUBE_SHAPE_H_
