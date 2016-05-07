#include "MyApplication.h"
#include "Valera3D.h"

#include "stream/StreamManager.h"
#include "TextureSwitcher.h"
#include "LightAnimator.h"

using namespace v3d;
using namespace core;
using namespace scene;
using namespace stream;
using namespace event;

MyApplication::MyApplication(int& argc, char** argv)
    : BaseApplication(argc, argv)
    , m_textureSwitcher(nullptr)
    , m_lightAnimator(nullptr)
{
    BaseApplication::getPlatform()->createWindowWithContext(Dimension2D(1024, 768));

    m_textureSwitcher = new TextureSwitcher(BaseApplication::getSceneManager());
    m_lightAnimator = new LightAnimator(BaseApplication::getSceneManager());
}

MyApplication::~MyApplication()
{
    delete m_textureSwitcher;
    m_textureSwitcher = nullptr;

    delete m_lightAnimator;
    m_lightAnimator = nullptr;
}

void MyApplication::init()
{
    if (m_textureSwitcher)
    {
        m_textureSwitcher->loadModel();
        BaseApplication::getInputEventHandler()->connectKeyboardEvent(std::bind(&TextureSwitcher::onKeyboard, m_textureSwitcher, std::placeholders::_1));
    }

    if (m_lightAnimator)
    {
        m_lightAnimator->createLight(Vector3D(0, 0, -5), 1.0f, 5.0f);
        BaseApplication::getInputEventHandler()->connectKeyboardEvent(std::bind(&LightAnimator::onKeyboard, m_lightAnimator, std::placeholders::_1));
    }

    CSkybox* skybox = BaseApplication::getSceneManager()->addSkyBox(
    "textures/skybox/jajlands_ft.jpg", "textures/skybox/jajlands_rt.jpg", "textures/skybox/jajlands_lf.jpg",
    "textures/skybox/jajlands_bk.jpg", "textures/skybox/jajlands_up.jpg", "textures/skybox/jajlands_dn.jpg");
    skybox->setRenderTechnique("shaders/skybox.xml");


    m_fpsCamera = BaseApplication::getSceneManager()->addFPSCamera(0, Vector3D(0.0f), Vector3D(0.7f, 0.0f, 0.7f));
    m_fpsCamera->setName("fpsCamera");

    m_camera = BaseApplication::getSceneManager()->addCamera(0, Vector3D(0.0f, 0.0f, 1.0f), Vector3D(0.0f, 0.0f, -1.0f));
    m_camera->setName("camera");

}

void MyApplication::run()
{
    if (m_lightAnimator)
    {
        m_lightAnimator->update(BaseApplication::getSceneManager()->getDeltaTime());
    }

    BaseApplication::getWindow()->setCaption("Test. FPS: " + std::to_string(BaseApplication::getSceneManager()->getFPS()));

    if (BaseApplication::getInputEventHandler()->isKeyPressed(EKeyCode::eKeyEscape))
    {
        BaseApplication::getPlatform()->closeWindow();
        return;
    }

    static bool pressed = true;
    if (BaseApplication::getInputEventHandler()->isKeyPressed(EKeyCode::eKeyKey_V) && !pressed)
    {
        pressed = true;

        if (!m_fpsCamera || !m_camera)
        {
            return;
        }

        if (getSceneManager()->isActiveCamera(m_fpsCamera))
        {
            m_camera->setPosition(m_fpsCamera->getPosition());
            m_camera->setTarget(m_fpsCamera->getTarget());
            getSceneManager()->setActiveCamera(m_camera);
        }
        else if (getSceneManager()->isActiveCamera(m_camera))
        {
            m_fpsCamera->setPosition(m_camera->getPosition());
            m_fpsCamera->setTarget(m_camera->getTarget());
            getSceneManager()->setActiveCamera(m_fpsCamera);
        }
    }

    if (!BaseApplication::getInputEventHandler()->isKeyPressed(EKeyCode::eKeyKey_V))
    {
        pressed = false;
    }
}
