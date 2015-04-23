#include "MyApplication.h"
#include "Valera3D.h"

using namespace v3d;
using namespace core;
using namespace scene;
using namespace event;

MyApplication::MyApplication(int& argc, char** argv)
    : BaseApplication(argc, argv)
{
    BaseApplication::getPlatform()->createWindowWithContext(core::Dimension2D(1024, 768));
}

MyApplication::~MyApplication()
{
}

void MyApplication::init()
{
    CText* freetype = BaseApplication::getSceneManager()->addText(0, "Hello world! Freetype Font", "fonts/comic.ttf", core::Vector3D(0, 2, -10));
    freetype->setAlignMode(CText::eFontAlignCenter);
    freetype->getMaterial()->setRenderTechnique("shaders/freetype_font.xml");

    CText* bitmap = BaseApplication::getSceneManager()->addText(0, "Hello world! Bitmap Font", "fonts/arial.fnt", core::Vector3D(0, 0, -10));
    bitmap->getMaterial()->setRenderTechnique("shaders/bitmap_font.xml");
  
    CNode* camera = BaseApplication::getSceneManager()->addCamera(0, Vector3D(0, 0, 0), Vector3D(0.f, 0.f, -1.0f));
    camera->setName("camera");

    BaseApplication::getInputEventHandler()->connectKeyboardEvent(std::bind(&MyApplication::onKeyboard, this, std::placeholders::_1));
}

void MyApplication::run()
{
    //TODO: main loop
}

void MyApplication::onKeyboard(const KeyboardInputEventPtr& event)
{
    if (event->_event == EKeyboardInputEvent::eKeyboardPressDown)
    {
        if (event->_key == EKeyCode::eKeyEscape)
        {
            getPlatform()->closeWindow();
        }
    }
}
