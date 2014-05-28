#ifndef _F3D_SHADER_ATTRIBUTE_H_
#define _F3D_SHADER_ATTRIBUTE_H_

#include "Object.h"

namespace f3d
{
namespace renderer
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

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

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    class CShaderAttribute : public CObject
    {
    public:

        CShaderAttribute();
        virtual                         ~CShaderAttribute();

        void                            setAttribute(EShaderAttribute type, const std::string& attribute);

        const std::string&              getAttributeName() const;
        EShaderAttribute                getAttributeType() const;

        static const std::string&       getShaderAttributeNameByType(EShaderAttribute type);
        static const EShaderAttribute   getShaderAttributeTypeByName(const std::string& name);

    private:

        EShaderAttribute                m_typeAttr;
        std::string                     m_attribute;

        static const std::string        s_shaderAttributeName[EShaderAttribute::eAttributeCount];
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CShaderAttribute>       AttributePtr;
    typedef std::map<std::string, AttributePtr>     AttributeList;

    //////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_F3D_SHADER_ATTRIBUTE_H_