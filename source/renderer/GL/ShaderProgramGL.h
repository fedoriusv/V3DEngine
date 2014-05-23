#ifndef _F3D_SHADER_PROGRAM_GL_H_
#define _F3D_SHADER_PROGRAM_GL_H_

#include "renderer/ShaderProgram.h"
#include "renderer/ShaderAttribute.h"

namespace f3d
{
namespace renderer
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    class CShaderProgramGL : public CShaderProgram
    {
    public:

        CShaderProgramGL();
        virtual ~CShaderProgramGL();

        void    create()    override;
        void    destroy()   override;
        void    bind()      override;

    private:

        bool    initProgram(u32& shaderProgram, std::vector<u32>& shaders);

        void    attachShader(u32 shaderProgram, u32 shader);
        void    detachShader(u32 shaderProgram, u32 shader);
        void    bindAttrib(u32 shaderProgram, EShaderAttribute type, const std::string& name);
        int     getAttrib(u32 shaderProgram, const std::string& name);
        void    deleteProgram(u32 shaderProgram);

    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_F3D_SHADER_PROGRAM_GL_H_