#ifndef _V3D_RENDERER_GL_H_
#define _V3D_RENDERER_GL_H_

#include "renderer/Renderer.h"

namespace v3d
{
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    
    /**
    * Inherited class for general render management. GL render only.
    */
    class CRendererGL : public CRenderer
    {
    public:
    
        CRendererGL(const DriverContextPtr& context);
        ~CRendererGL();
    
        void                init()                                                                  override;
    
        void                preRender(bool clear = false)                                           override;
        void                postRender()                                                            override;

        ShaderPtr           makeSharedShader()                                                      override;
        ShaderProgramPtr    makeSharedProgram()                                                     override;
        GeometryPtr         makeSharedGeometry(const CRenderTechnique* technique)                   override;
        CTexture*           makeSharedTexture()                                                     override;
        RenderStatePtr      makeSharedRenderState()                                                 override;
        RenderTargetPtr     makeSharedRenderTarget()                                                override;

        bool                isLocked() const;

    private:

        void                resetTexture()                                                          override;

        bool                m_isLocked;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_V3D_RENDERER_GL_H_
