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
    * Shader Attribute(Channel).
    * Client thread
    */
    class ShaderAttribute final
    {
    public:

        enum EShaderAttribute
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
        ShaderAttribute(const ShaderAttribute& attribute);
        ShaderAttribute& operator=(const ShaderAttribute& other);
        ~ShaderAttribute();



        const std::string&              getName() const;


        EShaderAttribute                getChannel() const;
        ShaderDataType::EShaderDataType                       getDataType() const;

        u32                             getLocation() const;

        void*                           getUserData()       const;
        u32                             getUserDataSize()   const;
        u32                             getUserDataCount()  const;

        bool                            parse(const tinyxml2::XMLElement* root);

        static const std::string&       getAttributeNameByValue(EShaderAttribute type);
        static const EShaderAttribute   getValueByAttributeName(const std::string& name);

    private:

        friend                          ShaderData;

        void                            setAttribute(const std::string& name, EShaderAttribute data);
        void                            setAttribute(ShaderDataType::EShaderDataType type, const std::string& name, u32 size, u32 count, const void* data);


        std::string                     m_name;

        ShaderDataType::EShaderDataType                       m_type;
        EShaderAttribute                m_channel;

        s32                             m_location;

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
