#pragma once

#include "renderer/Geometry.h"

#ifdef _OPENGL_RENDER_

namespace v3d
{
namespace renderer
{
namespace gl
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * Module for transform feedback management. GL render only.
    */
    class TransformFeedbackGL
    {
    public:

        TransformFeedbackGL();
        ~TransformFeedbackGL();

        bool            bind();
        void            unbind();

        static void     begin(EPrimitivesMode mode);
        static void     end();
        static void     pause();
        static void     resume();

        static void     transformFeedbackVaryings(u32 program, const std::vector<const c8*>& list);

    private:

        u32             m_id;

        static bool     s_started;
        static bool     s_paused;

    private:

        static u32      s_currentTransformFeedbackID;
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace gl
} //namespace renderer
} //namespace v3d

#endif //_OPENGL_RENDER_
