#include "QueryGL.h"
#include "Engine.h"

#ifdef _OPENGL_RENDER_
#include "GL/glew.h"

namespace v3d
{
namespace renderer
{
namespace gl
{

GLenum EQueryTargetGL[QueryGL::eQueryTargetCount] =
{
    GL_SAMPLES_PASSED,
    GL_ANY_SAMPLES_PASSED,
    GL_PRIMITIVES_GENERATED,
    GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN,
    GL_TIME_ELAPSED
};

void QueryGL::genQuery(u32 & query)
{
    glGenQueries(1, &query);
}

void QueryGL::deleteQuery(u32 & query)
{
    if (query != 0)
    {
        ASSERT(glIsQuery(query), "Invalid Query index");
        glDeleteQueries(1, &query);
        query = 0;
    }
}

void QueryGL::beginQuery(EQueryTarget target, u32 query)
{
    glBeginQuery(EQueryTargetGL[target], query);
    ASSERT(glIsQuery(query), "Invalid Query index");

    ENGINE_RENDERER->checkForErrors("QueryGL::beginQuery Error");
}

void QueryGL::endQuery(EQueryTarget target)
{
    glEndQuery(EQueryTargetGL[target]);

    ENGINE_RENDERER->checkForErrors("QueryGL::endQuery Error");
}

void QueryGL::getQueryResults(u32 query, s32* result)
{
    ASSERT(glIsQuery(query), "Invalid Query index");
    glGetQueryObjectiv(query, GL_QUERY_RESULT, result);
}

} //namespace gl
} //namespace renderer
} //namespace v3d

#endif //_OPENGL_DRIVER_
