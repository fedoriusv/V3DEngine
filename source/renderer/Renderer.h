#pragma once

#include "context/DeviceContext.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Geometry.h"
#include "Texture.h"
#include "RenderJob.h"
#include "RenderState.h"
#include "RenderPass.h"

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
    class RenderThread;

    /**
    * Interface for general render management.
    */
    class IRenderer : public std::enable_shared_from_this<IRenderer>
    {
    public:

        IRenderer(const ContextPtr context, bool isThreaded);
        virtual                         ~IRenderer();

        bool                            create();

        virtual platform::ERenderType   getRenderType() const = 0;

        //main thread
        void                            init();

        void                            beginFrame();
        void                            endFrame();
        void                            presentFrame();

        void                            draw();

    private:

        //render thread
        virtual void                    immediateInit() = 0;

        virtual void                    immediaterBeginFrame() = 0;
        virtual void                    immediateEndFrame() = 0;
        virtual void                    immediatePresentFrame() = 0;

        virtual void                    immediateDraw() = 0;


    public:
        void                        draw(const RenderJobPtr& job);

        
        virtual void                preRender(bool clear = false)  = 0;
        virtual void                postRender()                   = 0;





        void                        updateCamera(scene::CCamera* camera);

        void                        checkForErrors(const std::string& location = "");

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

        virtual void                resetTextures()     = 0;

        void                        updateLight(const core::Matrix4D& transform, const RenderPassPtr& pass);
        void                        updateMaterial(const MaterialPtr& material, const RenderPassPtr& pass);
        void                        updateTransform(const core::Matrix4D& transform, const RenderPassPtr& pass);
        void                        updateTexture(MaterialPtr& material, const RenderPassPtr& pass);
        void                        updateAdvanced(const RenderPassPtr& pass);

        ContextWPtr                 m_context;
        u32                         m_frameIndex;

        RenderThread*               m_renderThread;
        bool                        m_isThreaded;

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

    using RendererPtr  = std::shared_ptr<IRenderer>;
    using RendererWPtr = std::weak_ptr<IRenderer>;

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d
