#ifndef _V3D_RENDERER_GL_H_
#define _V3D_RENDERER_GL_H_

#include "renderer/Renderer.h"

namespace v3d
{
namespace renderer
{
namespace gl
{

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    
    /**
    * Inherited class for general render management. GL render only.
    */
    class CRendererGL : public CRenderer
    {
    public:
    
        CRendererGL(const ContextPtr& context);
        ~CRendererGL();
    
        void                init()                                                                  override;
    
        void                preRender(bool clear = false)                                           override;
        void                postRender()                                                            override;

        platform::ERenderType getRenderType() const                                                 override;

        ShaderPtr           makeSharedShader()                                                      override;
        ShaderProgramPtr    makeSharedProgram()                                                     override;
        GeometryPtr         makeSharedGeometry(const CRenderTechnique* technique)                   override;
        RenderStatePtr      makeSharedRenderState()                                                 override;

        RenderTargetPtr     makeSharedRenderTarget()                                                override;
        GeometryTargetPtr   makeSharedGeometryTarget()                                              override;

        bool                isLocked() const;

    private:

        void                resetTextures()                                                          override;

        bool                m_isLocked;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace gl
} //namespace renderer
} //namespace v3d

#endif //_V3D_RENDERER_GL_H_
