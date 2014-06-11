#ifndef _V3D_SHADER_PROGRAM_H_
#define _V3D_SHADER_PROGRAM_H_

#include "Object.h"
#include "Shader.h"
#include "ShaderData.h"

namespace v3d
{
namespace renderer
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    class CRenderPass;

    class CShaderProgram : public CObject
    {
    public:
        CShaderProgram(const ShaderDataPtr& data);
        virtual         ~CShaderProgram();

        u32             getShaderID()   const;
        bool            isEnable()      const;
        void            setEnable(bool enable);

		virtual bool	create()    = 0;
		virtual void	destroy()   = 0;
		virtual void	bind()      = 0;
        virtual void	unbind()    = 0;

	protected:

        friend          CRenderPass;

        void            addShader(ShaderPtr shader);
        void            destroyShader(ShaderPtr shader);

        void            getShaderIDArray(std::vector<u32>& shaders);

        virtual bool    create(const std::string& vShader, const std::string& fShader, u32 arg = 0, ...) = 0;

        virtual bool    setUniform(EShaderDataType type, const u32 shader, const std::string& attribute, void* value) = 0;

        u32             m_shaderProgID;

        bool            m_enable;
        bool            m_isActive;

        ShaderDataPtr   m_shaderData;
        ShaderList      m_shaderList;
    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    typedef std::shared_ptr<CShaderProgram> ShaderProgramPtr;

    //////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_V3D_SHADER_PROGRAM_H_
