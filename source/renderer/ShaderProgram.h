#ifndef _F3D_SHADER_PROGRAM_H_
#define _F3D_SHADER_PROGRAM_H_

#include "Object.h"
#include "Shader.h"
#include "ShaderUniform.h"

namespace f3d
{
namespace renderer
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////

    class CRenderPass;

	class CShaderProgram : public CObject
	{
	public:
						CShaderProgram();
		virtual			~CShaderProgram();
		
		u32				getShaderID()   const;
		bool			isEnable()      const;
		void			setEnable(bool enable);
		
		void			setUniformInt    (const std::string& name, const u32             value );
		void			setUniformFloat  (const std::string& name, const f32             value );
		void			setUniformVector2(const std::string& name, const core::Vector2D& vector);
		void			setUniformVector3(const std::string& name, const core::Vector3D& vector);
		void			setUniformVector4(const std::string& name, const core::Vector4D& vector);
		void			setUniformMatrix3(const std::string& name, const core::Matrix3D& matrix);
		void			setUniformMatrix4(const std::string& name, const core::Vector4D& matrix);

		virtual void	create()    = 0;
		virtual void	destroy()   = 0;
		virtual void	bind()      = 0;

	protected:

        friend          CRenderPass;
		
		bool			isExist(const std::string& name); 

		void			addShader(ShaderPtr shader);
		void			destroyShader(ShaderPtr shader);
		
		u32				m_shaderProgID;

		bool			m_enable;
		bool			m_isActive;

		ShaderList		m_shaderList;
		UniformList		m_uniformList;

        void            setDefaultUniform(const std::string& name, EShaderUniformDataType type,  EDefaultShaderData data);
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CShaderProgram> CShaderProgramPtr;

    //////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_F3D_SHADER_PROGRAM_H_