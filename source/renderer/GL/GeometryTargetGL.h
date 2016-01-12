#ifndef _V3D_GEOMETRY_TARGET_GL_H_
#define _V3D_GEOMETRY_TARGET_GL_H_

#include "renderer/GeometryTarget.h"
#include "renderer/GeometryTypes.h"

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
    class CGeometryTargetGL : public CGeometryTarget
    {
    public:
    
        CGeometryTargetGL();
        ~CGeometryTargetGL();

        void                        bind()      override;
        void                        unbind()    override;

    private:

        bool                        create()    override;
        void                        destroy()   override;

        gl::TransformFeedbackGL*    m_transformfeedback;
        u32                         m_queryId;
        EPrimitivesMode             m_mode;

        bool                        m_separated;

        bool                        m_initialized;
    };
    
    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_V3D_GEOMETRY_TARGET_GL_H_
