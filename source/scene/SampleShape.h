#ifndef _V3D_SAMPLE_SHAPE_H_
#define _V3D_SAMPLE_SHAPE_H_

#include "scene/Shape.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CSampleShape : public CShape
    {
    public:

        CSampleShape();
        ~CSampleShape();


        void        init()          override;
        void        render()        override;
        void        update(s32 dt)  override;

    private:

        void        refresh();
        void        build();
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_SAMPLE_SHAPE_H_
