#include "BaseApplication.h"
#include "utils/Logger.h"

namespace v3d
{

using namespace event;
using namespace scene;
using namespace platform;
using namespace renderer;

BaseApplication::BaseApplication(int& argc, char** argv)
{
    m_engine = Engine::getInstance();
}

BaseApplication::~BaseApplication()
{
    Engine::freeInstance();
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
	BaseApplication::getSceneManager()->init();

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

const WindowPtr BaseApplication::getWindow() const
{
    return m_engine->getWindow();
}

const InputEventHandlerPtr BaseApplication::getInputEventHandler() const
{
    return m_engine->getInputEventHandler();
}

const SceneManagerPtr BaseApplication::getSceneManager() const
{
    return m_engine->getSceneManager();
}

Engine* BaseApplication::getEngine()
{
    return m_engine;
}

} //namespace v3d
