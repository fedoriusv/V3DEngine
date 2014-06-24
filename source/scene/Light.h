#ifndef _V3D_LIGHT_H_
#define _V3D_LIGHT_H_

#include "Node.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    struct SLightData
    {
        Vector4D    _ambient;
        Vector4D    _diffuse;
        Vector4D    _specular;
        Vector3D    _direction;
        Vector3D    _attenuation;
        f32         _radius;

    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CLight : public CNode
    {
    public:

        CLight();
        virtual     ~CLight();

        void                render()         override;
        void                update(f64 time) override;
        void                init()           override;

        void                setAmbient(const Vector4D& color);
        void                setDiffuse(const Vector4D& color);
        void                setSpecular(const Vector4D& color);
        void                setRadius(const f32 radius);
        void                setAttenuation(const Vector3D& attenuation);
        void                setDirection(const Vector3D& direction);

        const Vector4D&     getAmbient()        const;
        const Vector4D&     getDiffuse()        const;
        const Vector4D&     getSpecular()       const;
        f32                 getRadius()         const;
        const Vector3D&     getAttenuation()    const;
        const Vector3D&     getDirection()      const;

    private:

        SLightData  m_data;

    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_LIGHT_H_