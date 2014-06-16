#ifndef _V3D_SHADER_UNIFORM_H_
#define _V3D_SHADER_UNIFORM_H_

#include "common.h"

namespace v3d
{
namespace renderer
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EShaderDataType
    {
        eTypeNone = 0,
        eTypeInt,
        eTypeFloat,
        eTypeVector2,
        eTypeVector3,
        eTypeVector4,
        eTypeMatrix3,
        eTypeMatrix4,

        eShaderDataCount,
    };

    enum EDefaultUniformData
    {
        eUserUniform = -1,
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

        eUniformsCount,
    };

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	
    class CShaderData;

    class CShaderUniform
    {
    public:

        CShaderUniform();
        virtual                             ~CShaderUniform();

        void                                setUniform(EShaderDataType type, const std::string& attribute, void* value, EDefaultUniformData data = eUserUniform);

        EShaderDataType                     getUniformType() const;
        void*                               getUniforValue() const;

        static const std::string&           getShaderUniformNameByValue(EDefaultUniformData type);
        static const EDefaultUniformData    getShaderUniformValueByName(const std::string& name);

    private:

        friend                              CShaderData;

        EDefaultUniformData                 m_defaultData;
        EShaderDataType                     m_uniformType;
        void*                               m_uniformValue;
        std::string                         m_attribute;

        void*                               allocMemory(EShaderDataType type, void* value);
        void                                deallocMemory();

        static const std::string            s_shaderUniformName[EDefaultUniformData::eUniformsCount];
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CShaderUniform>         UniformPtr;
    typedef std::map<std::string, UniformPtr>       UniformList;

    //////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_SHADER_UNIFORM_H_
