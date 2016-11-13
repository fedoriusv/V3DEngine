#include "Engine.h"
#include "platform/Platform.h"
#include "event/InputEventHandler.h"
#include "event/EventManager.h"
#include "scene/RenderTechniqueManager.h"
#include "scene/TextureManager.h"
#include "scene/FontManager.h"
#include "scene/ModelManager.h"
#include "scene/TargetManager.h"
#include "scene/ShaderManager.h"
#include "utils/Logger.h"

namespace v3d
{

using namespace event;
using namespace scene;
using namespace platform;

Engine::Engine()
    : m_window(nullptr)
    , m_inputEventHandler(nullptr)
    , m_render(nullptr)
    , m_scene(nullptr)
{
    m_inputEventHandler = std::make_shared<CInputEventHandler>();
    m_scene = std::make_shared<CSceneManager>();

    m_inputEventHandler->setEnableEvents(true);
}

Engine::~Engine()
{
    if (m_render)
    {
        //m_render->destroy()
    }

    if (m_context)
    {
        m_context->destroy();
    }

    m_inputEventHandler->setEnableEvents(false);

    CRenderTechniqueManager::freeInstance();
    TextureManager::freeInstance();
    CFontManager::freeInstance();
    CModelManager::freeInstance();
    CEventManager::freeInstance();
    CTargetManager::freeInstance();
    ShaderManager::freeInstance();
}

bool Engine::init()
{
    if (!m_context || !m_render)
    {
        return false;
    }

    m_render->init();

    return true;
}

bool Engine::createWindowWithContext(const core::Dimension2D& size, const core::Point2D& pos, bool isFullscreen, bool isResizeble, platform::ERenderType driverType)
{
    m_window = platform::Platform::createWindow(size, pos, isFullscreen, isResizeble);
    if (!m_window)
    {
        LOG_ERROR("Engine::createWindowWithContext: Can't create window");
        system("pause");

        return false;
    }

    m_context = platform::Platform::createContext(m_window, driverType);
    if (!m_context)
    {
        LOG_ERROR("Engine::createWindowWithContext: Can't create context");
        m_window->close();
        system("pause");

        return false;
    }

    m_render = platform::Platform::createRenderer(m_context, driverType);
    if (!m_render)
    {
        LOG_ERROR("Engine::createWindowWithContext: Can't create render");
        m_context->destroy();
        m_window->close();
        system("pause");

        return false;
    }

    return true;
}

const InputEventHandlerPtr Engine::getInputEventHandler() const
{
    return m_inputEventHandler;
}

const SceneManagerPtr Engine::getSceneManager() const
{
    return m_scene;
}

bool Engine::begin()
{
    if (!m_window)
    {
        return false;
    }

    m_inputEventHandler->update();
    if (m_render)
    {
        m_scene->draw();
    }

    return m_window->begin();
}

bool Engine::end()
{
    if (!m_window)
    {
        return false;
    }

    return m_window->end();
}

const WindowPtr Engine::getWindow() const
{
    return m_window;
}

const renderer::RendererPtr Engine::getRenderer() const
{
    return m_render;
}

const renderer::ContextPtr Engine::getContext() const
{
    return m_context;
}

} //namespace v3d
