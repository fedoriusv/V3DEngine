#ifndef _V3D_DEBUG_DRAW_H_
#define _V3D_DEBUG_DRAW_H_

#include "Object.h"
#include "RenderTechnique.h"

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CDebugDraw : public CObject
    {
    public:

        CDebugDraw();
        virtual                     ~CDebugDraw();
                                   
        virtual void                init() = 0;
        virtual void                draw() = 0;

    private:

        static RenderTechniquePtr   m_tehnique;

        u32                         m_arrayId;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CDebugDraw> DebugDrawPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_DEBUG_DRAW_H_