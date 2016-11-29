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

    class IRenderer;
    class ShaderProgram;

    /**
    * Shader Uniform
    * Client side
    */
    class ShaderUniform final
    {
    public:

        enum ETypeContent : s16
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

        ShaderUniform();
        ShaderUniform(const ShaderUniform& uniform) = delete;
        ShaderUniform& operator=(const ShaderUniform& uniform) = delete;
        ~ShaderUniform();

        const std::string&              getName() const;
        ShaderDataType::EDataType       getDataType() const;
        ETypeContent                    getType() const;

        bool                            parse(const tinyxml2::XMLElement* root);

        static const std::string&       getUniformNameByValue(ETypeContent type);
        static ETypeContent             getValueByUniformName(const std::string& name);

    private:

        friend                          IRenderer;
        friend                          ShaderProgram;

        void                            setUniform(const std::string& name, ETypeContent data);
        void                            setUniform(const std::string& name, ShaderDataType::EDataType type, u32 array, const void* data);

        bool                            parseUserUniform(const tinyxml2::XMLElement* root, const std::string& name, ShaderDataType::EDataType type);

        std::string                     m_name;

        void*                           m_initValue;
        u32                             m_array;

        u32                             m_set;
        u32                             m_binding;

        u32                             m_buffer;
        u32                             m_size;
        u32                             m_offset;

        ShaderDataType::EDataType       m_dataType;
        ETypeContent                    m_uniformContent;

        static const std::string        s_uniformName[ETypeContent::eUniformsCount];
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    using UniformList = std::vector<ShaderUniform*>;

    //////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d
