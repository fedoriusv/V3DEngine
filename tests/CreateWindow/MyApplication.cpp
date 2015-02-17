#include "MyApplication.h"
#include "Valera3D.h"

#include "stream/StreamManager.h"

using namespace v3d;
using namespace v3d::core;
using namespace v3d::scene;
using namespace v3d::stream;

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
    scene::CShape* cube = static_cast<scene::CShape*>(getSceneManager()->addCube(0, core::Vector3D(6, 1, 5)));
    //CShape* cube = static_cast<CShape*>(getSceneManager()->addSample(0, Vector3D(0, 0, -5)));
    cube->setName("cube");
    cube->getMaterial()->setRenderTechnique("shaders/texture.xml");
    //cube->getMaterial()->setRenderTechnique("shaders/light.xml");
    cube->getMaterial()->setTexture(0, "textures/box.jpg");

    //TODO: init
    /*scene::CShape* torus = static_cast<scene::CShape*>(BaseApplication::getSceneManager()->addTorus(0, core::Vector3D(0, 1, -10)));
    torus->getMaterial()->setTexture(0, "textures/wall.bmp");v
    torus->getMaterial()->setRenderTechnique("shaders/texture.xml");*/


    scene::CModel* model = static_cast<scene::CModel*>(BaseApplication::getSceneManager()->addModel("models/test.f3d", "shaders/simple.xml"));

    //for (u32 j = 0; j < 5; ++j)
    //{
    //    for (u32 i = 0; i < 5; ++i)
    //    {
    //        scene::CShape* cube1 = static_cast<scene::CShape*>(BaseApplication::getSceneManager()->addCube(0, core::Vector3D(1 + j * 2, -1, 1 + i * 2)));
    //        cube1->getMaterial()->setRenderTechnique("shaders/texture.xml");
    //        //cube1->getMaterial()->setRenderTechnique("shaders/light.xml");
    //        //cube1->getMaterial()->setDiffuseColor(Vector4D(0, 0, 1, 1));
    //        cube1->getMaterial()->setTexture(0, "textures/box.jpg");
    //    }
    //}

    //BaseApplication::getSceneManager()->addLight(0, Vector3D(6, 3, 5))->setName("light");

    scene::CSkybox* skybox = static_cast<scene::CSkybox*>(BaseApplication::getSceneManager()->addSkyBox("textures/skybox/jajlands_ft.jpg", "textures/skybox/jajlands_rt.jpg", "textures/skybox/jajlands_lf.jpg",
        "textures/skybox/jajlands_bk.jpg", "textures/skybox/jajlands_up.jpg", "textures/skybox/jajlands_dn.jpg"));
    skybox->getMaterial()->setRenderTechnique("shaders/skybox.xml");

    //cube->setRotation(Vector3D(10, 120, 0));
    //Vector3D test = cube->getRotation();

    scene::CNode* fpsCamera = BaseApplication::getSceneManager()->addFPSCamera(0, Vector3D(0, 0, 0), Vector3D(0.7f, 0, 0.7f));
    fpsCamera->setName("fpsCamera");
    scene::CNode* camera = BaseApplication::getSceneManager()->addCamera(0, Vector3D(0, 0, 0), Vector3D(0.0f, 0, -1.0f));
    camera->setName("camera");
    
	BaseApplication::getInputEventHandler()->connectKeyboardEvent(std::bind(&MyApplication::onKeyboard, this, std::placeholders::_1));
    BaseApplication::getInputEventHandler()->connectMouseEvent(std::bind(&MyApplication::onMouse, this, std::placeholders::_1));
    BaseApplication::getInputEventHandler()->connectGamepadEvent(std::bind(&MyApplication::onGamepad, this, std::placeholders::_1));
}

void MyApplication::run()
{
    //TODO: main loop
}

void MyApplication::onMouse(const v3d::event::MouseInputEventPtr& event)
{
}

void MyApplication::onGamepad(const v3d::event::GamepadInputEventPtr& event)
{
}

void MyApplication::onKeyboard(const event::KeyboardInputEventPtr& event)
{
    f32 step = 0.1f;
    f32 angle = 5.0f;
    static bool debug = false;

    if (event->_event == event::EKeyboardInputEvent::eKeyboardPressDown)
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
        if (event->_key == EKeyCode::eKeyKey_V)
        {
            CCamera* fpsCamera = static_cast<CCamera*>(getSceneManager()->getObjectByName("fpsCamera"));
            CCamera* camera = static_cast<CCamera*>(getSceneManager()->getObjectByName("camera"));
            if (!fpsCamera || !camera)
            {
                return;
            }

            if (getSceneManager()->isActiveCamera(fpsCamera))
            {
                camera->setPosition(fpsCamera->getPosition());
                camera->setTarget(fpsCamera->getTarget());
                getSceneManager()->setActiveCamera(camera);
            }
            else if (getSceneManager()->isActiveCamera(camera))
            {
                fpsCamera->setPosition(camera->getPosition());
                fpsCamera->setTarget(camera->getTarget());
                getSceneManager()->setActiveCamera(fpsCamera);
            }

        }


        CNode* light = getSceneManager()->getObjectByName("light");
        if (light)
        {
            if (event->_key == EKeyCode::eKeyKey_J)
            {
                light->setPosition(Vector3D(light->getPosition().x - step, light->getPosition().y, light->getPosition().z));
            }

            if (event->_key == EKeyCode::eKeyKey_L)
            {
                light->setPosition(Vector3D(light->getPosition().x + step, light->getPosition().y, light->getPosition().z));
            }
        }

        ///
        CNode* node = getSceneManager()->getObjectByName("cube");
        if (!node)
        {
            return;
        }

        if (event->_key == EKeyCode::eKeyUp && event->_event == event::EKeyboardInputEvent::eKeyboardPressDown)
        {
            node->setRotation(Vector3D(node->getRotation().x + angle, node->getRotation().y, node->getRotation().z));
        }
        if (event->_key == EKeyCode::eKeyDown && event->_event == event::EKeyboardInputEvent::eKeyboardPressDown)
        {
            node->setRotation(Vector3D(node->getRotation().x - angle, node->getRotation().y, node->getRotation().z));
        }
        if (event->_key == EKeyCode::eKeyLeft && event->_event == event::EKeyboardInputEvent::eKeyboardPressDown)
        {
            node->setRotation(Vector3D(node->getRotation().x, node->getRotation().y + angle, node->getRotation().z));
        }
        if (event->_key == EKeyCode::eKeyRight && event->_event == event::EKeyboardInputEvent::eKeyboardPressDown)
        {
            node->setRotation(Vector3D(node->getRotation().x, node->getRotation().y - angle, node->getRotation().z));
        }

        getPlatform()->getWindow()->setCaption("x= " + std::to_string(node->getRotation().x) + "; y = " + std::to_string(node->getRotation().y) + "; z = " + std::to_string(node->getRotation().z));
    }
    
   
    
}
