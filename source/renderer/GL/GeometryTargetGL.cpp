#include "GeometryTargetGL.h"
#include "Engine.h"
#include "utils/Logger.h"

#ifdef _OPENGL_RENDER_
#include "TransformFeedbackGL.h"
#include "BufferGL.h"
#include "RenderStateGL.h"
#include "QueryGL.h"
#include "GL/glew.h"

namespace v3d
{
namespace renderer
{

using namespace gl;

CGeometryTargetGL::CGeometryTargetGL()
    : m_transformfeedback(nullptr)
    , m_queryId(0U)

    , m_initialized(false)
{
    LOG_DEBUG("CGeometryTargetGL: CGeometryTargetGL constructor %x", this);
}

CGeometryTargetGL::~CGeometryTargetGL()
{
    ASSERT(!m_transformfeedback, "Transformfedback doesn't deleted");
    ASSERT(m_queryId == 0, "Query doesn't deleted");

    LOG_DEBUG("CGeometryTargetGL: CGeometryTargetGL destructor %x", this);
}

void CGeometryTargetGL::bind()
{
    if (!m_initialized)
    {
        return;
    }

    CRenderStateGL::rasterizerEnable(false);

    //if (m_transformfeedback)
    //{
    //    m_transformfeedback->bind();

    //}

    if (m_separated)
    {
        //TODO:
    }
    else
    {
        for (auto& buff : m_bufferList)
        {
            buff._buffer->bindToTarget(EBufferTarget::eTransformFeedbackBuffer, 0, 0);
        }
    }

    QueryGL::beginQuery(QueryGL::eFeedbackPrimitivesWrittenTarget, m_queryId);
    TransformFeedbackGL::begin(m_mode);

    ENGINE_RENDERER->checkForErrors("CGeometryTargetGL: Bind TransformFeedback Error");
}

void CGeometryTargetGL::unbind()
{
    if (!m_initialized)
    {
        return;
    }

    TransformFeedbackGL::end();
    QueryGL::endQuery(QueryGL::eFeedbackPrimitivesWrittenTarget);

    CRenderStateGL::rasterizerEnable(true);

    //TODO: glFlush();

    s32 result;
    QueryGL::getQueryResults(m_queryId, &result);

    if (m_separated)
    {
        //TODO:
    }
    else
    {
        for (auto& buff : m_bufferList)
        {
            buff._result = result;
        }
    }

    ENGINE_RENDERER->checkForErrors("CGeometryTargetGL: Unbind TransformFeedback Error");
}

bool CGeometryTargetGL::create()
{
    GLint originTransformFeedback = 0;
    glGetIntegerv(GL_TRANSFORM_FEEDBACK_BUFFER_BINDING, &originTransformFeedback);

    if (m_bufferList.empty())
    {
        return false;
    }

    if (m_separated)
    {
        //TODO:
    }
    else
    {
        u32 offset = 0;
        for (auto& buff : m_bufferList)
        {
            if (!buff._buffer)
            {
                buff._buffer = new BufferGL(EBufferTarget::eTransformFeedbackBuffer);
            }

            auto componentSize = [](EShaderDataType type) -> u32
            {
                switch (type)
                {
                case EShaderDataType::eTypeInt:
                    return sizeof(GLint);

                case EShaderDataType::eTypeFloat:
                    return sizeof(GLfloat);

                case EShaderDataType::ETypeDouble:
                    return sizeof(GLdouble);

                case EShaderDataType::eTypeVector2:
                    return sizeof(GLfloat) * 2;

                case EShaderDataType::eTypeVector3:
                    return sizeof(GLfloat) * 3;

                case EShaderDataType::eTypeVector4:
                    return sizeof(GLfloat) * 4;

                case EShaderDataType::eTypeMatrix3:
                case EShaderDataType::eTypeMatrix4:
                default:
                    return 0;
                }
            };

            buff._buffer->set(EDataUsageType::eReadStatic, componentSize(buff._type) * buff._size, nullptr);
        }
    }

    //m_transformfeedback = new TransformFeedbackGL();
    QueryGL::genQuery(m_queryId);

    ENGINE_RENDERER->checkForErrors("CGeometryTargetGL: init TransformFeedback Error");

    if (originTransformFeedback > 0)
    {
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, originTransformFeedback);
    }

    ENGINE_RENDERER->checkForErrors("CGeometryTargetGL: init TransformFeedback Error");

    m_initialized = true;

    return true;
}

void CGeometryTargetGL::destroy()
{
    if (!m_initialized)
    {
        return;
    }

    QueryGL::deleteQuery(m_queryId);

    if (m_transformfeedback)
    {
        m_transformfeedback->unbind();

        delete m_transformfeedback;
        m_transformfeedback = nullptr;
    }
}

} //namespace renderer
} //namespace v3d

#endif //_OPENGL_RENDER_
