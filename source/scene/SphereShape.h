#ifndef _V3D_SPHERE_SHAPE_H_
#define _V3D_SPHERE_SHAPE_H_

#include "scene/Shape.h"

namespace v3d
{
    namespace scene
    {
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////

        class CSphereShape : public CShape
        {
        public:

            CSphereShape();
            virtual     ~CSphereShape();

            void        render()         override;
            void        update(f64 time) override;

            void        init()           override;

            void        setSlices(const u32 value);
            void        setStacks(const u32 value);
            void        setRadius(const f32 radius);

            u32         getSlices() const;
            u32         getStacks() const;
            f32         getRadius() const;

        private:

            u32         m_slices;
            u32         m_stacks;
            f32         m_radius;

        };

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
}

#endif //_V3D_SPHERE_SHAPE_H_