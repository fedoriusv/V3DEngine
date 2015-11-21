#ifndef _APP_TEXTURE_SWITCHER_H_
#define _APP_TEXTURE_SWITCHER_H_

#include "Engine.h"

namespace v3d
{
namespace scene
{
    class CShape;
}
}

class TextureSwitcher
{
public:

    TextureSwitcher(const v3d::scene::SceneManagerPtr& scene);
    ~TextureSwitcher();

    void loadModel();
    void switchTextureMode(v3d::u32 mode);

    void onKeyboard(const v3d::event::KeyboardInputEventPtr& event);

private:

    v3d::scene::CShape*         m_object;
    v3d::scene::SceneManagerPtr m_scene;
};

#endif //_APP_TEXTURE_SWITCHER_H_
