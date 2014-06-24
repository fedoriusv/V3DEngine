#include "RenderStateGL.h"
#include "Engine.h"
#include "GL/glew.h"

using namespace v3d;
using namespace v3d::renderer;

GLenum ERenderPolygonModeGL[] =
{
    GL_FILL,
    GL_LINE,
    GL_POINT
};

GLenum ERenderWindingGL[] =
{
    GL_CW,
    GL_CCW
};

CRenderStateGL::CRenderStateGL()
{
}

CRenderStateGL::~CRenderStateGL()
{
}

void CRenderStateGL::bind()
{
    glPolygonMode(GL_FRONT_AND_BACK, ERenderPolygonModeGL[m_polygonMode]);

    glFrontFace(ERenderWindingGL[m_winding]);
    m_cullFace ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);


    //TODO:

    RENDERER->checkForErrors("CRenderStateGL Bind Error");
}