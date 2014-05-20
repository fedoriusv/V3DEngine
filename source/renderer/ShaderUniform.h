#ifndef _F3D_SHADER_UNIFORM_H_
#define _F3D_SHADER_UNIFORM_H_

#include "Object.h"

namespace f3d
{
namespace renderer
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////

	enum EShaderUniformType
	{
		eUniformInvalid = -1,
		eUniformInt,
		eUniformFloat,
		eUniformVector2,
		eUniformVector3,
		eUniformVector4,
		eUniformMatrix3,
		eUniformMatrix4,
		
		eCount,
	};

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    enum EDefaultShaderUniforms
    {
        eUserUniform = 0,
        eTransformProjectionMatrix,
        eTransformModelMatrix,
        eTransformViewMatrix,
        eTransformNormalMatrix,
        eTransformViewPosition,
        eTransformOrthoMatrix,

        eCount,
    };

    extern const std::string k_shaderUniformName[EDefaultShaderUniforms::eCount];

    extern const std::string& getShaderUniformNameByType(EDefaultShaderUniforms type);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	
	class CShaderUniform : public CObject
	{
	public:
							CShaderUniform();
							~CShaderUniform();

        void                setUniform(EShaderUniformType type, const std::string& attribute, void* value, EDefaultShaderUniforms val = eUserUniform);
	protected:

		EShaderUniformType	m_uniformType;
		void*				m_uniformValue;
		std::string			m_attribute;

		void*				allocMemory(EShaderUniformType type, void* value);
		void				deallocMemory();
		
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CShaderUniform>			UniformPtr;
    typedef std::map<std::string, UniformPtr>		UniformList;

    //////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_F3D_SHADER_UNIFORM_H_