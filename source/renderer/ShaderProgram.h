#ifndef _F3D_SHADER_PROGRAM_H_
#define _F3D_SHADER_PROGRAM_H_

#include "Object.h"
#include "Shader.h"
#include "ShaderData.h"
#include "tinyxml2.h"

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
		
		virtual void	create()    = 0;
		virtual void	destroy()   = 0;
		virtual void	bind()      = 0;

	protected:

        friend          CRenderPass;
		
		void			addShader(ShaderPtr shader);
		void			destroyShader(ShaderPtr shader);

        bool            parse(tinyxml2::XMLElement* root);
        bool            parseUniforms(tinyxml2::XMLElement* root);
        bool            parseAttributes(tinyxml2::XMLElement* root);
        bool            parseSamplers(tinyxml2::XMLElement* root);
        bool            parseShaders(tinyxml2::XMLElement* root);
		
		u32				m_shaderProgID;

		bool			m_enable;
		bool			m_isActive;

		ShaderList		m_shaderList;
        CShaderDataPtr	m_shaderData;;

	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CShaderProgram> CShaderProgramPtr;

    //////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_F3D_SHADER_PROGRAM_H_