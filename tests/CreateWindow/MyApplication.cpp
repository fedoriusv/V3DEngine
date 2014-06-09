#include "MyApplication.h"

using namespace v3d;

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
    scene::CShape* cube = static_cast<scene::CShape*>(getSceneManager()->addCube(0,core::Vector3D(0,0,-5)));
    cube->getMaterial()->loadRenderTechique("shaders/default.xml");

    getSceneManager()->addCamera(0, core::Vector3D(0, 1, 0), core::Vector3D(0, 0, -3));

	BaseApplication::getInputEventHandler()->connectKeyboardEvent(std::bind(&MyApplication::onKeyboard, this, std::placeholders::_1));
}

void MyApplication::run()
{
	//TODO: main loop
}

void MyApplication::onKeyboard(const event::SKeyboardInputEventPtr& event)
{
	int a = 0;
}
