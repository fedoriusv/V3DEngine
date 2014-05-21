#ifndef _F3D_SHADER_UNIFORM_H_
#define _F3D_SHADER_UNIFORM_H_

#include "Object.h"

namespace f3d
{
namespace renderer
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EShaderUniformDataType
    {
        eUniformNone = 0,
        eUniformInt,
        eUniformFloat,
        eUniformVector2,
        eUniformVector3,
        eUniformVector4,
        eUniformMatrix3,
        eUniformMatrix4,

        eUniformTypeCount,
    };

    extern const std::string&           getShaderUniformNameByType(EShaderUniformDataType type);
    extern const EShaderUniformDataType getShaderUniformTypeByName(const std::string& name);

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EDefaultShaderData
    {
        eUserUniform = 0,

        /*Uniforms*/
        eTransformProjectionMatrix,
        eTransformModelMatrix,
        eTransformViewMatrix,
        eTransformNormalMatrix,
        eTransformViewPosition,
        eTransformOrthoMatrix,

        /*Attributes*/
        eAttributeVertex,
        eAttributeColor,
        eAttributeNormal,
        eAttributeBinormal,
        eAttributeTangent,
        eAttributeTexture0,
        eAttributeTexture1,
        eAttributeTexture2,
        eAttributeTexture3,

        eDataCount,
    };

    extern const std::string&           getShaderUniformNameByValue(EDefaultShaderData type);
    extern const EDefaultShaderData     getShaderUniformValueByName(const std::string& name);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	
    class CShaderProgram;

	class CShaderUniform : public CObject
	{
	public:

        CShaderUniform();
        virtual                 ~CShaderUniform();

        void                    setUniform(EShaderUniformDataType type, const std::string& attribute, void* value, EDefaultShaderData val = eUserUniform);

	protected:

        EDefaultShaderData      m_defaultData;
        EShaderUniformDataType  m_uniformType;
		void*                   m_uniformValue;
		std::string             m_attribute;

        void*                   allocMemory(EShaderUniformDataType type, void* value);
		void                    deallocMemory();
		
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CShaderUniform>         UniformPtr;
    typedef std::map<std::string, UniformPtr>       UniformList;

    //////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_F3D_SHADER_UNIFORM_H_