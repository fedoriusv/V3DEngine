#include "MyApplication.h"
#include "Valera3D.h"

using namespace v3d;
using namespace core;
using namespace scene;
using namespace stream;
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
    MyApplication::switchModel(2);
    BaseApplication::getInputEventHandler()->connectKeyboardEvent(std::bind(&MyApplication::onKeyboard, this, std::placeholders::_1));
    BaseApplication::getInputEventHandler()->connectMouseEvent(std::bind(&MyApplication::onMouse, this, std::placeholders::_1));
    BaseApplication::getInputEventHandler()->connectGamepadEvent(std::bind(&MyApplication::onGamepad, this, std::placeholders::_1));
}

void MyApplication::run()
{
    BaseApplication::getWindow()->setCaption("Test. FPS: " + std::to_string(BaseApplication::getSceneManager()->getFPS()));
}

void MyApplication::onMouse(const event::MouseInputEventPtr& event)
{
}

void MyApplication::onGamepad(const event::GamepadInputEventPtr& event)
{
}

void MyApplication::useSimpleArray()
{
    const SceneManagerPtr& scene = BaseApplication::getSceneManager();
    scene->clear();

    for (s32 col = -2; col < 3; ++col)
    {
        for (s32 row = -2; row < 3; ++row)
        {
            for (s32 width = 0; width < 1; ++width)
            {
                CShape* model = scene->addCube(0, Vector3D((f32)row, (f32)col, -5));
                if (model)
                {
                    model->setName("cube_" + std::to_string(col) + "_" + std::to_string(row));
                    model->setScale(Vector3D(0.5f));
                    model->getMaterial()->setRenderTechnique("shaders/default.xml");
                    model->getMaterial()->setTexture(0, "textures/box.jpg");
                }
            }
        }
    }

    CNode* fpsCamera = scene->addFPSCamera(0, Vector3D(0, 0, 0), Vector3D(0.7f, 0, 0.7f));
    fpsCamera->setName("fpsCamera");
    CNode* camera = scene->addCamera(0, Vector3D(0, 0, 0), Vector3D(0.0f, 0, -5.0f));
    camera->setName("camera");

    scene->init();
}

void MyApplication::useInstancedObject()
{
    const SceneManagerPtr& scene = BaseApplication::getSceneManager();
    scene->clear();

    CShape* model = scene->addCube(0, Vector3D(0, 0, -5));
    if (model)
    {
        model->setName("cube");
        model->setScale(Vector3D(0.5f));
        model->getMaterial()->setRenderTechnique("shaders/default_instancing.xml");
        model->getMaterial()->setTexture(0, "textures/box.jpg");
        const RenderPassPtr& pass = model->getMaterial()->getRenderTechique()->getRenderPass(0);
        u32 index = 0;
        for (s32 col = -2; col < 3; ++col)
        {
            for (s32 row = -2; row < 3; ++row)
            {
                for (s32 width = 0; width < 1; ++width)
                {
                    pass->getUserShaderData()->setUniform("offsets[" + std::to_string(index) + "]", Vector3D((f32)col, (f32)row, f32(- 5 - width)));
                    ++index;
                }
            }
        }
        
    }

    CNode* fpsCamera = scene->addFPSCamera(0, Vector3D(0, 0, 0), Vector3D(0.7f, 0, 0.7f));
    fpsCamera->setName("fpsCamera");
    CNode* camera = scene->addCamera(0, Vector3D(0, 0, 0), Vector3D(0.0f, 0, -5.0f));
    camera->setName("camera");

    scene->init();
}

void MyApplication::useInstancedAttrObject()
{
    const SceneManagerPtr& scene = BaseApplication::getSceneManager();
    scene->clear();

    CShape* model = scene->addCube(0, Vector3D(0, 0, 0));
    if (model)
    {
        model->setName("cube");
        model->setScale(Vector3D(0.5f));
        model->getMaterial()->setRenderTechnique("shaders/default_instancing_attr.xml");
        model->getMaterial()->setTexture(0, "textures/box.jpg");
        const RenderPassPtr& pass = model->getMaterial()->getRenderTechique()->getRenderPass(0);
        
        std::vector<Vector3D> offset;
        for (s32 col = -2; col < 3; ++col)
        {
            for (s32 row = -2; row < 3; ++row)
            {
                for (s32 width = 0; width < 1; ++width)
                {
                    offset.push_back(Vector3D((f32)col, (f32)row, f32(-5 - width)));
                }
            }
        }

        pass->getUserShaderData()->setAttribute("offsets", 1, offset);
    }

    CNode* fpsCamera = scene->addFPSCamera(0, Vector3D(0, 0, 0), Vector3D(0.7f, 0, 0.7f));
    fpsCamera->setName("fpsCamera");
    CNode* camera = scene->addCamera(0, Vector3D(0, 0, 0), Vector3D(0.0f, 0, -5.0f));
    camera->setName("camera");

    scene->init();
}

void MyApplication::switchModel(u32 value)
{
    static u32 currValue = -1;
    if (currValue == value)
    {
        return;
    }

    switch(value)
    {
    case 0:
        MyApplication::useSimpleArray();
        currValue = value;
        return;

    case 1:
        MyApplication::useInstancedObject();
        currValue = value;
        return;

    case 2:
        MyApplication::useInstancedAttrObject();
        currValue = value;
        return;

    default:
        return;

    }
}

void MyApplication::onKeyboard(const event::KeyboardInputEventPtr& event)
{
    f32 step = 5.0f;
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

        if (event->_key == EKeyCode::eKeyKey_1)
        {
            MyApplication::switchModel(0);
        }
        else if (event->_key == EKeyCode::eKeyKey_2)
        {
            MyApplication::switchModel(1);
        }
        else if (event->_key == EKeyCode::eKeyKey_3)
        {
            MyApplication::switchModel(2);
        }
    }
}
