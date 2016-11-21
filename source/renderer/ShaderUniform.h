#pragma once

#include "common.h"
#include "ShaderDataTypes.h"

namespace tinyxml2
{
    class XMLElement;
}

namespace v3d
{
namespace renderer
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    class ShaderData;

    /**
    * Shader Uniform
    * Client side
    */
    class ShaderUniform final
    {
    public:

        enum EUniformData : s16
        {
            eUserUniform = -1,

            eTransformProjectionMatrix,
            eTransformModelMatrix,
            eTransformViewMatrix,
            eTransformNormalMatrix,
            eTransformViewPosition,
            eTransformViewUpVector,

            eMaterialAmbient,
            eMaterialDiffuse,
            eMaterialSpecular,
            eMaterialEmission,
            eMaterialShininess,
            eMaterialTransparency,

            eLightsCount,
            eLightPosition,
            eLightAmbient,
            eLightDiffuse,
            eLightSpecular,
            eLightDirection,
            eLightAttenuation,
            eLightRadius,

            eCurrentTime,
            eViewportSize,

            eUniformsCount,
        };

        ShaderUniform(const ShaderUniform& uniform);
        ShaderUniform& operator=(const ShaderUniform& uniform);
        ~ShaderUniform();

        const std::string&              getName() const;

        ShaderDataType::EShaderDataType getDataType() const;
        EUniformData                    getKindData() const;

        bool                            parse(const tinyxml2::XMLElement* root);

        static const std::string&       getUniformNameByValue(EUniformData type);
        static EUniformData             getValueByUniformName(const std::string& name);

    private:

        friend                          ShaderData;

        void                            setUniform(const std::string& name, EUniformData data);

        bool                            parseUserUniform(const tinyxml2::XMLElement* root, const std::string& name, ShaderDataType::EShaderDataType type);
        void                            parseArrayValue(const std::string& val, f32* array, u32 count);

        std::string                     m_name;

        ShaderDataType::EShaderDataType m_type;
        EUniformData                    m_data;

        static const std::string        s_uniformName[EUniformData::eUniformsCount];
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::pair<const std::string, ShaderUniform*>   UniformPair;
    typedef std::map<const std::string, ShaderUniform*>    UniformList;

    //////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d
