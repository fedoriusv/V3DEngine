#ifndef _F3D_SAMPLE_SHAPE_H_
#define _F3D_SAMPLE_SHAPE_H_

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
            virtual     ~CSampleShape();

            void        render()         override;
            void        update(f64 time) override;

            void        init()           override;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
}

#endif //_F3D_SAMPLE_SHAPE_H_