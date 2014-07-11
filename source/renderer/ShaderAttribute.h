#ifndef _V3D_SHADER_ATTRIBUTE_H_
#define _V3D_SHADER_ATTRIBUTE_H_

#include "common.h"

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
            eAttributeVertex,
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
        virtual                         ~CShaderAttribute();

        void                            setAttribute(EShaderAttribute type, const std::string& attribute);

        const std::string&              getAttributeName() const;
        EShaderAttribute                getAttributeType() const;

        static const std::string&       getAttributeNameByType(EShaderAttribute type);
        static const EShaderAttribute   getAttributeTypeByName(const std::string& name);

    private:

        EShaderAttribute                m_typeAttr;
        std::string                     m_attribute;

        static const std::string        s_attributeName[EShaderAttribute::eAttributeCount];
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CShaderAttribute>       AttributePtr;
    typedef std::map<std::string, AttributePtr>     AttributeList;

    //////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_SHADER_ATTRIBUTE_H_
