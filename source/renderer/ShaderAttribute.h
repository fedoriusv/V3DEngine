#ifndef _V3D_SHADER_ATTRIBUTE_H_
#define _V3D_SHADER_ATTRIBUTE_H_

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

    class CShaderData;

    class CShaderAttribute
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

            eAttribCount,
        };

        CShaderAttribute();
        ~CShaderAttribute();

        void                            setID(s32 id);

        CShaderAttribute&               operator=(const CShaderAttribute& other);

        const std::string&              getName()           const;
        u32                             getDivisor()        const;
        EDataType                       getType()           const;
        EShaderAttribute                getData()           const;
        s32                             getID()             const;

        void*                           getUserData()       const;
        u32                             getUserDataSize()   const;
        u32                             getUserDataCount()  const;

        static const std::string&       getNameByValue(EShaderAttribute type);
        static const EShaderAttribute   getValueByName(const std::string& name);

        bool                            parse(const tinyxml2::XMLElement* root);

    private:

        friend                          CShaderData;

        void                            setAttribute(const std::string& name, EShaderAttribute data);
        void                            setAttribute(EDataType type, const std::string& name, u32 divisor, u32 size, u32 count, const void* data);

        EDataType                       m_type;
        EShaderAttribute                m_data;
        std::string                     m_name;
        u32                             m_divisor;
        s32                             m_id;

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

        static const std::string        s_attributeName[eAttribCount];
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CShaderAttribute>       AttributePtr;
    typedef std::pair<std::string, AttributePtr>    AttributePair;
    typedef std::map<std::string, AttributePtr>     AttributeList;

    //////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_SHADER_ATTRIBUTE_H_
