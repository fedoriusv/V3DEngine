#ifndef _V3D_RENDER_STATE_GL_H_
#define _V3D_RENDER_STATE_GL_H_

#include "renderer/RenderState.h"

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CRenderStateGL : public CRenderState
    {
    public:

        CRenderStateGL();
        ~CRenderStateGL();

        void        bind() override;

    private:

        bool        blend(bool enable);
        bool        culling(bool enable);
        bool        polygonMode(EPolygonMode mode);
        bool        winding(EWinding winding);

        static bool s_currentBlend;
        static bool s_currentCulling;
        static u32  s_currentPolygonMode;
        static u32  s_currentWinding;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_RENDER_STATE_GL_H_