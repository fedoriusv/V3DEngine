#include "RenderStateGL.h"
#include "Engine.h"

#ifdef _OPENGL_DRIVER_
#include "GL/glew.h"

namespace v3d
{
namespace renderer
{
namespace gl
{

GLenum EWindingGL[] =
{
    GL_CW,
    GL_CCW
};

GLenum ECompareFuncGL[ECompareFunc::eCompareCount] =
{
    GL_LESS,
    GL_LEQUAL,
    GL_EQUAL,
    GL_GEQUAL,
    GL_GREATER,
    GL_NOTEQUAL,
    GL_ALWAYS,
    GL_NEVER
};

GLenum EPolygonModeGL[EPolygonMode::eModeCount] =
{
    GL_FILL,
    GL_LINE,
    GL_POINT
};

GLenum ECullfaceGL[ECullFace::eCullfaceCount] =
{
    GL_FRONT,
    GL_BACK,
    GL_FRONT_AND_BACK
};

GLenum EBlendFactorGL[EBlendFactor::eBlendCount] =
{
    GL_ZERO,
    GL_ONE,
    GL_SRC_COLOR,
    GL_ONE_MINUS_SRC_COLOR,
    GL_DST_COLOR,
    GL_ONE_MINUS_DST_COLOR,
    GL_SRC_ALPHA,
    GL_ONE_MINUS_SRC_ALPHA,
    GL_DST_ALPHA,
    GL_ONE_MINUS_DST_ALPHA,
    GL_SRC_ALPHA_SATURATE
};

u32 CRenderStateGL::s_currentCullface = -1;
bool CRenderStateGL::s_currentCulling = false;

u32 CRenderStateGL::s_currentPolygonMode = -1;
u32 CRenderStateGL::s_currentWinding = -1;

bool CRenderStateGL::s_currentDepthTest = false;
u32 CRenderStateGL::s_currentDepthFunc = -1;
bool CRenderStateGL::s_currentDepthMask = false;
bool CRenderStateGL::s_currentStencilMask = false;

bool CRenderStateGL::s_currentBlend = false;
u32 CRenderStateGL::s_currentBlendDst = -1;
u32 CRenderStateGL::s_currentBlendSrc = -1;

f32  CRenderStateGL::s_pointSize = 1.0f;
bool CRenderStateGL::s_pointSizeProgram = false;
bool CRenderStateGL::s_rasterizerEnable = true;

u32 CRenderStateGL::s_patchesSize = 0;
f32 CRenderStateGL::s_patchLevel[] = { 0.f, 0.f };

CRenderStateGL::CRenderStateGL()
{
    CRenderStateGL::pointSize(1.0f);
    CRenderStateGL::pointSizeProgram(false);
    CRenderStateGL::rasterizerEnable(true);
}

CRenderStateGL::~CRenderStateGL()
{
}

void CRenderStateGL::bind()
{
    CRenderStateGL::polygonMode(m_polygonMode);
    CRenderStateGL::winding(m_winding);

    CRenderStateGL::culling(m_culling);
    CRenderStateGL::cullface(m_cullface);

    CRenderStateGL::depthWrite(m_depthWrite);
    CRenderStateGL::depthTest(m_depthTest);
    CRenderStateGL::depthFunc(m_depthFunc);

    CRenderStateGL::blend(m_blend);
    CRenderStateGL::blendFunc(m_blendSrc, m_blendDst);

    RENDERER->checkForErrors("CRenderStateGL: Bind Error");
}

bool CRenderStateGL::blend(bool enable)
{
    if (enable != s_currentBlend)
    {
        enable ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
        s_currentBlend = enable;

        return true;
    }

    return false;
}

bool CRenderStateGL::cullface(ECullFace mode)
{
    if (mode != s_currentCullface)
    {
        glCullFace(ECullfaceGL[mode]);
        s_currentCullface = mode;

        return true;
    }

    return false;
}

bool CRenderStateGL::culling(bool enable)
{
    if (enable != s_currentCulling)
    {
        enable ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
        s_currentCulling = enable;

        return true;
    }

    return false;
}

bool CRenderStateGL::polygonMode(EPolygonMode mode)
{
    if (mode != s_currentPolygonMode)
    {
        glPolygonMode(GL_FRONT_AND_BACK, EPolygonModeGL[mode]);
        s_currentPolygonMode = mode;

        return true;
    }

    return false;
}

bool CRenderStateGL::winding(EWinding winding)
{
    if (winding != s_currentWinding)
    {
        glFrontFace(EWindingGL[winding]);
        s_currentWinding = winding;

        return true;
    }

    return false;
}

bool CRenderStateGL::depthFunc(ECompareFunc mode)
{
    if (mode != s_currentDepthFunc)
    {
        glDepthFunc(ECompareFuncGL[mode]);
        s_currentDepthFunc = mode;

        return true;
    }

    return false;
}

bool CRenderStateGL::depthWrite(bool enable)
{
    if (enable != s_currentDepthMask)
    {
        glDepthMask(enable ? GL_TRUE : GL_FALSE);
        s_currentDepthMask = enable;

        return true;
    }

    return false;
}

bool CRenderStateGL::stencilWrite(bool enable)
{
    if (enable != s_currentStencilMask)
    {
        glStencilMask(enable ? GL_TRUE : GL_FALSE);
        s_currentStencilMask = enable;

        return true;
    }

    return false;
}

bool CRenderStateGL::pointSize(f32 value)
{
    if (value != s_pointSize)
    {
        glPointSize(value); //less priority than pointSizeProgram
        s_pointSize = value;

#ifdef _DEBUG_GL
        RENDERER->checkForErrors(" CRenderStateGL::pointSize error");
#endif //_DEBUG_GL

        return true;
    }

    return false;
}

bool CRenderStateGL::pointSizeProgram(bool enable)
{
    if (enable != s_pointSizeProgram)
    {
        CRenderStateGL::pointSize(1.0f);
        enable ? glEnable(GL_PROGRAM_POINT_SIZE) : glDisable(GL_PROGRAM_POINT_SIZE); // use gl_PointSize in veratex shader to change size of pixel
        s_pointSizeProgram = enable;

#ifdef _DEBUG_GL
        RENDERER->checkForErrors("CRenderStateGL::pointSizeProgram error");
#endif //_DEBUG_GL

        return true;
    }

    return false;
}

bool CRenderStateGL::blendFunc(EBlendFactor src, EBlendFactor dst)
{
    if (s_currentBlendSrc != src || s_currentBlendDst != dst)
    {
        glBlendFunc(EBlendFactorGL[src], EBlendFactorGL[dst]);
        s_currentBlendDst = dst;
        s_currentBlendSrc = src;

        return true;
    }

    return false;
}

bool CRenderStateGL::depthTest(bool enable)
{
    if (enable != s_currentDepthTest)
    {
        enable ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
        s_currentDepthTest = enable;

        return true;
    }

    return false;
}

bool CRenderStateGL::rasterizerEnable(bool enable)
{
    if (enable != s_rasterizerEnable)
    {
        enable ? glDisable(GL_RASTERIZER_DISCARD) : glEnable(GL_RASTERIZER_DISCARD);
        s_rasterizerEnable = enable;

        return true;
    }

    return false;
}

bool CRenderStateGL::patchLevel(u32 size, f32 inner, f32 outer)
{
    ASSERT(size <= 4U, "Invalid size");

    bool changed = false;
    if (size != s_patchesSize)
    {
        glPatchParameteri(GL_PATCH_VERTICES, size);
        s_patchesSize = size;

#ifdef _DEBUG_GL
        RENDERER->checkForErrors("CRenderStateGL::patchLevel GL_PATCH_VERTICES error");
#endif //_DEBUG_GL
        changed =  true;
    }

    if (inner != s_patchLevel[0] || outer != s_patchLevel[1])
    {
        std::array<f32, 2> innerArray;
        innerArray.fill(inner);
        glPatchParameterfv(GL_PATCH_DEFAULT_INNER_LEVEL, innerArray.data());
        s_patchLevel[0] = inner;

#ifdef _DEBUG_GL
        RENDERER->checkForErrors("CRenderStateGL::patchLevel GL_PATCH_DEFAULT_INNER_LEVEL error");
#endif //_DEBUG_GL

        std::array<f32, 4> outerArray;
        outerArray.fill(outer);
        glPatchParameterfv(GL_PATCH_DEFAULT_OUTER_LEVEL, outerArray.data());
        s_patchLevel[1] = outer;

#ifdef _DEBUG_GL
        RENDERER->checkForErrors("CRenderStateGL::patchLevel GL_PATCH_DEFAULT_OUTER_LEVEL error");
#endif //_DEBUG_GL

        changed = true;
    }

    return changed;
}

} //namespace gl
} //namespace renderer
} //namespace v3d

#endif //_OPENGL_DRIVER_
