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
            eAttributeUser = -1,

            eAttributePosition,
            eAttributeColor,
            eAttributeNormal,
            eAttributeBinormal,
            eAttributeTangent,
            eAttributeTexture0,
            eAttributeTexture1,
            eAttributeTexture2,
            eAttributeTexture3,

            eAttributeCount,
        };

        CShaderAttribute();
        ~CShaderAttribute();

        void                            setID(s32 id);

        CShaderAttribute&               operator=(const CShaderAttribute& other);

        const std::string&              getName()           const;
        u32                             getDivisor()        const;
        EShaderAttribute                getType()           const;
        u32                             getID()             const;

        void*                           getUserData()       const;
        u32                             getUserDataSize()   const;
        u32                             getUserDataCount()  const;

        static const std::string&       getNameByValue(EShaderAttribute type);
        static const EShaderAttribute   getValueByName(const std::string& name);

        bool                            parse(const tinyxml2::XMLElement* root);

    private:

        friend                          CShaderData;

        void                            setAttribute(const std::string& name, EShaderAttribute type);
        void                            setAttribute(const std::string& name, u32 divisor, u32 size, u32 count, const void* data);

        EShaderAttribute                m_type;
        std::string                     m_name;
        u32                             m_divisor;
        u32                             m_id;

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

        static const std::string        s_attributeName[eAttributeCount];
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CShaderAttribute>       AttributePtr;
    typedef std::pair<std::string, AttributePtr>    AttributePair;
    typedef std::map<std::string, AttributePtr>     AttributeList;

    //////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_SHADER_ATTRIBUTE_H_
