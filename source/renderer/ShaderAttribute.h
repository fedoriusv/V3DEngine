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

    /**
    * Shader Attribute(Channel).
    * Client thread
    */
    class ShaderAttribute final
    {
    public:

        enum EShaderAttribute : s16
        {
            eAttribUser = -1,

            eAttribVertexPosition,
            eAttribVertexColor,
            eAttribVertexNormal,
            eAttribVertexBinormal,
            eAttribVertexTangent,
            eAttribVertexTexture0,
            eAttribVertexTexture1,
            eAttribVertexTexture2,
            eAttribVertexTexture3,

            eAttribParticalPosition,
            eAttribParticalColor,
            eAttribParticalVelocity,
            eAttribParticalLifeTime,
            eAttribParticalSize,
            eAttribParticalType,

            eAttribColorAttachment,

            eAttribCount,
        };

        ShaderAttribute();
        ShaderAttribute(const ShaderAttribute& attribute) = delete;
        ShaderAttribute& operator=(const ShaderAttribute& attribute) = delete;
        ~ShaderAttribute();

        const std::string&              getName() const;


        EShaderAttribute                getChannel() const;
        ShaderDataType::EDataType       getDataType() const;

        bool                            parse(const tinyxml2::XMLElement* root);

        static const std::string&       getAttributeNameByValue(EShaderAttribute type);
        static const EShaderAttribute   getValueByAttributeName(const std::string& name);

    private:

        void                            setAttribute(const std::string& name, EShaderAttribute data);
        void                            setAttribute(ShaderDataType::EDataType type, const std::string& name, u32 size, u32 count, const void* data);

        void*                           getUserData()       const;
        u32                             getUserDataSize()   const;
        u32                             getUserDataCount()  const;

        struct SUserData
        {
            SUserData();
            SUserData(u32 size, u32 count, const void* data);
            ~SUserData();

            SUserData& operator=(const SUserData&);

            u32                         size;
            u32                         count;
            void*                       data;

            void                        copy(u32 size, u32 count, const void* data);
            void                        free();
        };

        std::string                     m_name;
        SUserData*                      m_userData;

        ShaderDataType::EDataType       m_dataType;
        EShaderAttribute                m_channel;

        static const std::string        s_attributeName[EShaderAttribute::eAttribCount];
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    using AttributeList = std::vector<ShaderAttribute*>;

    //////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d
