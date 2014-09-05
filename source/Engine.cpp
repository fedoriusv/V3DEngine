#include "Engine.h"
#include "platform/Platform.h"
#include "event/InputEventHandler.h"
#include "scene/RenderTechniqueManager.h"
#include "scene/TextureManager.h"
#include "scene/FontManager.h"

using namespace v3d;
using namespace v3d::platform;
using namespace v3d::event;
using namespace v3d::scene;

CEngine::CEngine()
: m_platform(nullptr)
, m_inputEventHandler(nullptr)
, m_scene(nullptr)
{
    m_platform = std::make_shared<CPlatform>(CPlatform());
    m_inputEventHandler = std::make_shared<CInputEventHandler>(CInputEventHandler());
    m_scene = std::make_shared<CSceneManager>(CSceneManager());
}

CEngine::~CEngine()
{
    CRenderTechniqueManager::freeInstance();
    CTextureManager::freeInstance();
    CFontManager::freeInstance();
}

bool CEngine::init()
{
    if (m_platform->hasError())
    {
        return false;
    }

    if (!m_platform->init())
    {
        return false;
    }

    return true;
}

PlatformPtr CEngine::getPlatform() const
{
    return m_platform;
}

event::InputEventHandlerPtr CEngine::getInputEventHandler() const
{
    return m_inputEventHandler;
}

scene::SceneManagerPtr CEngine::getSceneManager() const
{
    return m_scene;
}

bool CEngine::begin()
{
    if (!m_platform)
    {
        return false;
    }

    m_inputEventHandler->update();
    m_scene->draw();

    return m_platform->begin();
}

bool CEngine::end()
{
    if (!m_platform)
    {
        return false;
    }

    return m_platform->end();
}

platform::WindowPtr CEngine::getWindow() const
{
    if (!m_platform)
    {
        return nullptr;
    }

    return m_platform->getWindow();
}

renderer::RendererPtr CEngine::getRenderer() const
{
    if (!m_platform)
    {
        return nullptr;
    }

    return m_platform->getRenderer();
}