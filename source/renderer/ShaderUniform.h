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
	
	class CShaderUniform : public CObject
	{
	public:
							CShaderUniform();
							~CShaderUniform();

		void				setUniform(EShaderUniformType type, const std::string& attribute, void* value);
	protected:

		EShaderUniformType	m_uniformType;
		void*				m_uniformValue;
		std::string			m_attribute;

		void*				allocMemory(EShaderUniformType type, void* value);
		void				deallocMemory();
		
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_F3D_SHADER_UNIFORM_H_