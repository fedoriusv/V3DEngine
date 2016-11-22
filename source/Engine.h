#pragma once

#include "utils/Singleton.h"
#include "platform/Platform.h"
#include "platform/Window.h"
#include "context/DeviceContext.h"
#include "event/InputEventHandler.h"
#include "scene/SceneManager.h"
#include "renderer/Renderer.h"


namespace v3d
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
    * Engine. General module.
    */
    class Engine final : public utils::TSingleton<Engine>
    {
    private:

        Engine();
        ~Engine();

    public:

        bool                                createWindowWithContext(
                                                const core::Dimension2D& size = core::Dimension2D(800U, 600),
                                                const core::Point2D& pos = core::Point2D(100U, 100U),
                                                bool isFullscreen = false,
                                                bool isResizeble = false,
                                                renderer::ERenderType driverType = renderer::ERenderType::eRenderOpenGL);

        const event::InputEventHandlerPtr   getInputEventHandler() const;
        const scene::SceneManagerPtr        getSceneManager()const;
        const platform::WindowPtr           getWindow() const;
        const renderer::RendererPtr         getRenderer() const;
        const renderer::ContextPtr          getContext() const;

        bool                                init();
        bool                                begin();
        bool                                end();

    private:

        friend utils::TSingleton<Engine>;

        platform::WindowPtr                 m_window;
        renderer::ContextPtr                m_context;
        event::InputEventHandlerPtr         m_inputEventHandler;

        renderer::RendererPtr               m_render;
        //physics::PhysicsPtr                 m_physics;
        //audio::Audio                        m_audio;

        scene::SceneManagerPtr              m_scene;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    #define ENGINE_RENDERER        Engine::getInstance()->getRenderer()
    #define ENGINE_INPUT_EVENTS    Engine::getInstance()->getInputEventHandler()
    #define ENGINE_WINDOW          Engine::getInstance()->getWindow()
    #define ENGINE_CONTEXT         Engine::getInstance()->getContext()

    /////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace v3d
