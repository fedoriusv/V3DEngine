#ifndef _V3D_ENGINE_H_
#define _V3D_ENGINE_H_

#include "platform/Platform.h"
#include "platform/Window.h"
#include "event/InputEventHandler.h"
#include "scene/SceneManager.h"
#include "renderer/Renderer.h"
#include "context/DriverContext.h"

namespace v3d
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    class CEngine : public Singleton<CEngine>
    {
    public:

        CEngine();
        virtual                             ~CEngine();

        const platform::PlatformPtr&        getPlatform()          const;
        const event::InputEventHandlerPtr&  getInputEventHandler() const;
        const scene::SceneManagerPtr&       getSceneManager()      const;
        const platform::WindowPtr           getWindow()            const;
        const renderer::RendererPtr         getRenderer()          const;
        const renderer::DriverContextPtr    getContext()           const;

        bool                                init();
        bool                                begin();
        bool                                end();

    private:

        platform::PlatformPtr               m_platform;
        event::InputEventHandlerPtr         m_inputEventHandler;
        scene::SceneManagerPtr              m_scene;

    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    #define RENDERER        CEngine::getInstance()->getRenderer()
    #define INPUT_EVENTS    CEngine::getInstance()->getInputEventHandler()
    #define WINDOW          CEngine::getInstance()->getWindow()
    #define DRIVER_CONTEXT  CEngine::getInstance()->getContext()

    /////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif //_V3D_ENGINE_H_
