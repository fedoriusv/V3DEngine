#include "RenderStateGL.h"
#include "Engine.h"
#include "GL/glew.h"

using namespace v3d;
using namespace v3d::renderer;

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

GLenum EBlendFactorGL[] =
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

CRenderStateGL::CRenderStateGL()
{
}

CRenderStateGL::~CRenderStateGL()
{
}

void CRenderStateGL::bind()
{
    glPolygonMode(GL_FRONT_AND_BACK, EPolygonModeGL[m_polygonMode]);

    glFrontFace(EWindingGL[m_winding]);
    m_cullFace ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);


    m_blend ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
    glBlendFunc(EBlendFactorGL[m_blendDst], EBlendFactorGL[m_blendSrc]);

    RENDERER->checkForErrors("CRenderStateGL Bind Error");
}