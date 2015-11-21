#ifndef _LIGHT_ANIMATOR_H_
#define _LIGHT_ANIMATOR_H_

#include "Engine.h"

namespace v3d
{
namespace scene
{
    class CLight;
}
namespace utils
{
    class CTimer;
}
}


class LightAnimator
{
public:

    LightAnimator(const v3d::scene::SceneManagerPtr& scene);
    ~LightAnimator();

    void createLight(const v3d::core::Vector3D& pos, v3d::f32 radius, v3d::f32 speed);
    void update(v3d::u32 dt);

    v3d::scene::CLight* getLight() const;

private:

    void  calcAnimatorPoints();

    v3d::scene::CLight*         m_light;
    v3d::scene::SceneManagerPtr m_scene;

    v3d::core::Vector3D         m_position;
    v3d::f32                    m_radius;
    v3d::f32                    m_speed;

    std::vector<v3d::core::Vector3D>  m_points;

    v3d::utils::CTimer*         m_timer;
    v3d::u32                    m_lastTime;
    v3d::u32                    m_lastTime2;

    v3d::s32                    m_index;

};

#endif //_LIGHT_ANIMATOR_H_
