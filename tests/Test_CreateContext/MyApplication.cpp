#include "MyApplication.h"
#include "Valera3D.h"

#include "stream/StreamManager.h"

using namespace v3d;
using namespace core;
using namespace scene;
using namespace stream;
using namespace platform;
using namespace event;

MyApplication::MyApplication(int& argc, char** argv)
    : BaseApplication(argc, argv)
{
    BaseApplication::getPlatform()->createWindowWithContext(Dimension2D(1024, 768), Point2D(100, 100), false, false, EDriverType::eDriverDirect3D);
}

MyApplication::~MyApplication()
{
}

void MyApplication::init()
{
    BaseApplication::getInputEventHandler()->connectKeyboardEvent(std::bind(&MyApplication::onKeyboard, this, std::placeholders::_1));
}

void MyApplication::run()
{
    BaseApplication::getWindow()->setCaption("Test. FPS: " + std::to_string(v3d::CEngine::getInstance()->getFPS()));
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
            getPlatform()->closeWindow();
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
