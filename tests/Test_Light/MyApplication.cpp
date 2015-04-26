#include "MyApplication.h"
#include "Valera3D.h"

#include "stream/StreamManager.h"

using namespace v3d;
using namespace core;
using namespace scene;
using namespace stream;
using namespace event;
using namespace renderer;

MyApplication::MyApplication(int& argc, char** argv)
    : BaseApplication(argc, argv)
{
    BaseApplication::getPlatform()->createWindowWithContext(Dimension2D(1024, 768));
}

MyApplication::~MyApplication()
{
}

void MyApplication::init()
{
    CShape* cube = BaseApplication::getSceneManager()->addCube(0, Vector3D(0, 1, -5));
    cube->setName("cube");
    cube->getMaterial()->setRenderTechnique("shaders/light.xml");
    cube->getMaterial()->getRenderTechique()->getRenderPass(0)->getRenderState()->setPolygonMode(ePolyModeFill);

    CLight* light = BaseApplication::getSceneManager()->addLight(0, Vector3D(6, 3, 5));
    //->setName("light");

    CNode* fpsCamera = BaseApplication::getSceneManager()->addFPSCamera(0, Vector3D(0, 0, 0), Vector3D(0.7f, 0, 0.7f));
    fpsCamera->setName("fpsCamera");
    CNode* camera = BaseApplication::getSceneManager()->addCamera(0, Vector3D(0, 0, 0), Vector3D(0.0f, 0, -1.0f));
    camera->setName("camera");
    
    BaseApplication::getInputEventHandler()->connectKeyboardEvent(std::bind(&MyApplication::onKeyboard, this, std::placeholders::_1));
    BaseApplication::getInputEventHandler()->connectMouseEvent(std::bind(&MyApplication::onMouse, this, std::placeholders::_1));
}

void MyApplication::run()
{
    //TODO: main loop
}

void MyApplication::onMouse(const event::MouseInputEventPtr& event)
{
}

void MyApplication::onKeyboard(const event::KeyboardInputEventPtr& event)
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

        ///
        CNode* node = getSceneManager()->getObjectByName("cube");
        if (!node)
        {
            return;
        }

        if (event->_key == EKeyCode::eKeyUp && event->_event == EKeyboardInputEvent::eKeyboardPressDown)
        {
            node->setRotation(Vector3D(node->getRotation().x + angle, node->getRotation().y, node->getRotation().z));
        }
        if (event->_key == EKeyCode::eKeyDown && event->_event == EKeyboardInputEvent::eKeyboardPressDown)
        {
            node->setRotation(Vector3D(node->getRotation().x - angle, node->getRotation().y, node->getRotation().z));
        }
        if (event->_key == EKeyCode::eKeyLeft && event->_event == EKeyboardInputEvent::eKeyboardPressDown)
        {
            node->setRotation(Vector3D(node->getRotation().x, node->getRotation().y + angle, node->getRotation().z));
        }
        if (event->_key == EKeyCode::eKeyRight && event->_event == EKeyboardInputEvent::eKeyboardPressDown)
        {
            node->setRotation(Vector3D(node->getRotation().x, node->getRotation().y - angle, node->getRotation().z));
        }

        getPlatform()->getWindow()->setCaption("x= " + std::to_string(node->getRotation().x) + "; y = " + std::to_string(node->getRotation().y) + "; z = " + std::to_string(node->getRotation().z));
    }
}
