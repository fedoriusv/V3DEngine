#ifndef _V3D_RENDERER_H_
#define _V3D_RENDERER_H_

#include "context/DriverContext.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Geometry.h"
#include "Texture.h"
#include "RenderJob.h"

#include "RenderTarget.h"
#include "GeometryTarget.h"

namespace v3d
{
namespace scene
{
    class CLight;
    class CSceneManager;
    class CCamera;
}
namespace renderer
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CMaterial;

    /**
    * Interface for general render management.
    */
    class CRenderer
    {
    public:

        CRenderer(const DriverContextPtr& context);
        virtual                     ~CRenderer();

        const DriverContextPtr&     getContext() const;

        virtual void                init()                         = 0;
        
        virtual void                preRender(bool clear = false)  = 0;
        virtual void                postRender()                   = 0;

        void                        draw(const RenderJobPtr& job);

        void                        updateCamera(scene::CCamera* camera);

        void                        checkForErrors(const std::string& location = "");

        virtual CTexture*           createTexture() = 0;

        virtual ShaderPtr           makeSharedShader()                                                      = 0;
        virtual ShaderProgramPtr    makeSharedProgram()                                                     = 0;
        virtual GeometryPtr         makeSharedGeometry(const CRenderTechnique* technique)                   = 0;
        virtual RenderStatePtr      makeSharedRenderState()                                                 = 0;

        virtual RenderTargetPtr     makeSharedRenderTarget()                                                = 0;
        virtual GeometryTargetPtr   makeSharedGeometryTarget()                                              = 0;
#ifdef _DEBUG
        void                        setDebugMode(bool active);
        bool                        isDebugMode() const;
#endif

        const core::Rect32&         getViewportSize() const;
        u32                         getFrameIndex()   const;

        const RenderTargetPtr&      getDefaultRenderTarget() const;
        const RenderTargetPtr&      getCurrentRenderTarget() const;
        void                        setCurrentRenderTarget(const RenderTargetPtr& target);

        core::Point2D               convertPosCanvasToScreen(const core::Vector2D& pos);
        core::Vector2D              convertPosScreenToCanvas(const core::Point2D& pos);

    protected:

        friend                      scene::CSceneManager;

        virtual void                resetTexture()  = 0;

        void                        updateLight(const core::Matrix4D& transform, const RenderPassPtr& pass);
        void                        updateMaterial(const MaterialPtr& material, const RenderPassPtr& pass);
        void                        updateTransform(const core::Matrix4D& transform, const RenderPassPtr& pass);
        void                        updateTexture(MaterialPtr& material, const RenderPassPtr& pass);
        void                        updateAdvanced(const RenderPassPtr& pass);

        DriverContextPtr            m_context;
        u32                         m_frameIndex;

        RenderTargetPtr             m_defaultRenderTarget;
        RenderTargetPtr             m_currentRenderTarget;

#ifdef _DEBUG
        bool                        m_debugMode;
#endif
        scene::CCamera*             m_camera;

        void                        addLight(scene::CLight* lights);
        std::vector<scene::CLight*> m_lightList;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CRenderer>  RendererPtr;

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d

#endif //_V3D_RENDERER_H_
