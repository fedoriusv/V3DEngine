#ifndef _V3D_QUERY_GL_H_
#define _V3D_QUERY_GL_H_

#include "common.h"

namespace v3d
{
namespace renderer
{
namespace gl
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * Module for query management. GL render only.
    */
    class QueryGL
    {
    public:

        enum EQueryTarget
        {
            eSamplesPassedTarget,
            eAnySamplesPassedTarget,
            ePrimitivesGeneratedTarget,
            eFeedbackPrimitivesWrittenTarget,
            eTimeElapsed,

            eQueryTargetCount
        };

        static void         genQuery(u32& query);
        static void         deleteQuery(u32& query);

        static void         beginQuery(EQueryTarget target, u32 query);
        static void         endQuery(EQueryTarget target);

        static void         getQueryResults(u32 query, s32* result);

    private:

        static u32          s_currentQuery;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace gl
} //namespace renderer
} //namespace v3d

#endif //_V3D_QUERY_GL_H_
