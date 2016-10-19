#include "MyApplication.h"
#include "Valera3D.h"

#include "stream/StreamManager.h"
#include "scene/TextureManager.h"

using namespace v3d;
using namespace core;
using namespace scene;
using namespace stream;
using namespace platform;
using namespace event;
using namespace renderer;

MyApplication::MyApplication(int& argc, char** argv)
    : BaseApplication(argc, argv)
{
    BaseApplication::getEngine()->createWindowWithContext(Dimension2D(1024, 768), Point2D(100, 100), false, false, ERenderType::eRenderVulkan);
}

MyApplication::~MyApplication()
{
}

void MyApplication::init()
{
    BaseApplication::getInputEventHandler()->connectKeyboardEvent(std::bind(&MyApplication::onKeyboard, this, std::placeholders::_1));

    TexturePtr tex = TextureManager::getInstance()->load("textures/box.jpg");
}

void MyApplication::run()
{
    //BaseApplication::getWindow()->setCaption("Test. FPS: " + std::to_string(BaseApplication::getSceneManager()->getFPS()));
}

void MyApplication::onKeyboard(const KeyboardInputEventPtr& event)
{
    f32 step = 5.0f;
    f32 angle = 5.0f;
    static bool debug = false;

    if (event->_event == EKeyboardInputEvent::eKeyboardPressDown)
    {
        if (event->_key == EKeyCode::eKeyEscape)
        {
            BaseApplication::getWindow()->close();
        }
        if (event->_key == EKeyCode::eKeyKey_N)
        {
            debug = !debug;
#ifdef _DEBUG
            BaseApplication::getSceneManager()->setDebugMode(debug);
#endif
        }
    }
}
