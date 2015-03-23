#include "RenderStateGL.h"
#include "Engine.h"
#include "GL/glew.h"

using namespace v3d;
using namespace renderer;

GLenum EPolygonModeGL[] =
{
    GL_FILL,
    GL_LINE,
    GL_POINT
};

GLenum EWindingGL[] =
{
    GL_CW,
    GL_CCW
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
bool CRenderStateGL::s_currentCulling = true;
u32 CRenderStateGL::s_currentPolygonMode = ePolyModeFill;
u32 CRenderStateGL::s_currentWinding = eWindingCW;

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
    CRenderStateGL::culling(m_cullFace);

    CRenderStateGL::blend(m_blend);
    glBlendFunc(EBlendFactorGL[m_blendDst], EBlendFactorGL[m_blendSrc]);

    RENDERER->checkForErrors("CRenderStateGL Bind Error");
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
        glPolygonMode(GL_FRONT_AND_BACK, EWindingGL[winding]);
        s_currentWinding = winding;

        return true;
    }

    return false;
}