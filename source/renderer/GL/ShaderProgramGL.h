#ifndef _F3D_SHADER_PROGRAM_GL_H_
#define _F3D_SHADER_PROGRAM_GL_H_

#include "renderer/ShaderProgram.h"

namespace v3d
{
namespace renderer
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    class CShaderProgramGL : public CShaderProgram
    {
    public:

        CShaderProgramGL(const ShaderDataPtr& data);
        virtual ~CShaderProgramGL();

        bool    create()    override;
        void    destroy()   override;
        void    bind()      override;
        void    unbind()    override;

    private:

        bool    create(const std::string& vShader, const std::string& fShader, u32 arg, ...) override;

        bool    initProgram(u32& shaderProgram, const std::vector<u32>& shaders);

        void    attachShader(u32 shaderProgram, u32 shader);
        void    detachShader(u32 shaderProgram, u32 shader);
        void    bindAttrib(u32 shaderProgram, EShaderAttribute type, const std::string& name);
        s32     getAttrib(u32 shaderProgram, const std::string& name);
        void    createProgram(u32& shaderProgram);
        void    deleteProgram(u32 shaderProgram);
        void    useProgram(u32 shaderProgram);

        bool    setUniform(EShaderDataType type, const u32 shader, const std::string& attribute, void* value) override;

    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////
}
}

#endif //_F3D_SHADER_PROGRAM_GL_H_