#ifndef _V3D_SHADER_ATTRIBUTE_H_
#define _V3D_SHADER_ATTRIBUTE_H_

#include "common.h"

namespace tinyxml2
{
    class XMLElement;
}

namespace v3d
{
namespace renderer
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    class CShaderAttribute
    {
    public:

        enum EShaderAttribute
        {
            eAttributeNone = -1,
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

        CShaderAttribute&               operator=(const CShaderAttribute& other);

        void                            setAttribute(const std::string& attribute);
        const std::string&              getAttribute() const;

        void                            setType(EShaderAttribute type);
        EShaderAttribute                getType() const;

        static const std::string&       getAttributeNameByType(EShaderAttribute type);
        static const EShaderAttribute   getAttributeTypeByName(const std::string& name);

        bool                            parse(const tinyxml2::XMLElement* root);

    private:

        EShaderAttribute                m_type;
        std::string                     m_attribute;

        static const std::string        s_attributeName[eAttributeCount];
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CShaderAttribute>       AttributePtr;
    typedef std::vector<AttributePtr>               AttributeList;

    //////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_SHADER_ATTRIBUTE_H_
