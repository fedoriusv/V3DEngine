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

        void*                           getUserData()       const;
        u32                             getUserDataSize()   const;
        u32                             getUserDataCount()  const;

        bool                            parse(const tinyxml2::XMLElement* root);

        static const std::string&       getAttributeNameByValue(EShaderAttribute type);
        static const EShaderAttribute   getValueByAttributeName(const std::string& name);

    private:

        void                            setAttribute(const std::string& name, EShaderAttribute data);
        void                            setAttribute(ShaderDataType::EDataType type, const std::string& name, u32 size, u32 count, const void* data);

        std::string                     m_name;

        ShaderDataType::EDataType       m_type;
        EShaderAttribute                m_channel;

        struct SUserData
        {
            SUserData();
            SUserData(u32 size, u32 count, const void* data);
            ~SUserData();

            SUserData& operator=(const SUserData&);

            u32                         _size;
            u32                         _count;
            void*                       _data;

            void                        copy(u32 size, u32 count, const void* data);
            void                        free();
        };

        SUserData*                      m_userData;

        static const std::string        s_attributeName[EShaderAttribute::eAttribCount];
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::pair<const std::string, ShaderAttribute*> AttributePair;
    typedef std::map<const std::string, ShaderAttribute*>  AttributeList;

    //////////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace renderer
} //namespace v3d
