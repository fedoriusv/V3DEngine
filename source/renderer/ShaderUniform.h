#ifndef _V3D_SHADER_UNIFORM_H_
#define _V3D_SHADER_UNIFORM_H_

#include "common.h"

namespace v3d
{
namespace renderer
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EUniformData
    {
        eUniformUser = -1,

        eTransformProjectionMatrix,
        eTransformModelMatrix,
        eTransformViewMatrix,
        eTransformNormalMatrix,
        eTransformViewPosition,
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

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    class CShaderData;

    class CShaderUniform
    {
    public:

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

            eTypeCount,
        };

        CShaderUniform();
        ~CShaderUniform();

        void                        setUniform(EDataType type, const std::string& attribute, void* value);
        void                        setUniforID(s32 id);

        EDataType                   getUniformType() const;
        void*                       getUniforValue() const;
        const std::string&          getUniformName() const;

        EUniformData                getUniformData() const;
        s32                         getUniformID(const s32 index = -1)   const;


        static const std::string&   getNameByValue(EUniformData type);
        static const EUniformData   getValueByName(const std::string& name);

    private:

        friend                      CShaderData;

        void                        setUniform(const std::string& attribute, EUniformData data);

        EDataType                   m_uniformType;
        void*                       m_uniformValue;
        std::string                 m_attribute;
        EUniformData                m_uniformData;
        s32                         m_id;

        void                        allocMemory(EDataType type, void* value);
        void                        deallocMemory();

        static const std::string    s_uniformName[eUniformsCount];

    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CShaderUniform>         UniformPtr;
    typedef std::map<std::string, UniformPtr>       UniformList;

    //////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_SHADER_UNIFORM_H_
