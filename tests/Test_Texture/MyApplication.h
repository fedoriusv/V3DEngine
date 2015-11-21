#ifndef _MY_APPLICATION_H_
#define _MY_APPLICATION_H_

#include "BaseApplication.h"

class TextureSwitcher;
class LightAnimator;

namespace v3d
{
namespace scene
{
    CCamera;
}
}

class MyApplication : public v3d::BaseApplication
{
public:

    MyApplication(int& argc, char** argv);
    virtual             ~MyApplication();

    void                init() override;
    void                run()  override;

private:

    TextureSwitcher*        m_textureSwitcher;
    LightAnimator*          m_lightAnimator;

    v3d::scene::CCamera*    m_camera;
    v3d::scene::CCamera*    m_fpsCamera;

};

#endif //_MY_APPLICATION_H_