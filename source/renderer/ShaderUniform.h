#ifndef _F3D_SHADER_UNIFORM_H_
#define _F3D_SHADER_UNIFORM_H_

#include "Object.h"

namespace f3d
{
namespace renderer
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EShaderDataType
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

    extern const std::string&    getShaderDataNameByType(EShaderDataType type);
    extern const EShaderDataType getShaderDataTypeByName(const std::string& name);

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EDefaultUniformData
    {
        eUserUniform = -1,
        eTransformProjectionMatrix,
        eTransformModelMatrix,
        eTransformViewMatrix,
        eTransformNormalMatrix,
        eTransformViewPosition,
        eTransformOrthoMatrix,

        eUniformsCount,
    };

    extern const std::string&           getShaderUniformNameByType(EDefaultUniformData type);
    extern const EDefaultUniformData    getShaderUniformTypeByName(const std::string& name);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	
    class CShaderProgram;

	class CShaderUniform : public CObject
	{
	public:

        CShaderUniform();
        virtual                 ~CShaderUniform();

        void                    setUniform(EShaderDataType type, const std::string& attribute, void* value, EDefaultUniformData data = eUserUniform);

	protected:

        EDefaultUniformData     m_defaultData;
        EShaderDataType         m_uniformType;
		void*                   m_uniformValue;
		std::string             m_attribute;

        void*                   allocMemory(EShaderDataType type, void* value);
		void                    deallocMemory();
		
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CShaderUniform>         UniformPtr;
    typedef std::map<std::string, UniformPtr>       UniformList;

    //////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_F3D_SHADER_UNIFORM_H_