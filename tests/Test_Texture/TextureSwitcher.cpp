#include "TextureSwitcher.h"
#include "Valera3D.h"

using namespace v3d;
using namespace core;
using namespace scene;
using namespace event;

TextureSwitcher::TextureSwitcher(const v3d::scene::SceneManagerPtr& scene)
    : m_object(nullptr)
    , m_scene(scene)
{
}

TextureSwitcher::~TextureSwitcher()
{
    m_scene->dropNode(m_object);

    m_scene = nullptr;
    m_object = nullptr;
}

void TextureSwitcher::loadModel()
{
    CShape* cube = m_scene->addCube(0, Vector3D(0, 0, -5));
    cube->setName("cube");

    m_object = cube;
    TextureSwitcher::switchTextureMode(3);
}

void TextureSwitcher::switchTextureMode(u32 mode)
{
    static u32 currMode = -1;

    if (currMode == mode)
    {
        return;
    }

    if (!m_object)
    {
        return;
    }

    switch (mode)
    {
    case 0: //default
        m_object->setRenderTechnique("shaders/texture_without_light.xml");
        m_object->getMaterial()->setTexture(0, "textures/bricks2.jpg");
        break;

    case 1: //with light
        m_object->setRenderTechnique("shaders/texture.xml");
        m_object->getMaterial()->setTexture(0, "textures/bricks2.jpg");
        break;

    case 2: //normal map
        m_object->setRenderTechnique("shaders/texture_normalmap.xml");
        m_object->getMaterial()->setTexture(0, "textures/bricks2.jpg");
        m_object->getMaterial()->setTexture(1, "textures/bricks2_normal.jpg");
        break;

    case 3: //parallax map
        m_object->setRenderTechnique("shaders/texture_parallaxmap.xml");
        m_object->getMaterial()->setTexture(0, "textures/bricks2.jpg");
        m_object->getMaterial()->setTexture(1, "textures/bricks2_normal.jpg");
        m_object->getMaterial()->setTexture(2, "textures/bricks2_disp.jpg");
        break;

    default:
        break;
    }

    currMode = mode;
}

void TextureSwitcher::onKeyboard(const v3d::event::KeyboardInputEventPtr& event)
{
    if (!m_object)
    {
        return;
    }

    f32 angle = 10.0f;
    if (event->_key == EKeyCode::eKeyUp && event->_event == EKeyboardInputEvent::eKeyboardPressDown)
    {
        m_object->setRotation(Vector3D(m_object->getRotation().x + angle, m_object->getRotation().y, m_object->getRotation().z));
    }
    if (event->_key == EKeyCode::eKeyDown && event->_event == EKeyboardInputEvent::eKeyboardPressDown)
    {
        m_object->setRotation(Vector3D(m_object->getRotation().x - angle, m_object->getRotation().y, m_object->getRotation().z));
    }
    if (event->_key == EKeyCode::eKeyLeft && event->_event == EKeyboardInputEvent::eKeyboardPressDown)
    {
        m_object->setRotation(Vector3D(m_object->getRotation().x, m_object->getRotation().y + angle, m_object->getRotation().z));
    }
    if (event->_key == EKeyCode::eKeyRight && event->_event == EKeyboardInputEvent::eKeyboardPressDown)
    {
        m_object->setRotation(Vector3D(m_object->getRotation().x, m_object->getRotation().y - angle, m_object->getRotation().z));
    }

    if (event->_key == EKeyCode::eKeyKey_1 && event->_event == EKeyboardInputEvent::eKeyboardPressDown)
    {
        TextureSwitcher::switchTextureMode(0);
    }
    else if (event->_key == EKeyCode::eKeyKey_2 && event->_event == EKeyboardInputEvent::eKeyboardPressDown)
    {
        TextureSwitcher::switchTextureMode(1);
    }
    else if (event->_key == EKeyCode::eKeyKey_3 && event->_event == EKeyboardInputEvent::eKeyboardPressDown)
    {
        TextureSwitcher::switchTextureMode(2);
    }
    else if (event->_key == EKeyCode::eKeyKey_4 && event->_event == EKeyboardInputEvent::eKeyboardPressDown)
    {
        TextureSwitcher::switchTextureMode(3);
    }
}
