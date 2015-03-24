#include "RenderStateGL.h"
#include "Engine.h"
#include "GL/glew.h"

using namespace v3d;
using namespace renderer;

GLenum EWindingGL[] =
{
    GL_CW,
    GL_CCW
};

GLenum ECompareFuncGL[] =
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

bool CRenderStateGL::s_currentBlend = false;
u32 CRenderStateGL::s_currentCullface = -1;
bool CRenderStateGL::s_currentCulling = false;
u32 CRenderStateGL::s_currentPolygonMode = -1;
u32 CRenderStateGL::s_currentWinding = -1;
u32 CRenderStateGL::s_currentDepthFunc = -1;
bool CRenderStateGL::s_currentDepthMask = false;

CRenderStateGL::CRenderStateGL()
{
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
 
    CRenderStateGL::blend(m_blend);
    glBlendFunc(EBlendFactorGL[m_blendDst], EBlendFactorGL[m_blendSrc]);


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
        enable ? glDepthMask(GL_TRUE) : glDepthMask(GL_FALSE);
        s_currentDepthMask = enable;

        return true;
    }

    return false;
}