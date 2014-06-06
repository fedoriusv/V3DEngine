#ifndef _F3D_SHADER_UNIFORM_H_
#define _F3D_SHADER_UNIFORM_H_

#include "common.h"

namespace v3d
{
namespace renderer
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EShaderDataType
    {
        eDataNone = 0,
        eDataInt,
        eDataFloat,
        eDataVector2,
        eDataVector3,
        eDataVector4,
        eDataMatrix3,
        eDataMatrix4,

        eDataTypeCount,
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
	
    class CShaderUniform
    {
    public:

        CShaderUniform();
        virtual                             ~CShaderUniform();

        void                                setUniform(EShaderDataType type, const std::string& attribute, void* value, EDefaultUniformData data = eUserUniform);

        static const std::string&           getShaderUniformNameByValue(EDefaultUniformData type);
        static const EDefaultUniformData    getShaderUniformValueByName(const std::string& name);

    private:

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

#endif //_F3D_SHADER_UNIFORM_H_