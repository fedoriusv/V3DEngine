#pragma once

#include "renderer/RenderState.h"

#ifdef _OPENGL_RENDER_

namespace v3d
{
namespace renderer
{
namespace gl
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * Inherited class for render states management. GL render only.
    */
    class CRenderStateGL final : public CRenderState
    {
    public:

        CRenderStateGL();
        CRenderStateGL(const CRenderStateGL& state)            = delete;
        CRenderStateGL& operator=(const CRenderStateGL& state) = delete;

        ~CRenderStateGL();

        void         bind() override;

    public:

        static bool  blend(bool enable);
        static bool  blendFunc(EBlendFactor src, EBlendFactor dst);
        static bool  cullface(ECullFace mode);
        static bool  culling(bool enable);
        static bool  polygonMode(EPolygonMode mode);
        static bool  winding(EWinding winding);
        static bool  depthTest(bool enable);
        static bool  depthFunc(ECompareFunc mode);
        static bool  depthWrite(bool enable);
        static bool  stencilWrite(bool enable);
        static bool  pointSize(f32 value);
        static bool  pointSizeProgram(bool enable);
        static bool  rasterizerEnable(bool enable);
        static bool  patchLevel(u32 size, f32 inner, f32 outer);

    private:

        static u32  s_currentCullface;
        static bool s_currentCulling;

        static u32  s_currentPolygonMode;
        static u32  s_currentWinding;

        static bool s_currentDepthTest;
        static u32  s_currentDepthFunc;
        static bool s_currentDepthMask;
        static bool s_currentStencilMask;

        static bool s_currentBlend;
        static u32  s_currentBlendDst;
        static u32  s_currentBlendSrc;

        static f32  s_pointSize;
        static bool s_pointSizeProgram;
        static bool s_rasterizerEnable;

        static u32  s_patchesSize;
        static f32  s_patchLevel[2];
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace gl
} //namespace renderer
} //namespace v3d

#endif //_OPENGL_RENDER_
