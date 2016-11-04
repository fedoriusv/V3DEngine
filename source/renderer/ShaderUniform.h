#pragma once

#include "common.h"
#include "DataTypes.h"

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
    class ConstantBuffer;

    /**
    * Shader Uniform.
    * Client thread
    */
    class ShaderUniform final
    {
    public:

        enum EUniformData
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

        explicit ShaderUniform(ConstantBuffer* buffer);
        ShaderUniform(const ShaderUniform& uniform);
        ShaderUniform& operator=(const ShaderUniform& uniform);
        ~ShaderUniform();

        const std::string&          getName() const;

        EDataType                   getDataType() const;
        EUniformData                getData()       const;

        bool                        parse(const tinyxml2::XMLElement* root);

        static const std::string&   getUniformNameByValue(EUniformData type);
        static EUniformData         getValueByUniformName(const std::string& name);

    private:

        friend                      ShaderData;

        void                        setUniform(const std::string& name, EUniformData data);
        void                        setUniform(const std::string& name, EDataType type, ,);

        bool                        parseUserUniform(const tinyxml2::XMLElement* root, const std::string& name, EDataType type);
        void                        parseArrayValue(const std::string& val, f32* array, u32 count);

        std::string                 m_name;

        EDataType                   m_type;
        EUniformData                m_data;

        s32                         m_set;
        s32                         m_binding;

        u32                         m_offset;
        u32                         m_size;
        u32                         m_count;

        ConstantBuffer*             m_buffer;

        static const std::string    s_uniformName[EUniformData::eUniformsCount];

    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    //typedef std::pair<const std::string, ShaderUniform*>   UniformPair;
    //typedef std::map<const std::string, ShaderUniform*>    UniformList;

    //////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d
