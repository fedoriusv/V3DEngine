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

    extern const std::string&       getShaderAttributeNameByType(EShaderAttribute type);
    extern const EShaderAttribute   getShaderAttributeTypeByName(const std::string& name);

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    class CShaderAttribute : public CObject
    {
    public:

        CShaderAttribute();
        virtual             ~CShaderAttribute();

        void                addAttribute(EShaderAttribute type, const std::string& attribute);

    private:

        EShaderAttribute    m_type;
        std::string         m_attribute;


    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CShaderAttribute>       AttributePtr;
    typedef std::map<std::string, AttributePtr>     AttributeList;

    //////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_F3D_SHADER_ATTRIBUTE_H_