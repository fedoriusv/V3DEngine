#ifndef _V3D_GEOMETRY_TARGET_GL_H_
#define _V3D_GEOMETRY_TARGET_GL_H_

#include "renderer/GeometryTarget.h"

namespace v3d
{
namespace renderer
{
namespace gl
{
    class TransformFeedbackGL;
}

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * Inherited class used transform feedback target. GL render only.
    * Used output to vertex buffer through transform feedback
    */
    class CGeometryTargetGL final : public CGeometryTarget
    {
    public:
    
        CGeometryTargetGL();
        CGeometryTargetGL(const CGeometryTargetGL&)            = delete;
        CGeometryTargetGL& operator=(const CGeometryTargetGL&) = delete;

        ~CGeometryTargetGL();

        void                        bind()      override;
        void                        unbind()    override;

    private:

        bool                        create()    override;
        void                        destroy()   override;

        gl::TransformFeedbackGL*    m_transformfeedback;
        u32                         m_queryId;

        bool                        m_initialized;
    };
    
    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_V3D_GEOMETRY_TARGET_GL_H_
