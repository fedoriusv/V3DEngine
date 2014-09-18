#include "BaseApplication.h"
#include "utils/Logger.h"

using namespace v3d;
using namespace v3d::platform;

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

platform::PlatformPtr BaseApplication::getPlatform() const
{
    return m_engine->getPlatform();
}

platform::WindowPtr BaseApplication::getWindow() const
{
    return m_engine->getPlatform()->getWindow();
}

event::InputEventHandlerPtr BaseApplication::getInputEventHandler() const
{
    return m_engine->getInputEventHandler();
}

scene::SceneManagerPtr BaseApplication::getSceneManager() const
{
    return m_engine->getSceneManager();
}
