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
    , m_scene(nullptr)
{
    m_inputEventHandler = std::make_shared<CInputEventHandler>();
    m_scene = std::make_shared<CSceneManager>();

    m_inputEventHandler->setEnableEvents(true);
}

Engine::~Engine()
{
    m_inputEventHandler->setEnableEvents(false);

    CRenderTechniqueManager::freeInstance();
    CTextureManager::freeInstance();
    CFontManager::freeInstance();
    CModelManager::freeInstance();
    CEventManager::freeInstance();
    CTargetManager::freeInstance();
    CShaderManager::freeInstance();
}

bool Engine::init()
{
    if (!m_device->init())
    {
        return false;
    }

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

    renderer::ContextPtr context = platform::Platform::createContext(m_window, driverType);
    if (!context)
    {
        LOG_ERROR("Engine::createWindowWithContext: Can't create context");
        system("pause");

        return false;
    }

    renderer::RendererPtr renderer = platform::Platform::createRenderer(context, driverType);
    if (!renderer)
    {
        LOG_ERROR("Engine::createWindowWithContext: Can't create render");
        system("pause");

        return false;
    }

    m_device = std::make_unique<Device>(renderer, true);
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
    if (m_device->getRenderer())
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
    if (!m_device)
    {
        return nullptr;
    }

    return m_device->getRenderer();
}

const renderer::ContextPtr Engine::getContext() const
{
    if (!m_device)
    {
        return nullptr;
    }

    return m_device->getRenderer()->getContext();
}

} //namespace v3d
