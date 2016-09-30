#include "TransformFeedbackGL.h"
#include "utils/Logger.h"
#include "Engine.h"

#ifdef _OPENGL_RENDER_
#include "GL/glew.h"

namespace v3d
{
namespace renderer
{
namespace gl
{

GLenum EBufferFeedbackModeGL[] =
{
    GL_INTERLEAVED_ATTRIBS,
    GL_SEPARATE_ATTRIBS
};

const GLenum EPrimitivesModeGL[EPrimitivesMode::ePrimitivesModeCount] =
{
    GL_TRIANGLES,
    GL_TRIANGLES,
    GL_LINES,
    GL_LINES,
    GL_POINTS
};

u32 TransformFeedbackGL::s_currentTransformFeedbackID = 0;

bool TransformFeedbackGL::s_started = false;
bool TransformFeedbackGL::s_paused = false;

void TransformFeedbackGL::transformFeedbackVaryings(u32 program, const std::vector<const c8*>& list)
{
    ASSERT(glIsProgram(program), "Invalid Index Shader program");
    glTransformFeedbackVaryings(program, (u32)list.size(), (const GLchar**)list.data(), EBufferFeedbackModeGL[0]);
}

TransformFeedbackGL::TransformFeedbackGL()
    : m_id(0)
{
    LOG_DEBUG("TransformFeedbackGL::TransformFeedbackGL constructor %x", this);

    glGenTransformFeedbacks(1, &m_id);
}

TransformFeedbackGL::~TransformFeedbackGL()
{
    ASSERT(!s_started, "TransformFeedback doesn't stoped");

    if (m_id > 0)
    {
        ASSERT(glIsTransformFeedback(m_id), "Invalid TransformFeedback index");
        glDeleteTransformFeedbacks(1, &m_id);
        m_id = 0;
    }

    ASSERT(m_id == 0, "TransformFeedback doesn't deleted");
    LOG_DEBUG("TransformFeedbackGL::TransformFeedbackGL destructor %x", this);
}

bool TransformFeedbackGL::bind()
{
    ASSERT(!s_started, "TransformFeedback doesn't stoped");

    if (s_currentTransformFeedbackID != m_id)
    {
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_id);
        ASSERT(glIsTransformFeedback(m_id), "Invalid TransformFeedback index");
        s_currentTransformFeedbackID = m_id;

        return true;
    }

    return false;
}

void TransformFeedbackGL::unbind()
{
    if (s_currentTransformFeedbackID != 0)
    {
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
        s_currentTransformFeedbackID = 0;
    }
}

void TransformFeedbackGL::begin(EPrimitivesMode mode)
{
    if (s_started)
    {
        return;
    }

    glBeginTransformFeedback(EPrimitivesModeGL[mode]);
    s_started = true;
    s_paused = false;

    ENGINE_RENDERER->checkForErrors("TransformFeedbackGL::begin Error");
}

void TransformFeedbackGL::end()
{
    if (!s_started)
    {
        return;
    }

    glEndTransformFeedback();
    s_started = false;
    s_paused = false;

    ENGINE_RENDERER->checkForErrors("TransformFeedbackGL::end Error");
}

void TransformFeedbackGL::pause()
{
    if (!s_started || s_paused)
    {
        return;
    }

    glPauseTransformFeedback();
    s_paused = true;

    ENGINE_RENDERER->checkForErrors("TransformFeedbackGL::pause Error");
}

void TransformFeedbackGL::resume()
{
    if (!s_started || !s_paused)
    {
        return;
    }

    glResumeTransformFeedback();
    s_paused = false;

    ENGINE_RENDERER->checkForErrors("TransformFeedbackGL::resume Error");
}


} //namespace gl
} //namespace renderer
} //namespace v3d

#endif //_OPENGL_RENDER_
