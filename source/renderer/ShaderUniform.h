#ifndef _V3D_SHADER_UNIFORM_H_
#define _V3D_SHADER_UNIFORM_H_

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

    enum EDataType
    {
        eTypeNone = 0,
        eTypeInt,
        eTypeFloat,
        eTypeVector2,
        eTypeVector3,
        eTypeVector4,
        eTypeMatrix3,
        eTypeMatrix4,

        eDataTypeCount
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    class CShaderData;

    class CShaderUniform
    {
    public:

        enum EUniformData
        {
            eUserUniform = -1,

            eTransformProjectionMatrix,
            eTransformModelMatrix,
            eTransformViewMatrix,
            eTransformNormalMatrix,
            eTransformViewPosition,
            eTransformViewUpVector,
            eTransformOrthoMatrix,

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

            eUniformsCount,
        };

        CShaderUniform();
        ~CShaderUniform();

        void                        setID(s32 id);

        const std::string&          getAttribute()  const;
        EDataType                   getType()       const;
        void*                       getValue()      const;
        EUniformData                getData()       const;
        s32                         getID()         const;

        bool                        parse(const tinyxml2::XMLElement* root);


        static const std::string&   getNameByValue(EUniformData type);
        static EUniformData         getValueByName(const std::string& name);

    private:

        friend                      CShaderData;

        void                        setUniform(const std::string& attribute, EUniformData data);
        void                        setUniform(EDataType type, const std::string& attribute, void* value);

        bool                        parseUserUniform(const tinyxml2::XMLElement* root, const std::string& name, EDataType type);
        void                        parseArrayValue(const std::string& val, f32* array, u32 count);

        EDataType                   m_type;
        void*                       m_value;
        std::string                 m_attribute;
        EUniformData                m_data;

        s32                         m_id;

        void                        allocMemory(EDataType type, void* value);
        void                        deallocMemory();

        static const std::string    s_uniformName[eUniformsCount];

    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CShaderUniform>         UniformPtr;
    typedef std::pair<std::string, UniformPtr>      UniformPair;
    typedef std::map<std::string, UniformPtr>       UniformList;

    //////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_SHADER_UNIFORM_H_
