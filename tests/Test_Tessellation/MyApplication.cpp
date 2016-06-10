#include "MyApplication.h"
#include "Valera3D.h"

using namespace v3d;
using namespace core;
using namespace scene;
using namespace stream;

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
    const SceneManagerPtr& scene = BaseApplication::getSceneManager();

    CRectangleShape* screenLeft = BaseApplication::getSceneManager()->addRectangle(0, Rect32(0, 0, 512, 768));
    screenLeft->setName("screenLeft");
    screenLeft->setRenderTechnique("shaders/screen2DLeft.xml");

    CRectangleShape* screenRight = BaseApplication::getSceneManager()->addRectangle(0, Rect32(512, 0, 1024, 768));
    screenRight->setName("screenRight");
    screenRight->setRenderTechnique("shaders/screen2DRight.xml");

    CShape* sample0 = scene->addSample(0, Vector3D(0, 0, -3));
    sample0->setName("sample0");
    sample0->setRenderTechnique("shaders/simple_target_tessilation.xml");
    //sample0->getRenderTechique()->getRenderPass(0)->addRenderTarget("screen2DRightTarget");
    //sample0->getRenderTechique()->getRenderPass(0)->removeRenderTarget("screen2DRightTarget");

    //CShape* sample1 = scene->addSample(0, Vector3D(0, 0, -3));
    //sample1->setName("sample1");
    //sample1->setRenderTechnique(sample0->getRenderTechique()->clone());
    ////sample1->setRenderTechnique("shaders/simple_target.xml");
    //sample1->getRenderTechique()->getRenderPass(0)->getShaderProgram()->setUndefine("STATIC_LEVEL");

    CCamera* fpsCamera = BaseApplication::getSceneManager()->addFPSCamera(0, Vector3D(0, 0, 0), Vector3D(0.7f, 0, 0.7f));
    fpsCamera->setName("fpsCamera");
    CCamera* camera = scene->addCamera(0, Vector3D(0, 0, 0), Vector3D(0.0f, 0, -5.0f));
    camera->setName("camera");

    BaseApplication::getSceneManager()->setActiveCamera(fpsCamera);

    BaseApplication::getInputEventHandler()->connectKeyboardEvent(std::bind(&MyApplication::onKeyboard, this, std::placeholders::_1));
    BaseApplication::getInputEventHandler()->connectMouseEvent(std::bind(&MyApplication::onMouse, this, std::placeholders::_1));
}

void MyApplication::run()
{
    BaseApplication::getWindow()->setCaption("Test. FPS: " + std::to_string(BaseApplication::getSceneManager()->getFPS()));
}

void MyApplication::onMouse(const event::MouseInputEventPtr& event)
{
}

void MyApplication::onKeyboard(const event::KeyboardInputEventPtr& event)
{
    if (event->_event == event::EKeyboardInputEvent::eKeyboardPressDown)
    {
        if (event->_key == EKeyCode::eKeyEscape)
        {
            getPlatform()->closeWindow();
        }

        if (event->_key == EKeyCode::eKeyKey_V)
        {
            CCamera* fpsCamera = static_cast<CCamera*>(BaseApplication::getSceneManager()->getObjectByName("fpsCamera"));
            CCamera* camera = static_cast<CCamera*>(BaseApplication::getSceneManager()->getObjectByName("camera"));
            if (!fpsCamera || !camera)
            {
                return;
            }

            if (BaseApplication::getSceneManager()->isActiveCamera(fpsCamera))
            {
                camera->setPosition(fpsCamera->getPosition());
                camera->setTarget(fpsCamera->getTarget());
                BaseApplication::getSceneManager()->setActiveCamera(camera);
            }
            else if (BaseApplication::getSceneManager()->isActiveCamera(camera))
            {
                fpsCamera->setPosition(camera->getPosition());
                fpsCamera->setTarget(camera->getTarget());
                BaseApplication::getSceneManager()->setActiveCamera(fpsCamera);
            }
        }
    }

}
