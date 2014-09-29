#include "Engine.h"
#include "platform/Platform.h"
#include "event/InputEventHandler.h"
#include "scene/RenderTechniqueManager.h"
#include "scene/TextureManager.h"
#include "scene/FontManager.h"
#include "scene/ModelManager.h"

using namespace v3d;
using namespace v3d::platform;
using namespace v3d::event;
using namespace v3d::scene;
using namespace v3d::renderer;

CEngine::CEngine()
    : m_platform(nullptr)
    , m_inputEventHandler(nullptr)
    , m_scene(nullptr)
{
    m_platform = std::make_shared<CPlatform>();
    m_inputEventHandler = std::make_shared<CInputEventHandler>();
    m_scene = std::make_shared<CSceneManager>();
}

CEngine::~CEngine()
{
    CRenderTechniqueManager::freeInstance();
    CTextureManager::freeInstance();
    CFontManager::freeInstance();
    CModelManager::freeInstance();
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

const PlatformPtr& CEngine::getPlatform() const
{
    return m_platform;
}

const InputEventHandlerPtr& CEngine::getInputEventHandler() const
{
    return m_inputEventHandler;
}

const SceneManagerPtr& CEngine::getSceneManager() const
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

const WindowPtr CEngine::getWindow() const
{
    if (!m_platform)
    {
        return nullptr;
    }

    return m_platform->getWindow();
}

const RendererPtr CEngine::getRenderer() const
{
    if (!m_platform)
    {
        return nullptr;
    }

    return m_platform->getRenderer();
}