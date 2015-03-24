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

    public:

        static bool  blend(bool enable);
        static bool  cullface(ECullFace mode);
        static bool  culling(bool enable);
        static bool  polygonMode(EPolygonMode mode);
        static bool  winding(EWinding winding);
        static bool  depthFunc(ECompareFunc mode);
        static bool  depthWrite(bool enable);

    private:

        static bool s_currentBlend;
        static u32  s_currentCullface;
        static bool s_currentCulling;
        static u32  s_currentPolygonMode;
        static u32  s_currentWinding;
        static u32  s_currentDepthFunc;
        static bool s_currentDepthMask;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_RENDER_STATE_GL_H_