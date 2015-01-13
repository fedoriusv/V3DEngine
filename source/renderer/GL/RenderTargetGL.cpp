#include "RenderTargetGL.h"
#include "GL/glew.h"

using namespace v3d;
using namespace renderer;

CRenderTargetGL::CRenderTargetGL()
: m_fboID(0)
{
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint*)&m_fboID);
}

CRenderTargetGL::~CRenderTargetGL()
{
}

void CRenderTargetGL::bind()
{
}

void CRenderTargetGL::create()
{
}

void CRenderTargetGL::destroy()
{

}