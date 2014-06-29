#ifndef _V3D_SHADER_DEFAULT_UNIFORM_H_
#define _V3D_SHADER_DEFAULT_UNIFORM_H_

#include "common.h"

namespace v3d
{
namespace renderer
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    class CShaderData;

    class CShaderDefaultUniform
    {
    public:

    //////////////////////////////////////////////////////////////////////////////////////////////////////

        enum EUniformData
        {
            eUniformUnknown = -1,

            eTransformProjectionMatrix,
            eTransformModelMatrix,
            eTransformViewMatrix,
            eTransformNormalMatrix,
            eTransformViewPosition,
            eTransformOrthoMatrix,

            eMaterialAmbient,
            eMaterialDiffuse,
            eMaterialSpecular,
            eMaterialEmission,
            eMaterialShininess,

            eLightsCount,

            eLightPosition,
            eLightAmbient,
            eLightDiffuse,
            eLightSpecular,
            eLightDirection,
            eLightAttenuation,
            eLightRadius,

            eUniformsCount,
        };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

        CShaderDefaultUniform();
        virtual                                     ~CShaderDefaultUniform();

        void                                        setUniform(const std::string& attribute, EUniformData data);

        CShaderDefaultUniform::EUniformData         getUniformType() const;

        static const std::string&                   getNameByValue(CShaderDefaultUniform::EUniformData type);
        static const CShaderDefaultUniform::EUniformData getValueByName(const std::string& name);

    private:

        friend                                      CShaderData;

        EUniformData                                m_uniformType;
        std::string                                 m_attribute;

        static const std::string                    s_uniformName[eUniformsCount];
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CShaderDefaultUniform>    DefaultUniformPtr;
    typedef std::map<std::string, DefaultUniformPtr>  DefaultUniformList;

    //////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_SHADER_DEFAULT_UNIFORM_H_
