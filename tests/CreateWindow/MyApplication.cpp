#include "MyApplication.h"

using namespace f3d;

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
    LOG_INFO("---------Init Application--------");

    //TODO: user data
    scene::CShape* cube = static_cast<scene::CShape*>(getSceneManager()->addCube());
    cube->getMaterial()->loadRenderTechique("default.xml");

	BaseApplication::getInputEventHandler()->connectKeyboardEvent(std::bind(&MyApplication::onKeyboard, this, std::placeholders::_1));
}

void MyApplication::run()
{
	//TODO: main loop
}

void MyApplication::onKeyboard(const f3d::event::SKeyboardInputEventPtr& event)
{
	int a = 0;
}
