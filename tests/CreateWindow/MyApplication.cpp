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
    //TODO: init
    scene::CShape* cube = static_cast<scene::CShape*>(getSceneManager()->addCube(0,core::Vector3D(0,0,-5)));
    //CShape* cube = static_cast<CShape*>(getSceneManager()->addSample(0, Vector3D(0, 0, -5)));
    cube->setName("cube");
    cube->getMaterial()->setRenderTechnique("shaders/sample.xml");
    //cube->getMaterial()->setTexture(0, "textures/box.jpg");

    cube->setRotation(Vector3D(10, 120, 0));
    Vector3D test = cube->getRotation();

    getSceneManager()->addCamera(0, Vector3D(0, 0, 0), Vector3D(0, 0, -1), Vector3D(0, 1, 0));

	BaseApplication::getInputEventHandler()->connectKeyboardEvent(std::bind(&MyApplication::onKeyboard, this, std::placeholders::_1));
}

void MyApplication::run()
{
	//TODO: main loop
}

void MyApplication::onKeyboard(const event::SKeyboardInputEventPtr& event)
{
    f32 step = 0.1f;
    f32 angle = 5.0f;

    CNode* node = getSceneManager()->getObjectByName("cube");
    if (!node)
    {
        return;
    }

    if (event->_key == EKeyCode::eKeyKey_W)
    {
        node->setPosition(Vector3D(node->getPosition().x, node->getPosition().y, node->getPosition().z + step));
    }
    if (event->_key == EKeyCode::eKeyKey_S)
    {
        node->setPosition(Vector3D(node->getPosition().x, node->getPosition().y, node->getPosition().z - step));
    }
    if (event->_key == EKeyCode::eKeyUp)
    {
        node->setRotation(Vector3D(node->getRotation().x + angle, node->getRotation().y, node->getRotation().z));
    }
    if (event->_key == EKeyCode::eKeyDown)
    {
        node->setRotation(Vector3D(node->getRotation().x - angle, node->getRotation().y, node->getRotation().z));
    }
    if (event->_key == EKeyCode::eKeyLeft)
    {
        node->setRotation(Vector3D(node->getRotation().x, node->getRotation().y + angle, node->getRotation().z));
    }
    if (event->_key == EKeyCode::eKeyRight)
    {
        node->setRotation(Vector3D(node->getRotation().x, node->getRotation().y - angle, node->getRotation().z));
    }

    getPlatform()->getWindow()->setCaption("x= " + std::to_string(node->getRotation().x) + "; y = " + std::to_string(node->getRotation().y) + "; z = " + std::to_string(node->getRotation().z));
    
}
