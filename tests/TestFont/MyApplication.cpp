#include "MyApplication.h"

using namespace v3d;
using namespace v3d::core;
using namespace v3d::scene;

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
    //CShape* cube = static_cast<CShape*>(getSceneManager()->addSample(0, Vector3D(0, 0, -5)));
    //cube->setName("cube");
    //cube->getMaterial()->setRenderTechnique("shaders/simple.xml");

    scene::CFont* font = static_cast<scene::CFont*>(BaseApplication::getSceneManager()->addFont(0, "text", "fonts/arial.ttf", core::Vector3D(0, 0, -4)));
    font->getMaterial()->setRenderTechnique("shaders/font.xml");
    //font->getMaterial()->setTexture(0, "textures/box.jpg");
  
    scene::CNode* camera = BaseApplication::getSceneManager()->addCamera(0, Vector3D(0, 0, 0), Vector3D(0.f, 0.f, -1.0f));
    camera->setName("camera");

    BaseApplication::getSceneManager()->setBackgroundColor(Vector3D(0.1,0.1,0.1));
    
	BaseApplication::getInputEventHandler()->connectKeyboardEvent(std::bind(&MyApplication::onKeyboard, this, std::placeholders::_1));
    BaseApplication::getInputEventHandler()->connectMouseEvent(std::bind(&MyApplication::onMouse, this, std::placeholders::_1));
}

void MyApplication::run()
{
	//TODO: main loop
}

void MyApplication::onMouse(const v3d::event::SMouseInputEventPtr& event)
{
}

void MyApplication::onKeyboard(const event::SKeyboardInputEventPtr& event)
{
    if (event->_event == event::EKeyboardInputEvent::eKeyboardPressDown)
    {
        if (event->_key == EKeyCode::eKeyEscape)
        {
            getPlatform()->closeWindow();
        }
    }
}
