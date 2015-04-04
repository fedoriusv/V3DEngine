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

#endif //_V3D_SAMPLE_SHAPE_H_
