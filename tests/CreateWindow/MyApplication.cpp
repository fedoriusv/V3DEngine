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
    LOG_INFO("---------Init Application--------");

    //TODO: user data
    //scene::CShape* cube = static_cast<scene::CShape*>(getSceneManager()->addCube(0,core::Vector3D(0,0,-5)));
    CShape* cube = static_cast<CShape*>(getSceneManager()->addSample(0, Vector3D(0, 0, -5)));
    cube->setName("cube");
    cube->getMaterial()->setRenderTechnique("shaders/sample.xml");
    cube->getMaterial()->setTexture(0, "textures/box.jpg");

    getSceneManager()->addCamera(0, Vector3D(0, 0, 0.0f));

	BaseApplication::getInputEventHandler()->connectKeyboardEvent(std::bind(&MyApplication::onKeyboard, this, std::placeholders::_1));
}

void MyApplication::run()
{
	//TODO: main loop
}

void MyApplication::onKeyboard(const event::SKeyboardInputEventPtr& event)
{
    f32 step = 0.5;

    CNode* node = getSceneManager()->getObjectByName("cube");
    if (event->_key == EKeyCode::eKeyKey_W)
    {
        if (node)
        {
            node->setPosition(Vector3D(node->getPosition().x, node->getPosition().y, node->getPosition().z + step));
        }
        return;
    }

    if (event->_key == EKeyCode::eKeyKey_S)
    {
        if (node)
        {
            node->setPosition(Vector3D(node->getPosition().x, node->getPosition().y, node->getPosition().z - step));
        }
        return;
    }
}
