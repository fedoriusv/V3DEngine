#ifndef _V3D_LIGHT_H_
#define _V3D_LIGHT_H_

#include "Node.h"
#include "stream/Resource.h"

namespace v3d
{
namespace scene
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    struct SLightData
    {
        core::Vector4D    _ambient;
        core::Vector4D    _diffuse;
        core::Vector4D    _specular;
        core::Vector3D    _direction;
        core::Vector3D    _attenuation;
        f32               _radius;

    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class CLight : public CNode, public stream::IResource
    {
    public:

        CLight();
        virtual                   ~CLight();

        void                      init()         override;
        void                      update(s32 dt) override;

        void                      init(const stream::IStreamPtr stream)  override;
        bool                      load()                                  override;
                                  
        void                      setAmbient(const  core::Vector4D& color);
        void                      setDiffuse(const  core::Vector4D& color);
        void                      setSpecular(const  core::Vector4D& color);
        void                      setRadius(f32 radius);
        void                      setAttenuation(const  core::Vector3D& attenuation);
        void                      setDirection(const  core::Vector3D& direction);
                                  
        const  core::Vector4D&    getAmbient()        const;
        const  core::Vector4D&    getDiffuse()        const;
        const  core::Vector4D&    getSpecular()       const;
        f32                       getRadius()         const;
        const  core::Vector3D&    getAttenuation()    const;
        const  core::Vector3D&    getDirection()      const;

    private:

        SLightData                m_data;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CLight> LightPtr;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_LIGHT_H_
