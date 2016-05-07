#ifndef _V3D_PARTICLE_SYSTEM_H_
#define _V3D_PARTICLE_SYSTEM_H_

#include "Node.h"
#include "renderer/Renderable.h"

namespace v3d
{
namespace scene
{
    class CParticleSystem : public CNode, public renderer::Renderable
    {
    public:

        CParticleSystem(const std::string& texture);
        ~CParticleSystem();

        void                    init()         override;
        void                    update(s32 dt) override;
        void                    render()       override;

        void                    setVelocityRange(const core::Vector3D& min, const core::Vector3D& max);
        void                    setLifeRange(f32 min, const f32 max);
        void                    setAmountOfParticles(u32 amount);

        struct SParticle
        {
            core::Vector3D _position;
            core::Vector3D _velocity;
            core::Vector4D _color;
            
            f32             _lifeTime;
            f32             _size;
            f32             _angle;
            f32             _weight;

            u32             _type;
        };

    private:

        void                    build();
        void                    refresh();

        core::Vector3D          m_velocityMin;
        core::Vector3D          m_velocityMax;

        core::Vector3D          m_gravity;

        f32                     m_lifeMin;
        f32                     m_lifeMax;

        f32                     m_size;
        u32                     m_amount;
    };
}
}

#endif //_V3D_PARTICLE_SYSTEM_H_