#include "BaseApplication.h"
#include "utils/Logger.h"

using namespace v3d;
using namespace v3d::event;
using namespace v3d::scene;
using namespace v3d::platform;
using namespace v3d::renderer;

BaseApplication::BaseApplication(int& argc, char** argv)
{
    m_engine = CEngine::getInstance();
}

BaseApplication::~BaseApplication()
{
    CEngine::freeInstance();
}

int BaseApplication::exec()
{
    if (!m_engine->init())
    {
        delete this;
        return 0;
    }

    LOG_INFO("---------Init Application--------");
    this->init();
    getSceneManager()->init();

    LOG_INFO("---------Run Application--------");
    while (m_engine->begin())
    {
        this->run();

        m_engine->end();
    }

    LOG_INFO("---------Exit Application--------");

    delete this;
    return 0;
}

const PlatformPtr& BaseApplication::getPlatform() const
{
    return m_engine->getPlatform();
}

const WindowPtr BaseApplication::getWindow() const
{
    return m_engine->getPlatform()->getWindow();
}

const InputEventHandlerPtr& BaseApplication::getInputEventHandler() const
{
    return m_engine->getInputEventHandler();
}

const SceneManagerPtr& BaseApplication::getSceneManager() const
{
    return m_engine->getSceneManager();
}
